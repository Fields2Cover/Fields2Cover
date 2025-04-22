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
  /// Generate route to cover the swaths on a field.
  ///   If two consecutive swaths are far away,
  ///   the route connects both through the headland.
  ///
  /// @param cells Headland swath rings used to travel through the headlands
  /// @param swaths_by_cells Swaths to be covered.
  /// @param show_log Show log from the optimizer
  /// @param d_tol Tolerance distance to consider if two points are the same.
  /// @param redirect_swaths Whether to allow redirecting swaths
  /// @param time_limit_seconds Maximum time to spend on optimization
  /// @param search_for_optimum If true, uses guided local search which may take longer
  ///        but can find more optimal solutions. If false, uses automatic search which is faster
  ///        but may find less optimal solutions.
  /// @return Route that covers all the swaths
  virtual F2CRoute genRoute(
      const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
      bool show_log = false, double d_tol = 1e-4, bool redirect_swaths = true,
      long int time_limit_seconds = 1, bool search_for_optimum = false);

  /// Set the start and the end of the route.
  void setStartAndEndPoint(const F2CPoint& p);

  /// Create graph to compute the shortest path between two points
  ///   in the headlands.
  ///
  /// @param cells Headland swath rings used to travel through the headlands
  /// @param swaths_by_cells Swaths to be covered.
  /// @param d_tol Tolerance distance to consider if two points are the same.
  virtual F2CGraph2D createShortestGraph(
      const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
      double d_tol) const;

  /// Create graph to compute the cost of covering the swaths in a given order.
  ///
  /// @param cells Headland swath rings used to travel through the headlands
  /// @param swaths_by_cells Swaths to be covered.
  /// @param shortest_graph Graph to compute the shortest path
  ///          between two nodes.
  /// @param d_tol Tolerance distance to consider if two points are the same.
  virtual F2CGraph2D createCoverageGraph(
      const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
      F2CGraph2D& shortest_graph,
      double d_tol, bool redirect_swaths = true) const;


  virtual ~RoutePlannerBase() = default;

  /// Use the optimizer to generate the index of the points of the best
  ///   coverage route.
  /// @param cov_graph Graph representing the coverage problem
  /// @param show_log Whether to show optimization logs
  /// @param time_limit_seconds Maximum time to spend on optimization
  /// @param use_guided_local_search If true, uses guided local search which may take longer
  ///        but can find more optimal solutions. If false, uses automatic search which is faster
  ///        but may find less optimal solutions.
  virtual std::vector<long long int> computeBestRoute(
      const F2CGraph2D& cov_graph, bool show_log, long int time_limit_seconds,
      bool use_guided_local_search = true) const;

  /// Tranform index of points to an actual Route.
  virtual F2CRoute transformSolutionToRoute(
      const std::vector<long long int>& route_ids,
      const F2CSwathsByCells& swaths_by_cells,
      const F2CGraph2D& coverage_graph,
      F2CGraph2D& shortest_graph) const;

 protected:
  std::optional<F2CPoint> r_start_end;
};



}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_ROUTE_PLANNING_BASE_H_

