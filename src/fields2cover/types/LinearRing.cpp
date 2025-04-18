//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/LinearRing.h"
#include "fields2cover/types/LineString.h"

namespace f2c::types {

LinearRing::LinearRing() {
  this->data_ = std::shared_ptr<OGRLinearRing>(
    static_cast<OGRLinearRing*>(
      OGRGeometryFactory::createGeometry(wkbLinearRing)),
    [](OGRLinearRing* f) {OGRGeometryFactory::destroyGeometry(f);});
}

LinearRing::LinearRing(const std::vector<Point>& ps) {
  for (auto&& p : ps) {
    this->addPoint(p);
  }
}

LinearRing::LinearRing(const std::initializer_list<Point>& ps) {
  for (auto&& p : ps) {
    this->addPoint(p);
  }
}

void LinearRing::operator*=(double b) {
  for (auto&& p : *this) {
    p *= b;
  }
}

double LinearRing::getX(size_t i) const {
  return this->data_->getX(i);
}

double LinearRing::getY(size_t i) const {
  return this->data_->getY(i);
}
double LinearRing::getZ(size_t i) const {
  return this->data_->getZ(i);
}
double LinearRing::length() const {
  return this->data_->get_Length();
}

void LinearRing::reversePoints() {
  this->data_->reversePoints();
}

LinearRing& LinearRing::changeStartPoint(int id_new_start) {
  LinearRing new_ring;
  int N = static_cast<int>(this->size()) - (this->isClosed() ? 1 : 0);
  int start = id_new_start % N;
  if (start < 0) start += N;
  for (size_t i = 0; i < N; ++i) {
    int idx = start + i;
    if (idx >= N) {
      idx -= N;
    }
    new_ring.addPoint(this->getGeometry(idx));
  }
  new_ring.addPoint(new_ring.startPoint());
  *this = new_ring;
  return *this;
}

size_t LinearRing::size() const {
  return isEmpty() ? 0 : this->data_->getNumPoints();
}

LineString LinearRing::toLineString() const {
  return static_cast<LineString>(*this);
}

void LinearRing::getGeometry(size_t i, Point& point) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearRing does not contain point " +
        std::to_string(i));
  }
  data_->getPoint(i, point.get());
}

void LinearRing::getGeometry(size_t i, Point& point) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearRing does not contain point " +
        std::to_string(i));
  }
  data_->getPoint(i, point.get());
}

Point LinearRing::getGeometry(size_t i) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearRing does not contain point " +
        std::to_string(i));
  }
  OGRPoint point;
  data_->getPoint(i, &point);
  return Point(point);
}

const Point LinearRing::getGeometry(size_t i) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearRing does not contain point " +
        std::to_string(i));
  }
  return Point(data_->getX(i), data_->getY(i), data_->getZ(i));
}

void LinearRing::setGeometry(size_t i, const Point& p) {
  data_->setPoint(i, p.getX(), p.getY(), p.getZ());
}

void LinearRing::addGeometry(const Point& p) {
  this->addPoint(p);
}

void LinearRing::addPoint(double x, double y, double z) {
  data_->addPoint(x, y, z);
}

void LinearRing::addPoint(const Point& p) {
  data_->addPoint(p.getX(), p.getY(), p.getZ());
}

LinearRing& LinearRing::removePoint(size_t i) {
  this->data_->removePoint(i);
  return *this;
}

const Point LinearRing::startPoint() const {
  return getGeometry(0);
}

const Point LinearRing::endPoint() const {
  return getGeometry(size()-1);
}

bool LinearRing::isClockwise() const {
  return data_->isClockwise();
}

bool LinearRing::isClosed() const {
  return this->size() > 2 && this->startPoint() == this->endPoint();
}

double LinearRing::minAltitude() const {
  double alt {std::numeric_limits<double>::max()};
  for (size_t i = 0; i < size() - 1; ++i) {
    Point pi = this->getGeometry(i);
    Point pi1 = this->getGeometry(i + 1);

    double max_d {std::numeric_limits<double>::min()};
    double min_d {std::numeric_limits<double>::max()};
    for (size_t j = 0; j < size(); ++j) {
      double d = this->getGeometry(j).signedDistance2Segment(pi, pi1);
      if (max_d < d) {max_d = d;}
      if (min_d > d) {min_d = d;}
    }
    double dist = max_d - min_d;
    if (dist < alt && dist > 0) {
      alt = dist;
    }
  }
  return alt;
}

Point LinearRing::closestPointTo(const Point& p) const {
  std::vector<double> dist;
  std::vector<Point> ps;
  for (size_t i = 0; i < this->size() - 1; ++i) {
    ps.emplace_back(p.closestPointInSegment(this->at(i), this->at(i+1)));
    dist.emplace_back(ps.back().distance(p));
  }
  return ps[std::min_element(dist.begin(), dist.end()) - dist.begin()];
}

LinearRing& LinearRing::bufferOutwards(const std::vector<double>& d) {
  auto ds = d;
  if (!this->isClockwise()) {
    for (auto&& d_val : ds) {
      d_val *= -1.0;
    }
  }
  *this = this->getParallelLine(ds);
  return *this;
}

LinearRing& LinearRing::bufferInwards(const std::vector<double>& d) {
  auto ds = d;
  if (this->isClockwise()) {
    for (auto&& d_val : ds) {
      d_val *= -1.0;
    }
  }
  *this = this->getParallelLine(ds);
  return *this;
}




std::vector<size_t> LinearRing::getLongestEdgeIds(double ang) const {
  std::vector<std::vector<size_t>> edges;
  std::vector<size_t> edge {0, 1};

  auto getAng = [] (const Point& p1, const Point& p2, const Point& p3) {
    return M_PI - fabs(M_PI - Point::getAngleFromPoints(p1, p2, p3));
  };
  for (size_t i = 2; i < size(); ++i) {
    if (getAng(at(i-2), at(i-1), at(i)) < ang) {
      edge.emplace_back(i);
    } else {
      edges.emplace_back(edge);
      edge = std::vector<size_t>({i-1, i});
    }
  }
  if (edges.size() > 0 && getAng(at(size()-2), at(0), at(1)) < ang) {
    edge.pop_back();
    for (size_t i = 0; i < edges[0].size(); ++i) {
      edge.emplace_back(edges[0][i]);
    }
  }
  edges.emplace_back(edge);

  double longest_d {0.0};
  for (auto&& e : edges) {
    double d {0.0};
    for (size_t i = 1; i < e.size(); ++i) {
      d += at(e[i-1]).distance(at(e[i]));
    }
    if (d > longest_d) {
      longest_d = d;
      edge = e;
    }
  }
  return edge;
}

LineString LinearRing::getLongestEdge(double ang) const {
  auto edge_ids = this->getLongestEdgeIds(ang);
  LineString edge;
  for (size_t id : edge_ids) {
    edge.addPoint(this->at(id));
  }
  return edge;
}


LinearRing& LinearRing::filterSelfIntersections() {
  if (this->size() <= 3) {
    return *this;
  }
  for (size_t i = 0; i + 2 < size(); ++i) {
    auto s_i = this->getSegment(i);
    auto s_j = this->getSegment(i+1);
    if (s_i.within(s_j) || s_i.contains(s_j)) {
      this->removePoint(i+1);
    }
  }
  for (size_t i = 0; i + 2 < size(); ++i) {
    for (size_t j = this->size() - 2; j > i + 1; --j) {
      if (i == 0 && j + 2 == size()) {continue;}
      if (this->getSegment(i).intersects(this->getSegment(j))) {
        auto p_i = Point::intersectionOfLines(at(i), at(i+1), at(j), at(j+1));
        for (size_t k = j; k > i + 1; --k) {
          this->removePoint(k);
        }
        this->setGeometry(i+1, p_i);
        break;
      }
    }
  }
  return *this;
}

LineString LinearRing::getSegment(size_t i) const {
  return LineString(this->at(i), this->at(i+1));
}

LineString LinearRing::getLastSegment() const {
  return this->getSegment(this->size()-2);
}

double LinearRing::segmentLength(size_t i) const {
  return this->at(i).distance(this->at(i+1));
}

double LinearRing::segmentAng(size_t i) const {
  return (this->at(i + 1) - this->at(i)).getAngleFromPoint();
}

LinearRing LinearRing::getParallelLine(const std::vector<double>& d) const {
  if (this->size() != d.size() + 1) {
    throw std::invalid_argument(
        "Error LinearRing::getParallelLine. "  \
        "LinearRing::size() and d.size() + 1 are not equal");
  }

  std::vector<Point> ps0, ps1;
  for (size_t i = 0; i < d.size(); ++i) {
    double ang = this->segmentAng(i);
    ps0.emplace_back(at(i).getPointFromAngle(ang + 0.5 * M_PI, d[i]));
    ps1.emplace_back(at(i+1).getPointFromAngle(ang + 0.5 * M_PI, d[i]));
  }

  LinearRing ring;
  ring.addPoint(Point::intersectionOfLines(
        ps0.back(), ps1.back(), ps0[0], ps1[0]));
  for (size_t i = 1; i < ps0.size(); ++i) {
    ring.addPoint(Point::intersectionOfLines(
          ps0[i-1], ps1[i-1], ps0[i], ps1[i]));
  }
  ring.addPoint(ring[0]);
  ring.filterSelfIntersections();
  return ring;
}

LinearRing LinearRing::getParallelLine(double d) const {
  return getParallelLine(std::vector<double>(this->size()-1, d));
}


}  // namespace f2c::types

