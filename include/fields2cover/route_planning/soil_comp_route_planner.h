//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_SOIL_COMP_ROUTE_PLANNER_BASE_H_
#define FIELDS2COVER_ROUTE_PLANNING_SOIL_COMP_ROUTE_PLANNER_BASE_H_

#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/route_planning/route_planner_base.h"

namespace f2c::rp {

class SoilCompRoutePlanner : public RoutePlannerBase {
 public:
  using RoutePlannerBase::genRoute;

  // std::vector<int64_t> soil_costs {1000, 800, 500, 300, 100, 50};
  std::vector<int64_t> soil_costs {1000, 10, 1};


  void setSoilCosts(const std::vector<int64_t>& costs) {
    this->soil_costs = costs;
  }

  F2CRoute genRoute(
      const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
      bool show_log = false, double d_tol = 1e-4, bool redirect_swaths = true) const override;
  std::vector<F2CRoute> genRoute(
      const F2CCells& cells, const std::vector<F2CSwathsByCells>& v_swaths,
      bool show_log = false, double d_tol = 1e-4) const override;

  F2CGraph2D updateCosts(
      const F2CGraph2D& shortest_graph, const F2CRoute& route) const;
  F2CGraph2D updateCosts(
      const F2CGraph2D& shortest_graph,
      const std::vector<F2CRoute>& routes) const;

  int64_t computeSoilCompaction(
      const F2CGraph2D& shortest_graph,
      const std::vector<F2CRoute>& routes) const;
};



}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_SOIL_COMP_ROUTE_PLANNER_BASE_H_

