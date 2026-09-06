//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <set>
#include <thread>
#include <utility>
#include <vector>
#include "fields2cover/route_planning/free_space_route_planner.h"

namespace f2c::rp {

namespace {

// A ring flattened into plain arrays, with its box. Asking a polygon whether it
// holds a point is the inner loop of a quadratic graph, so it is worth keeping
// out of the geometry library.
struct Ring {
  std::vector<double> x, y;
  double x0 {1e18}, x1 {-1e18}, y0 {1e18}, y1 {-1e18};

  void add(double px, double py) {
    x.push_back(px);
    y.push_back(py);
    x0 = std::min(x0, px); x1 = std::max(x1, px);
    y0 = std::min(y0, py); y1 = std::max(y1, py);
  }

  bool holds(double px, double py) const {
    if (px < x0 || px > x1 || py < y0 || py > y1) {
      return false;
    }
    bool in = false;
    const size_t n = x.size();
    for (size_t i = 0, j = n - 1; i < n; j = i++) {
      if (((y[i] > py) != (y[j] > py)) &&
          (px < (x[j] - x[i]) * (py - y[i]) / (y[j] - y[i]) + x[i])) {
        in = !in;
      }
    }
    return in;
  }
};

struct Poly {
  Ring outer;
  std::vector<Ring> holes;

  bool holds(double px, double py) const {
    if (!outer.holds(px, py)) {
      return false;
    }
    for (const auto& h : holes) {
      if (h.holds(px, py)) {
        return false;
      }
    }
    return true;
  }
};

class Area {
 public:
  explicit Area(const F2CCells& cs) {
    for (size_t i = 0; i < cs.size(); ++i) {
      const F2CCell c = cs.getGeometry(i);
      Poly p;
      readRing(c.getExteriorRing(), &p.outer);
      for (size_t r = 0; r + 1 < c.size(); ++r) {
        Ring h;
        readRing(c.getInteriorRing(r), &h);
        p.holes.push_back(std::move(h));
      }
      polys_.push_back(std::move(p));
    }
  }

  bool holds(double px, double py) const {
    for (const auto& p : polys_) {
      if (p.holds(px, py)) {
        return true;
      }
    }
    return false;
  }

  bool isEmpty() const { return polys_.empty(); }

  // Every ring edge, for the crossing test.
  void edges(std::vector<std::array<double, 4>>* out) const {
    for (const auto& p : polys_) {
      pushRing(p.outer, out);
      for (const auto& h : p.holes) {
        pushRing(h, out);
      }
    }
  }

 private:
  static void readRing(const F2CLinearRing& r, Ring* out) {
    for (size_t k = 0; k < r.size(); ++k) {
      const F2CPoint v = r.getGeometry(k);
      out->add(v.getX(), v.getY());
    }
  }
  static void pushRing(const Ring& r, std::vector<std::array<double, 4>>* out) {
    for (size_t i = 0; i + 1 < r.x.size(); ++i) {
      out->push_back({r.x[i], r.y[i], r.x[i + 1], r.y[i + 1]});
    }
  }
  std::vector<Poly> polys_;
};

double side(double ox, double oy, double ax, double ay, double bx, double by) {
  return (ax - ox) * (by - oy) - (ay - oy) * (bx - ox);
}

int sign(double v, double eps) { return v > eps ? 1 : (v < -eps ? -1 : 0); }

// Whether two segments cross properly. Meeting at an end does not count: the
// nodes sit on the border, so every edge touches it.
bool crosses(const std::array<double, 4>& s, double px, double py,
    double qx, double qy, double eps) {
  const int d1 = sign(side(s[0], s[1], s[2], s[3], px, py), eps);
  const int d2 = sign(side(s[0], s[1], s[2], s[3], qx, qy), eps);
  const int d3 = sign(side(px, py, qx, qy, s[0], s[1]), eps);
  const int d4 = sign(side(px, py, qx, qy, s[2], s[3]), eps);
  return d1 * d2 < 0 && d3 * d4 < 0;
}

void ringCorners(const F2CCells& cs, std::vector<F2CPoint>* out) {
  for (size_t i = 0; i < cs.size(); ++i) {
    const F2CCell c = cs.getGeometry(i);
    for (size_t r = 0; r < c.size(); ++r) {
      const F2CLinearRing ring =
          (r == 0) ? c.getExteriorRing() : c.getInteriorRing(r - 1);
      for (size_t k = 0; k + 1 < ring.size(); ++k) {
        out->push_back(ring.getGeometry(k));
      }
    }
  }
}

// Union-find, to count the pieces the graph falls into.
class Pieces {
 public:
  explicit Pieces(size_t n) : parent_(n) {
    std::iota(parent_.begin(), parent_.end(), 0);
  }
  size_t root(size_t a) {
    return parent_[a] == a ? a : parent_[a] = root(parent_[a]);
  }
  void join(size_t a, size_t b) { parent_[root(a)] = root(b); }
  size_t count() {
    std::set<size_t> roots;
    for (size_t i = 0; i < parent_.size(); ++i) {
      roots.insert(root(i));
    }
    return roots.size();
  }

 private:
  std::vector<size_t> parent_;
};

}  // namespace


F2CGraph2D FreeSpaceRoutePlanner::createShortestGraph(
    const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
    double d_tol) const {
  F2CGraph2D g;
  if (cells.size() == 0) {
    n_components_ = 0;
    return g;
  }

  const Area ground(cells);
  std::vector<std::array<double, 4>> border;
  ground.edges(&border);

  // The band along the edge the route should stay out of where it can. Priced,
  // not forbidden: a swath end sits on the border and has no other way out.
  const bool has_band = (clearance_ > 0.0 && clearance_cost_ > 0.0);
  const F2CCells inner =
      has_band ? cells.buffer(-clearance_) : F2CCells();
  const Area room(inner);

  // Nodes: every swath end, and every corner of the ground.
  std::vector<F2CPoint> nodes;
  const F2CCells outline =
      (corner_tol_ > 0.0) ? cells.simplify(corner_tol_) : cells;
  ringCorners(outline, &nodes);
  const size_t n_corners = nodes.size();
  for (auto&& swaths : swaths_by_cells) {
    for (auto&& s : swaths) {
      nodes.push_back(s.startPoint());
      nodes.push_back(s.endPoint());
    }
  }
  if (nodes.size() < 2) {
    n_components_ = nodes.size();
    return g;
  }

  const double step = (sample_step_ > 0.0) ? sample_step_ : 0.5;
  const double eps = std::max(d_tol, 1e-9);

  // Whether the segment between two nodes stays on the ground, and how much of
  // it runs inside the clearance band.
  auto reaches = [&](const F2CPoint& a, const F2CPoint& b,
      bool a_end, bool b_end,
      double* in_band) -> bool {
    const double ax = a.getX(), ay = a.getY();
    const double bx = b.getX(), by = b.getY();
    const double dx = bx - ax, dy = by - ay;
    const double len2 = dx * dx + dy * dy;
    if (len2 < 1e-12) {
      return false;
    }
    for (const auto& s : border) {
      if (crosses(s, ax, ay, bx, by, eps)) {
        return false;
      }
    }
    // Crossing alone is not enough: a chord between two corners can clip a spur
    // by entering and leaving at corners, which is not a proper crossing.
    const double len = std::sqrt(len2);
    const int n = std::max(2, static_cast<int>(std::ceil(len / step)));
    const double dl = len / n;
    const double free_of_band = 1.5 * clearance_;
    double band = 0.0;
    for (int t = 0; t < n; ++t) {
      const double u = (t + 0.5) / n;
      const double mx = ax + dx * u, my = ay + dy * u;
      if (!ground.holds(mx, my)) {
        return false;
      }
      if (has_band && !room.isEmpty() && !room.holds(mx, my)) {
        const double from_a = a_end ? std::hypot(mx - ax, my - ay) : 1e18;
        const double from_b = b_end ? std::hypot(mx - bx, my - by) : 1e18;
        if (std::min(from_a, from_b) > free_of_band) {
          band += dl;
        }
      }
    }
    if (in_band != nullptr) {
      *in_band = band;
    }
    return true;
  };

  // The pairs are independent, so share them out; F2CGraph2D is filled after.
  struct Edge { size_t i, k; double band; };
  unsigned n_threads = std::thread::hardware_concurrency();
  n_threads = std::max(1u, std::min(n_threads, 16u));
  std::vector<std::vector<Edge>> found(n_threads);
  std::vector<std::thread> workers;
  for (unsigned t = 0; t < n_threads; ++t) {
    workers.emplace_back([&, t] {
      for (size_t i = t; i < nodes.size(); i += n_threads) {
        for (size_t k = i + 1; k < nodes.size(); ++k) {
          double band = 0.0;
          if (reaches(nodes[i], nodes[k], i >= n_corners, k >= n_corners,
                &band)) {
            found[t].push_back({i, k, band});
          }
        }
      }
    });
  }
  for (auto& w : workers) {
    w.join();
  }

  Pieces pieces(nodes.size());
  for (const auto& v : found) {
    for (const auto& e : v) {
      const double len = nodes[e.i].distance(nodes[e.k]);
      const double cost = len + clearance_cost_ * e.band;
      g.addEdge(nodes[e.i], nodes[e.k], static_cast<int64_t>(1e3 * cost));
      pieces.join(e.i, e.k);
    }
  }
  n_components_ = pieces.count();
  return g;
}

void FreeSpaceRoutePlanner::setClearance(double clearance) {
  this->clearance_ = std::fabs(clearance);
}

double FreeSpaceRoutePlanner::getClearance() const {
  return this->clearance_;
}

void FreeSpaceRoutePlanner::setClearanceCost(double cost) {
  this->clearance_cost_ = std::fabs(cost);
}

double FreeSpaceRoutePlanner::getClearanceCost() const {
  return this->clearance_cost_;
}

void FreeSpaceRoutePlanner::setCornerTolerance(double tol) {
  this->corner_tol_ = std::fabs(tol);
}

double FreeSpaceRoutePlanner::getCornerTolerance() const {
  return this->corner_tol_;
}

void FreeSpaceRoutePlanner::setSampleStep(double step) {
  this->sample_step_ = std::fabs(step);
}

double FreeSpaceRoutePlanner::getSampleStep() const {
  return this->sample_step_;
}

size_t FreeSpaceRoutePlanner::getComponentCount() const {
  return this->n_components_;
}

}  // namespace f2c::rp
