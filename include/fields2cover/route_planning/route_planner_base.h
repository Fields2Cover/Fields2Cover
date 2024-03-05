//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_ROUTE_PLANNING_BASE_H_
#define FIELDS2COVER_ROUTE_PLANNING_ROUTE_PLANNING_BASE_H_

#include <vector>
#include <map>
#include <optional>
#include <limits>
#include <utility>
#include "fields2cover/types.h"
#include "fields2cover/objectives/rp_obj/rp_objective.h"
#include "fields2cover/objectives/rp_obj/direct_dist_path_obj.h"
#include "fields2cover/route_planning/single_cell_swaths_order_base.h"

namespace f2c::rp {

class RoutePlannerBase {
 public:
  virtual F2CRoute genRoute(
      const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
      bool show_log = false, double d_tol = 1e-4);

  virtual std::vector<F2CRoute> genRoute(
      const F2CCells& cells, const std::vector<F2CSwathsByCells>& v_swaths,
      bool show_log = false, double d_tol = 1e-4);

  virtual F2CGraph2D createShortestGraph(
      const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
      double d_tol) const;

  virtual F2CGraph2D createCoverageGraph(
      const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
      F2CGraph2D& shortest_graph,
      double d_tol) const;

  virtual std::vector<int64_t> computeBestRoute(
      const F2CGraph2D& cov_graph, bool show_log) const;

  virtual F2CRoute transformSolutionToRoute(
      const std::vector<int64_t>& route_ids,
      const F2CSwathsByCells& swaths_by_cells,
      const F2CGraph2D& coverage_graph,
      F2CGraph2D& shortest_graph) const;

 public:
  std::optional<F2CPoint> r_start_end;
};



}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_ROUTE_PLANNING_BASE_H_

