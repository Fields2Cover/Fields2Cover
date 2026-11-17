//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <algorithm>
#include <cstdint>
#include <limits>
#include <optional>
#include <vector>
#include "fields2cover/route_planning/single_cell_swaths_order_base.h"
#include "fields2cover/route_planning/route_planner_base.h"

namespace f2c::rp {

namespace {

// Traversing a cell the other way round: the swaths in reverse order, each of
// them driven backwards.
F2CSwaths reversed(const F2CSwaths& swaths) {
  F2CSwaths out = swaths.clone();
  out.reverse();
  for (size_t i = 0; i < out.size(); ++i) {
    out.at(i).reverse();
  }
  return out;
}

// Cost of leaving a cell at \p from, heading \p from_angle, and entering the
// next one at \p entry, heading \p entry_angle.
//
// The distance comes from the headland graph, not from a straight line: a cell
// on the other side of an obstacle is far to drive to even when it looks near.
// The two angle terms charge the turn needed to leave and the turn needed to
// line up on entry, so the route does not enter a cell against its direction.
double entryCost(const F2CPoint& from, const std::optional<double>& from_angle,
    const F2CPoint& entry, double entry_angle, double width, F2CGraph2D& graph) {
  // The graph reports what it cannot reach as the value it was handed, so the
  // sentinel is passed in rather than guessed at: cells on the other side of a
  // gap in the graph are then ordered by straight-line distance instead.
  constexpr int64_t kUnreachable = 1LL << 40;
  double dist = from.distance(entry);
  const int64_t cost = graph.shortestPathCost(from, entry, kUnreachable);
  if (cost < kUnreachable) {
    dist = static_cast<double>(cost) * 1e-3;
  }
  if (dist < 1e-6) {
    return dist;
  }
  const double bearing = (entry - from).getAngleFromPoint();
  double turn = F2CPoint::getAngleDiffAbs(bearing, entry_angle);
  if (from_angle) {
    turn += F2CPoint::getAngleDiffAbs(*from_angle, bearing);
  }
  return dist + width * turn;
}

// Order the cells, and pick which end of each one to enter, so consecutive
// cells are cheap to reach. The cells come in the order the decomposition
// produced them, which carries no geometric meaning of its own.
F2CSwathsByCells orderCells(const F2CSwathsByCells& sorted,
    const std::optional<F2CPoint>& start, F2CGraph2D& graph) {
  std::vector<size_t> left;
  for (size_t i = 0; i < sorted.size(); ++i) {
    if (sorted.at(i).size() > 0) {
      left.emplace_back(i);
    }
  }
  F2CSwathsByCells ordered;
  if (left.empty()) {
    return ordered;
  }
  if (!start) {
    // No point to start from, so the first cell is the one that came first.
    ordered.emplace_back(sorted.at(left.front()));
    left.erase(left.begin());
  }

  while (!left.empty()) {
    std::optional<F2CPoint> from;
    std::optional<double> from_angle;
    if (ordered.size() > 0) {
      const F2CSwath& last = ordered.back().back();
      from = last.endPoint();
      from_angle = last.getOutAngle();
    } else {
      from = *start;
    }

    auto best = left.begin();
    bool best_reversed = false;
    double best_cost = std::numeric_limits<double>::max();
    for (auto it = left.begin(); it != left.end(); ++it) {
      const F2CSwaths& cell = sorted.at(*it);
      const double width = cell.at(0).getWidth();
      const double as_is = entryCost(*from, from_angle, cell.at(0).startPoint(),
          cell.at(0).getInAngle(), width, graph);
      if (as_is < best_cost) {
        best_cost = as_is; best = it; best_reversed = false;
      }
      const double flipped = entryCost(*from, from_angle, cell.back().endPoint(),
          cell.back().getOutAngle() + M_PI, width, graph);
      if (flipped < best_cost) {
        best_cost = flipped; best = it; best_reversed = true;
      }
    }
    ordered.emplace_back(
        best_reversed ? reversed(sorted.at(*best)) : sorted.at(*best));
    left.erase(best);
  }
  return ordered;
}

}  // namespace

F2CSwaths SingleCellSwathsOrderBase::genSortedSwaths(
    const F2CSwaths& swaths, uint32_t variant) const {
  // Always sort swaths to work with them in the same direction.
  F2CSwaths new_swaths = swaths.clone();
  if (new_swaths.size() > 0) {
    new_swaths.sort();
    this->changeStartPoint(new_swaths, variant);
    this->sortSwaths(new_swaths);
    new_swaths.reverseDirOddSwaths();
  }
  return new_swaths;
}

F2CSwathsByCells SingleCellSwathsOrderBase::genSortedSwaths(
    const F2CSwathsByCells& swaths, uint32_t variant) const {
  F2CSwathsByCells sorted;
  for (auto&& cell_swaths : swaths) {
    sorted.emplace_back(this->genSortedSwaths(cell_swaths, variant));
  }
  return sorted;
}

F2CRoute SingleCellSwathsOrderBase::genRoute(
    const F2CCells& cells, const F2CSwathsByCells& swaths, double d_tol) const {
  F2CSwathsByCells sorted = this->genSortedSwaths(swaths);
  // A bare order ignores the boundary, so a skip cuts over covered ground.
  // Reuse the headland graph the route planner already builds.
  // The planner is told about the start point too, so the graph carries it as
  // a node and the legs to and from it follow the headland like any other
  // connection.
  RoutePlannerBase planner;
  if (this->r_start_end) {
    planner.setStartAndEndPoint(*this->r_start_end);
  }
  F2CGraph2D graph = planner.createShortestGraph(cells, sorted, d_tol);

  F2CRoute route;
  bool first = true;
  for (auto&& cell_swaths : orderCells(sorted, this->r_start_end, graph)) {
    for (auto&& s : cell_swaths) {
      if (first) {
        if (this->r_start_end) {
          route.addConnection(
              graph.shortestPath(*this->r_start_end, s.startPoint()));
        }
        route.addSwath(s);
        first = false;
      } else {
        route.addSwath(s, graph);
      }
    }
  }
  if (this->r_start_end && !route.isEmpty()) {
    route.addConnection(graph.shortestPath(route.endPoint(), *this->r_start_end));
  }
  return route;
}

void SingleCellSwathsOrderBase::setStartAndEndPoint(const F2CPoint& p) {
  this->r_start_end = p;
}

void SingleCellSwathsOrderBase::changeStartPoint(
    F2CSwaths& swaths, uint32_t variant) const {
  if (variant & 1) {
    swaths.reverse();
  }
  if (variant & 2) {
    swaths[0].reverse();
  }
}



}  // namespace f2c::rp

