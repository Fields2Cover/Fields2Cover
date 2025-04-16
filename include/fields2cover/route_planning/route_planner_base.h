//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_ROUTE_PLANNING_BASE_H_
#define FIELDS2COVER_ROUTE_PLANNING_ROUTE_PLANNING_BASE_H_

#include <ortools/constraint_solver/routing.h>
#include <ortools/constraint_solver/routing_enums.pb.h>
#include <ortools/constraint_solver/routing_index_manager.h>
#include <ortools/constraint_solver/routing_parameters.h>
#include <limits>
#include <map>
#include <optional>
#include <utility>
#include <vector>

#include "fields2cover/types.h"
#include "fields2cover/objectives/rp_obj/rp_objective.h"
#include "fields2cover/objectives/rp_obj/direct_dist_path_obj.h"
#include "fields2cover/route_planning/single_cell_swaths_order_base.h"

namespace f2c::rp {

enum class OrtoolsFirstSolStrategy {
  AUTOMATIC = static_cast<int>(
      operations_research::FirstSolutionStrategy::AUTOMATIC),
  PATH_CHEAPEST_ARC = static_cast<int>(
      operations_research::FirstSolutionStrategy::PATH_CHEAPEST_ARC),
  PATH_MOST_CONSTRAINED_ARC = static_cast<int>(
      operations_research::FirstSolutionStrategy::PATH_MOST_CONSTRAINED_ARC),
  EVALUATOR_STRATEGY = static_cast<int>(
      operations_research::FirstSolutionStrategy::EVALUATOR_STRATEGY),
  SAVINGS = static_cast<int>(
      operations_research::FirstSolutionStrategy::SAVINGS),
  SWEEP = static_cast<int>(
      operations_research::FirstSolutionStrategy::SWEEP),
  CHRISTOFIDES = static_cast<int>(
      operations_research::FirstSolutionStrategy::CHRISTOFIDES),
  ALL_UNPERFORMED = static_cast<int>(
      operations_research::FirstSolutionStrategy::ALL_UNPERFORMED),
  BEST_INSERTION = static_cast<int>(
      operations_research::FirstSolutionStrategy::BEST_INSERTION),
  PARALLEL_CHEAPEST_INSERTION = static_cast<int>(
      operations_research::FirstSolutionStrategy::PARALLEL_CHEAPEST_INSERTION),
  LOCAL_CHEAPEST_INSERTION = static_cast<int>(
      operations_research::FirstSolutionStrategy::LOCAL_CHEAPEST_INSERTION),
  GLOBAL_CHEAPEST_ARC = static_cast<int>(
      operations_research::FirstSolutionStrategy::GLOBAL_CHEAPEST_ARC),
  LOCAL_CHEAPEST_ARC = static_cast<int>(
      operations_research::FirstSolutionStrategy::LOCAL_CHEAPEST_ARC),
  FIRST_UNBOUND_MIN_VALUE = static_cast<int>(
      operations_research::FirstSolutionStrategy::FIRST_UNBOUND_MIN_VALUE)
};

enum class OrtoolsLocalSearch {
  AUTOMATIC = static_cast<int>(
      operations_research::LocalSearchMetaheuristic::AUTOMATIC),
  GREEDY_DESCENT = static_cast<int>(
      operations_research::LocalSearchMetaheuristic::GREEDY_DESCENT),
  GUIDED_LOCAL_SEARCH = static_cast<int>(
      operations_research::LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH),
  SIMULATED_ANNEALING = static_cast<int>(
      operations_research::LocalSearchMetaheuristic::SIMULATED_ANNEALING),
  TABU_SEARCH = static_cast<int>(
      operations_research::LocalSearchMetaheuristic::TABU_SEARCH),
  GENERIC_TABU_SEARCH = static_cast<int>(
      operations_research::LocalSearchMetaheuristic::GENERIC_TABU_SEARCH)
};




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
  /// @return Route that covers all the swaths
  virtual F2CRoute genRoute(
      const F2CCells& cells,
      const F2CSwathsByCells& swaths_by_cells,
      bool show_log = false, double d_tol = 1e-4,
      bool redirect_swaths = true) const;

  virtual F2CRoute genRoute(
      const F2CMultiLineString& transit_lanes,
      const F2CSwathsByCells& swaths_by_cells,
      bool show_log = false, double d_tol = 1e-4,
      bool redirect_swaths = true) const;

  virtual F2CRoute genRoute(
      const std::vector<F2CMultiLineString>& transit_lanes,
      const F2CSwathsByCells& swaths_by_cells,
      bool show_log = false, double d_tol = 1e-4,
      bool redirect_swaths = true) const;

  virtual F2CRoute genRoute(
      const std::vector<F2CMultiLineString>& transit_lanes,
      const F2CCells& hl_area,
      const F2CSwathsByCells& swaths_by_cells,
      bool show_log = false, double d_tol = 1e-4,
      bool redirect_swaths = true) const;

  virtual F2CRoute genRoute(
      const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
      const F2CRobot& robot,
      bool show_log = false, double d_tol = 1e-4,
      bool redirect_swaths = true) const;

  virtual F2CRoute genRoute(
      const F2CMultiLineString& transit_lines,
      const F2CSwathsByCells& swaths_by_cells,
      const F2CRobot& robot,
      bool show_log = false, double d_tol = 1e-4,
      bool redirect_swaths = true) const;

  virtual F2CRoute genRoute(
      const std::vector<F2CMultiLineString>& transit_lanes,
      const F2CCells& hl_area,
      const F2CSwathsByCells& swaths_by_cells,
      const F2CRobot& robot,
      bool show_log = false, double d_tol = 1e-4,
      bool redirect_swaths = true) const;

  virtual std::vector<F2CRoute> genRoute(
      const F2CCells& cells, const std::vector<F2CSwathsByCells>& v_swaths,
      bool show_log = false, double d_tol = 1e-4) const;

  virtual F2CRoute genRoute2(
      const std::vector<F2CMultiLineString>& transit_lanes,
      const F2CCells& hl_area,
      const F2CSwathsByCells& swaths_by_cells,
      const F2CRobot& robot,
      bool show_log = false, double d_tol = 1e-4,
      bool redirect_swaths = true) const;

  virtual F2CRoute genRoute2(
      const F2CMultiLineString& transit_lines,
      const F2CSwathsByCells& swaths_by_cells,
      const F2CRobot& robot,
      bool show_log = false, double d_tol = 1e-4,
      bool redirect_swaths = true) const;

  /// Set the start and the end of the route.
  void setStartAndEndPoint(const F2CPoint& p);
  void setStartPoint(const F2CPoint& p);
  void setEndPoint(const F2CPoint& p);

  /// Create graph to compute the shortest path between two points
  ///   in the headlands.
  ///
  /// @param cells Headland swath rings used to travel through the headlands
  /// @param swaths_by_cells Swaths to be covered.
  /// @param d_tol Tolerance distance to consider if two points are the same.
  virtual F2CGraph2D createShortestGraph(
      const F2CMultiLineString& transit_lines,
      const F2CSwathsByCells& swaths_by_cells,
      double d_tol = 1e-4) const;

  virtual F2CGraph2D createShortestGraph(
      const F2CMultiLineString& transit_lines,
      const F2CSwathsByCells& swaths_by_cells,
      const F2CRobot& robot, double d_tol = 1e-4) const;

  /// Create graph to compute the cost of covering the swaths in a given order.
  ///
  /// @param cells Headland swath rings used to travel through the headlands
  /// @param swaths_by_cells Swaths to be covered.
  /// @param shortest_graph Graph to compute the shortest path
  ///          between two nodes.
  /// @param d_tol Tolerance distance to consider if two points are the same.
  virtual F2CGraph2D createCoverageGraph(
      const F2CSwathsByCells& swaths_by_cells,
      F2CGraph2D& shortest_graph,
      double d_tol = 1e-4, bool redirect_swaths = true) const;

  virtual F2CGraph2D createCoverageGraph(
      const F2CSwathsByCells& swaths_by_cells,
      const F2CRobot& robot,
      F2CGraph2D& shortest_graph,
      double d_tol = 1e-4, bool redirect_swaths = true) const;


  virtual ~RoutePlannerBase() = default;


  std::vector<std::vector<std::vector<int64_t>>> distMatrix(
      const F2CSwathsByCells& swaths_by_cells,
      const F2CRobot& robot,
      F2CGraph2D& cov_graph) const;
  std::vector<int64_t> computeBestRoute(
      const std::vector<std::vector<std::vector<int64_t>>>& demands,
      const F2CSwathsByCells& swaths_by_cells,
      const F2CRobot& robot, bool show_log = false) const;

  std::vector<std::vector<std::vector<int64_t>>> distMatrix2(
      const std::vector<std::vector<std::vector<int64_t>>>& mat,
      size_t n_swaths) const;
  std::vector<int64_t> computeBestRoute2(
      const std::vector<std::vector<std::vector<int64_t>>>& demands,
      const F2CSwathsByCells& swaths_by_cells,
      const F2CRobot& robot, bool show_log = false) const;

  virtual F2CRoute transformSolutionToRoute2(
      const std::vector<int64_t>& route_ids,
      const F2CSwathsByCells& swaths_by_cells,
      const F2CRobot& robot,
      const F2CGraph2D& coverage_graph,
      F2CGraph2D& shortest_graph) const;



 protected:
  /// Use the optimizer to generate the index of the points of the best
  ///   coverage route.
  virtual std::vector<int64_t> computeBestRoute(
      const F2CGraph2D& cov_graph, bool show_log) const;

  virtual std::vector<int64_t> computeBestRoute(
      const F2CGraph2D& cov_graph, const F2CRobot& robot, bool show_log) const;

  /// Tranform index of points to an actual Route.
  virtual F2CRoute transformSolutionToRoute(
      const std::vector<int64_t>& route_ids,
      const F2CSwathsByCells& swaths_by_cells,
      const F2CGraph2D& coverage_graph,
      F2CGraph2D& shortest_graph) const;

 private:
  const operations_research::Assignment* solveWithOrtools(
      operations_research::RoutingModel& routing, bool show_log) const;
  std::vector<int64_t> solToIds(const operations_research::Assignment* sol,
      const operations_research::RoutingIndexManager& manager,
      const operations_research::RoutingModel& routing) const;

 public:
  void setFirstSolStrategy(const OrtoolsFirstSolStrategy& strategy) {
    this->first_strategy = strategy;
  }

  void setLocalSearch(const OrtoolsLocalSearch& search) {
    this->local_search = search;
  }

 public:
  mutable std::vector<int64_t> results;
  mutable std::vector<F2CRoute> first_routes;

 protected:
  std::optional<F2CPoint> r_start;
  std::optional<F2CPoint> r_end;

  OrtoolsFirstSolStrategy first_strategy {OrtoolsFirstSolStrategy::AUTOMATIC};
  OrtoolsLocalSearch local_search {OrtoolsLocalSearch::AUTOMATIC};
  double search_time_limit {10.0};

  size_t p_reload = 20;  // Num of fractions of deposits
  size_t x_reload = 2;  // Num of full deposits

  int64_t INF = 1e15;
};



}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_ROUTE_PLANNING_BASE_H_

