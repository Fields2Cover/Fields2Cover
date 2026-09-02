//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <algorithm>
#include "fields2cover/route_planning/single_cell_swaths_order_base.h"
#include "fields2cover/route_planning/route_planner_base.h"

namespace f2c::rp {

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
  F2CGraph2D graph = RoutePlannerBase().createShortestGraph(
      cells, sorted, d_tol);
  F2CRoute route;
  for (auto&& cell_swaths : sorted) {
    for (auto&& s : cell_swaths) {
      route.addSwath(s, graph);
    }
  }
  return route;
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

