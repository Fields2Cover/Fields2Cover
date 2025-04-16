//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <algorithm>
#include "fields2cover/route_planning/single_cell_swaths_order_base.h"

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

void SingleCellSwathsOrderBase::changeStartPoint(
    F2CSwaths& swaths, uint32_t variant) const {
  if (variant & 1) {
    swaths.reverse();
  }
  if (variant & 2) {
    swaths[0].reverse();
  }
}


F2CRoute SingleCellSwathsOrderBase::genRoute(
    const F2CMultiLineString& transit_lanes, const F2CSwathsByCells& swaths,
    double d_tol) const {
  auto swaths_sorted = this->genSortedSwaths(swaths.flatten());
  F2CGraph2D shortest_graph = createShortestGraph(transit_lanes, swaths, d_tol);
  F2CRoute route;
  for (auto&& s : swaths_sorted) {
    route.addSwath(s, shortest_graph);
  }
  return route;
}

F2CRoute SingleCellSwathsOrderBase::genRoute(
    const std::vector<F2CMultiLineString>& transit_lanes,
    const F2CSwathsByCells& swaths,
    double d_tol) const {
  F2CMultiLineString lines;
  for (auto&& line : transit_lanes) {
    lines.append(line);
  }
  return this->genRoute(lines, swaths, d_tol);
}

F2CRoute SingleCellSwathsOrderBase::genRoute(
    const std::vector<F2CMultiLineString>& transit_lanes,
    const F2CCells& hl_area,
    const F2CSwathsByCells& swaths,
    double d_tol) const {
  return this->genRoute(
      F2CMultiLineString::connectMultiLineStrings(transit_lanes, hl_area),
      swaths, d_tol);
}

F2CRoute SingleCellSwathsOrderBase::genRoute(
    const F2CCells& cells, const F2CSwathsByCells& swaths, double d_tol) const {
  return this->genRoute(cells.getLineSections(), swaths, d_tol);
}


F2CGraph2D SingleCellSwathsOrderBase::createShortestGraph(
    const F2CMultiLineString& transit_lines,
    const F2CSwathsByCells& swaths_by_cells,
    double d_tol) const {
  F2CGraph2D g;
  return g.addEdges(transit_lines)
          .addEdgesSwathExtremes(swaths_by_cells, transit_lines)
          .reconnectEdges(d_tol);
}


}  // namespace f2c::rp

