//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <ortools/constraint_solver/routing.h>
#include <ortools/constraint_solver/routing_enums.pb.h>
#include <ortools/constraint_solver/routing_index_manager.h>
#include <ortools/constraint_solver/routing_parameters.h>
#include <math.h>
#include <utility>
#include <vector>
#include <limits>
#include "fields2cover/route_planning/route_planner_base.h"

namespace f2c::rp {

F2CRoute RoutePlannerBase::genRoute(
    const F2CMultiLineString& transit_lanes, const F2CSwathsByCells& swaths,
    bool show_log, double d_tol, bool redirect_swaths) const {
  F2CGraph2D shortest_graph = createShortestGraph(transit_lanes, swaths, d_tol);
  F2CGraph2D cov_graph = createCoverageGraph(
      swaths, shortest_graph, d_tol, redirect_swaths);

  std::vector<int64_t> v_route = computeBestRoute(cov_graph, show_log);
  return transformSolutionToRoute(
      v_route, swaths, cov_graph, shortest_graph);
}

std::vector<F2CRoute> RoutePlannerBase::genRoute(
    const F2CCells& cells, const std::vector<F2CSwathsByCells>& v_swaths,
    bool show_log, double d_tol) const {
  std::vector<F2CRoute> routes;
  for (auto&& s : v_swaths) {
    routes.emplace_back(genRoute(cells, s, show_log, d_tol));
  }
  return routes;
}


F2CRoute RoutePlannerBase::genRoute(
    const std::vector<F2CMultiLineString>& transit_lanes,
    const F2CSwathsByCells& swaths,
    bool show_log, double d_tol, bool redirect_swaths) const {
  F2CMultiLineString lines;
  for (auto&& line : transit_lanes) {
    lines.append(line);
  }
  return this->genRoute(lines, swaths, show_log, d_tol, redirect_swaths);
}

F2CRoute RoutePlannerBase::genRoute(
    const std::vector<F2CMultiLineString>& transit_lanes,
    const F2CCells& hl_area,
    const F2CSwathsByCells& swaths,
    bool show_log, double d_tol, bool redirect_swaths) const {
  return this->genRoute(
      F2CMultiLineString::connectMultiLineStrings(transit_lanes, hl_area),
      swaths, show_log, d_tol, redirect_swaths);
}

F2CRoute RoutePlannerBase::genRoute(
    const F2CCells& cells, const F2CSwathsByCells& swaths,
    bool show_log, double d_tol, bool redirect_swaths) const {
  return this->genRoute(cells.getLineSections(), swaths,
      show_log, d_tol, redirect_swaths);
}

F2CRoute RoutePlannerBase::genRoute(
    const F2CCells& cells, const F2CSwathsByCells& swaths,
    const F2CRobot& robot,
    bool show_log, double d_tol, bool redirect_swaths) const {
  return this->genRoute(cells.getLineSections(), swaths,
      show_log, d_tol, redirect_swaths);
}

F2CRoute RoutePlannerBase::genRoute(
    const F2CMultiLineString& transit_lines, const F2CSwathsByCells& swaths,
    const F2CRobot& robot,
    bool show_log, double d_tol, bool redirect_swaths) const {
  if (robot.getNumCapacities() == 0) {
    return genRoute(transit_lines, swaths, show_log, d_tol, redirect_swaths);
  }

  F2CGraph2D shortest_graph = createShortestGraph(
      transit_lines, swaths, robot, d_tol);
  F2CGraph2D cov_graph = createCoverageGraph(
      swaths, robot, shortest_graph, d_tol, redirect_swaths);

  std::vector<int64_t> v_route = computeBestRoute(cov_graph, robot, show_log);
  return transformSolutionToRoute(
      v_route, swaths, cov_graph, shortest_graph);
}

F2CRoute RoutePlannerBase::genRoute(
    const std::vector<F2CMultiLineString>& transit_lanes,
    const F2CCells& hl_area,
    const F2CSwathsByCells& swaths,
    const F2CRobot& robot,
    bool show_log, double d_tol, bool redirect_swaths) const {
  return this->genRoute(
      F2CMultiLineString::connectMultiLineStrings(transit_lanes, hl_area),
      swaths, robot,
      show_log, d_tol, redirect_swaths);
}

void RoutePlannerBase::setStartPoint(const F2CPoint& p) {
  this->r_start = p;
}

void RoutePlannerBase::setEndPoint(const F2CPoint& p) {
  this->r_end = p;
}

void RoutePlannerBase::setStartAndEndPoint(const F2CPoint& p) {
  this->r_start = p;
  this->r_end = p;
}

F2CGraph2D RoutePlannerBase::createShortestGraph(
    const F2CMultiLineString& transit_lines,
    const F2CSwathsByCells& swaths_by_cells,
    double d_tol) const {
  F2CGraph2D g;
  return g.addEdges(transit_lines)
          .addEdgesSwathExtremes(swaths_by_cells, transit_lines)
          .addEdge(this->r_start, transit_lines)
          .addEdge(this->r_end, transit_lines)
          .reconnectEdges(d_tol);
}

F2CGraph2D RoutePlannerBase::createShortestGraph(
    const F2CMultiLineString& transit_lines,
    const F2CSwathsByCells& swaths_by_cells,
    const F2CRobot& robot,
    double d_tol) const {
  return createShortestGraph(transit_lines, swaths_by_cells, d_tol)
      .addEdges(robot.getReloadPoints(), transit_lines)
      .reconnectEdges(d_tol);
}

F2CGraph2D RoutePlannerBase::createCoverageGraph(
    const F2CSwathsByCells& swaths_by_cells,
    F2CGraph2D& shortest_graph,
    double d_tol, bool redirect_swaths) const {
  F2CGraph2D g;
  F2CPoint deposit(-1e8, -1e8);  // Arbitrary point
  g.addDirectedEdge(deposit, deposit);

  if (redirect_swaths) {
    g.addEdges(swaths_by_cells);
    g.addEdges(swaths_by_cells, swaths_by_cells, shortest_graph);
  } else {
    g.addDirectedEdges(swaths_by_cells);
    g.addDirectedEdges(swaths_by_cells, swaths_by_cells, shortest_graph);
  }

  if (this->r_start) {
    g.addDirectedEdge(deposit, *this->r_start, 0);
    g.addDirectedEdges(*this->r_start, swaths_by_cells, shortest_graph);
  } else {
    g.addDirectedEdges(deposit, swaths_by_cells, 0);
  }
  if (this->r_end) {
    g.addDirectedEdges(swaths_by_cells, *this->r_end, shortest_graph);
    g.addDirectedEdge(*this->r_end, deposit, 0);
  } else {
    g.addDirectedEdges(swaths_by_cells, deposit, 0);
  }

  return g;
}

F2CGraph2D RoutePlannerBase::createCoverageGraph(
    const F2CSwathsByCells& swaths_by_cells,
    const F2CRobot& robot,
    F2CGraph2D& shortest_graph,
    double d_tol, bool redirect_swaths) const {
  F2CGraph2D g {createCoverageGraph(swaths_by_cells,
      shortest_graph, d_tol, redirect_swaths)};
  for (auto&& r : robot.getReloadPoints()) {
    g.addEdges(r, swaths_by_cells, shortest_graph);
    for (auto&& r2 : robot.getReloadPoints()) {
      g.addEdge(r, r2, shortest_graph);
    }
  }
  return g;
}

const operations_research::Assignment* RoutePlannerBase::solveWithOrtools(
    operations_research::RoutingModel& routing, bool show_log) const {
  operations_research::RoutingSearchParameters searchParameters =
    operations_research::DefaultRoutingSearchParameters();
  searchParameters.set_use_full_propagation(false);
  searchParameters.set_first_solution_strategy(
      static_cast<operations_research::FirstSolutionStrategy_Value>(
        static_cast<size_t>(this->first_strategy)));
  searchParameters.set_local_search_metaheuristic(
      static_cast<operations_research::LocalSearchMetaheuristic_Value>(
        static_cast<size_t>(this->local_search)));
  searchParameters.mutable_time_limit()->set_seconds(this->search_time_limit);
  searchParameters.set_log_search(show_log);
  return routing.SolveWithParameters(searchParameters);
}

std::vector<int64_t> RoutePlannerBase::solToIds(const operations_research::Assignment* sol,
    const operations_research::RoutingIndexManager& manager,
    const operations_research::RoutingModel& routing) const {
  std::vector<int64_t> v_id;
  int64_t index = routing.Start(0);

  index = sol->Value(routing.NextVar(index));
  while (!routing.IsEnd(index)) {
    v_id.emplace_back(manager.IndexToNode(index).value());
    index = sol->Value(routing.NextVar(index));
  }
  return v_id;
}


std::vector<int64_t> RoutePlannerBase::computeBestRoute(
    const F2CGraph2D& cov_graph, bool show_log) const {
  int depot_id = static_cast<int>(0);
  const operations_research::RoutingIndexManager::NodeIndex depot{depot_id};
  operations_research::RoutingIndexManager manager(cov_graph.numNodes(), 1, depot);
  operations_research::RoutingModel routing(manager);

  const int transit_callback_index = routing.RegisterTransitCallback(
      [&cov_graph, &manager] (int64_t from, int64_t to) -> int64_t {
        auto from_node = manager.IndexToNode(from).value();
        auto to_node = manager.IndexToNode(to).value();
        return cov_graph.getCostFromEdge(from_node, to_node);
      });
  routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);

  return solToIds(solveWithOrtools(routing, show_log), manager, routing);
}


std::vector<int64_t> RoutePlannerBase::computeBestRoute(
    const F2CGraph2D& cov_graph, const F2CRobot& robot, bool show_log) const {
  return computeBestRoute(cov_graph, show_log);
}

F2CRoute RoutePlannerBase::transformSolutionToRoute(
    const std::vector<int64_t>& route_ids,
    const F2CSwathsByCells& swaths_by_cells,
    const F2CGraph2D& coverage_graph,
    F2CGraph2D& shortest_graph) const {
  F2CRoute route;
  F2CSwath swath;
  const size_t NS = swaths_by_cells.sizeTotal();

  for (int i = 0; i < route_ids.size()-2; ++i) {
    F2CPoint p_s = coverage_graph.indexToNode(route_ids[i]);
    F2CPoint p_m = coverage_graph.indexToNode(route_ids[i+1]);
    F2CPoint p_e = coverage_graph.indexToNode(route_ids[i+2]);
    for (int j = 0; j < NS; ++j) {
      F2CSwath swath = swaths_by_cells.getSwath(j).clone();
      F2CPoint s_s = swath.startPoint();
      F2CPoint s_e = swath.endPoint();
      F2CPoint s_m = (s_s + s_e) * 0.5;

      if (p_e == s_s && p_s == s_e) {
        swath.reverse();
        s_s = swath.startPoint();
        s_e = swath.endPoint();
      }
      if (p_s == s_s && p_e == s_e && p_m == s_m) {
        if (route.isEmpty() && this->r_start && *this->r_start != s_s) {
          route.addConnection(shortest_graph.shortestPath(*this->r_start, s_s));
        }
        route.addSwath(swath, shortest_graph);
        break;
      }
    }
  }
  if (this->r_end) {
    route.addConnection(shortest_graph.shortestPath(route.endPoint(), *r_end));
  }
  return route;
}


std::vector<std::vector<std::vector<int64_t>>> RoutePlannerBase::distMatrix(
    const F2CSwathsByCells& swaths_by_cells,
    const F2CRobot& robot,
    F2CGraph2D& cov_graph) const {
  std::vector<F2CPoint> reload_points = robot.getReloadPoints();
  const size_t n_reload = reload_points.size();

  size_t n_swaths_ids = 3 * swaths_by_cells.sizeTotal();
  size_t n_reloads_ids = this->p_reload * n_reload;

  size_t id_swaths_0 = 2;  // start + end
  size_t id_reloads_0 = id_swaths_0 + n_swaths_ids;

  size_t N = id_reloads_0 + n_reloads_ids;

  std::vector<std::vector<int64_t>> dist(N, std::vector<int64_t>(N, INF));
  std::vector<std::vector<std::vector<int64_t>>> demands(
      robot.getNumCapacities(), std::vector<std::vector<int64_t>>(
          N, std::vector<int64_t>(N, 0)));

  auto capacities = robot.getCapacities();

  dist[1][0] = 0;  // end -> start

  // Connection between reload points

  for (size_t i = 0; i < n_reload; ++i) {
    for (size_t j = 0; j < n_reload; ++j) {
      int64_t D = cov_graph.getCostFromEdge(reload_points[i], reload_points[j]);
      for (size_t ki = 0; ki < p_reload; ++ki) {
        for (size_t kj = 0; kj < p_reload; ++kj) {
          size_t id_i = id_reloads_0 + i * this->p_reload + ki;
          size_t id_j = id_reloads_0 + j * this->p_reload + kj;
          dist[id_i][id_j] = D;
        }
      }
    }
  }



  for (size_t i = 0; i < swaths_by_cells.sizeTotal(); ++i) {
    size_t s_i = id_swaths_0 + i * 3;
    F2CSwath swath_i = swaths_by_cells.getSwath(i);

    F2CPoint si_start = swath_i.startPoint();
    F2CPoint si_end = swath_i.endPoint();
    F2CPoint si_mid = (si_start + si_end) * 0.5;

    // Start and end connections with swaths
    dist[0][s_i + 0] = r_start ?
      cov_graph.getCostFromEdge(r_start, si_start) : 0;
    dist[0][s_i + 2] = r_start ?
      cov_graph.getCostFromEdge(r_start, si_end) : 0;
    dist[s_i + 0][1] = r_end ?
      cov_graph.getCostFromEdge(si_start, r_end) : 0;
    dist[s_i + 2][1] = r_end ?
      cov_graph.getCostFromEdge(si_end, r_end) : 0;


    // Swath points from middle to extremes
    dist[s_i + 0][s_i + 1] = cov_graph.getCostFromEdge(si_start, si_mid);
    dist[s_i + 1][s_i + 0] = dist[s_i + 0][s_i + 1];
    dist[s_i + 1][s_i + 2] = dist[s_i + 0][s_i + 1];
    dist[s_i + 2][s_i + 1] = dist[s_i + 0][s_i + 1];

    for (size_t c = 0; c < capacities.size(); ++c) {
      demands[c][s_i + 0][s_i + 1] =
        dist[s_i + 0][s_i + 1] * capacities[c].rate;
      demands[c][s_i + 1][s_i + 0] =
        dist[s_i + 1][s_i + 0] * capacities[c].rate;
      demands[c][s_i + 1][s_i + 2] =
        dist[s_i + 1][s_i + 2] * capacities[c].rate;
      demands[c][s_i + 2][s_i + 1] =
        dist[s_i + 2][s_i + 1] * capacities[c].rate;
    }

    // Connection between near swaths
    for (size_t j = 0; j < swaths_by_cells.sizeTotal(); ++j) {
      size_t s_j = id_swaths_0 + j * 3;
      F2CSwath swath_j = swaths_by_cells.getSwath(j);

      F2CPoint sj_start = swath_j.startPoint();
      F2CPoint sj_end = swath_j.endPoint();

      dist[s_i + 0][s_j + 0] = cov_graph.getCostFromEdge(si_start, sj_start);
      dist[s_j + 0][s_i + 0] = dist[s_i + 0][s_j + 0];

      dist[s_i + 2][s_j + 0] = cov_graph.getCostFromEdge(si_end, sj_start);
      dist[s_j + 0][s_i + 2] = dist[s_i + 2][s_j + 0];

      dist[s_i + 0][s_j + 2] = cov_graph.getCostFromEdge(si_start, sj_end);
      dist[s_j + 2][s_i + 0] = dist[s_i + 0][s_j + 2];

      dist[s_i + 2][s_j + 2] = cov_graph.getCostFromEdge(si_end, sj_end);
      dist[s_j + 2][s_i + 2] = dist[s_i + 2][s_j + 2];

      for (size_t c = 0; c < capacities.size(); ++c) {
        if (capacities[c].type == f2c::types::RobotCapacityType::BY_LENGTH) {
          demands[c][s_i + 0][s_j + 0] =
            dist[s_i + 0][s_j + 0] * capacities[c].rate;
          demands[c][s_j + 0][s_i + 0] =
            dist[s_j + 0][s_i + 0] * capacities[c].rate;

          demands[c][s_i + 2][s_j + 0] =
            dist[s_i + 2][s_j + 0] * capacities[c].rate;
          demands[c][s_j + 0][s_i + 2] =
            dist[s_j + 0][s_i + 2] * capacities[c].rate;

          demands[c][s_i + 0][s_j + 2] =
            dist[s_i + 0][s_j + 2] * capacities[c].rate;
          demands[c][s_j + 2][s_i + 0] =
            dist[s_j + 2][s_i + 0] * capacities[c].rate;

          demands[c][s_i + 2][s_j + 2] =
            dist[s_i + 2][s_j + 2] * capacities[c].rate;
          demands[c][s_j + 2][s_i + 2] =
            dist[s_j + 2][s_i + 2] * capacities[c].rate;
        }
      }
    }

    // Connection between swath and reload points
    for (size_t j = 0; j < n_reload; ++j) {
      size_t r_j = id_reloads_0 + j * p_reload;

      int64_t dist_reload_s0 = cov_graph.getCostFromEdge(
          si_start, reload_points[j]);
      int64_t dist_reload_s2 = cov_graph.getCostFromEdge(
          si_end, reload_points[j]);

      for (size_t k = 0; k < p_reload; ++k) {
        dist[s_i + 0][r_j + k] = dist_reload_s0;
        dist[r_j + k][s_i + 0] = dist_reload_s0;

        dist[s_i + 2][r_j + k] = dist_reload_s2;
        dist[r_j + k][s_i + 2] = dist_reload_s2;

        for (size_t c = 0; c < capacities.size(); ++c) {
          if (capacities[c].type == f2c::types::RobotCapacityType::BY_LENGTH) {
            demands[c][s_i + 0][r_j + k] =
              dist[s_i + 0][r_j + k] * capacities[c].rate;
            demands[c][r_j + k][s_i + 0] =
              dist[r_j + k][s_i + 0] * capacities[c].rate;
            demands[c][s_i + 2][r_j + k] =
              dist[s_i + 2][r_j + k] * capacities[c].rate;
            demands[c][r_j + k][s_i + 2] =
              dist[r_j + k][s_i + 2] * capacities[c].rate;
          }
        }
      }
    }
  }

  // Reload points
  size_t c_r {0};
  for (size_t c = 0; c < capacities.size(); ++c) {
    for (size_t r = 0; r < capacities[c].reload_points.size(); ++r) {
      for (size_t p = 1; p <= p_reload; ++p) {
        for (size_t n = 0; n < N; ++n) {
          if (n != id_reloads_0 + c_r) {
            demands[c][n][id_reloads_0 + c_r] -=
              p * capacities[c].max_capacity / p_reload;
          }
        }
      ++c_r;
      }
    }
  }


  std::vector<std::vector<std::vector<int64_t>>> res;
  res.emplace_back(dist);
  for (size_t i = 0; i < demands.size(); ++i) {
    res.emplace_back(demands[i]);
  }
  return res;
}


std::vector<int64_t> RoutePlannerBase::computeBestRoute(
    const std::vector<std::vector<std::vector<int64_t>>>& demands,
    const F2CSwathsByCells& swaths,
    const F2CRobot& robot, bool show_log) const {
  int depot_id = static_cast<int>(0);
  const operations_research::RoutingIndexManager::NodeIndex depot{depot_id};
  operations_research::RoutingIndexManager manager(demands[0].size(), 1, depot);
  operations_research::RoutingModel routing(manager);

  std::vector<int> v_transit_callbacks;
  for (size_t i = 0; i < demands.size(); ++i) {
    v_transit_callbacks.emplace_back(routing.RegisterTransitCallback(
      [&demands, &manager, i] (int64_t from, int64_t to) -> int64_t {
        auto from_node = manager.IndexToNode(from).value();
        auto to_node = manager.IndexToNode(to).value();
        return demands[i][from_node][to_node];
      }));
  }
  routing.SetArcCostEvaluatorOfAllVehicles(v_transit_callbacks[0]);

  for (size_t i = 0; i < robot.getNumCapacities(); ++i) {
    routing.AddDimensionWithVehicleCapacity(
      v_transit_callbacks[i+1],    // transit callback index
      int64_t{0},               // null capacity slack
      {robot.getCapacity(i).max_capacity},  // vehicle maximum capacities
      true,                     // start cumul to zero
      robot.getCapacity(i).name);
  }
  for (int64_t i = 2 + 3 * swaths.sizeTotal(); i < demands[0].size(); ++i) {
    routing.AddDisjunction(
        {manager.NodeToIndex(operations_research::RoutingIndexManager::NodeIndex(i))}, 0);
  }

  return solToIds(solveWithOrtools(routing, show_log), manager, routing);
}


std::vector<std::vector<std::vector<int64_t>>> RoutePlannerBase::distMatrix2(
    const std::vector<std::vector<std::vector<int64_t>>>& mat,
    size_t n_swaths) const {
  const size_t N = mat[0].size() - n_swaths;
  std::vector<std::vector<std::vector<int64_t>>> res(
      mat.size(), std::vector<std::vector<int64_t>>(
        N, std::vector<int64_t>(N)));

  // Clean dist dimension matrix
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      res[0][i][j] = INF;
    }
  }
  res[0][1][0] = 0;
  for (size_t d = 0; d < mat.size(); ++d) {
    for (size_t i = 0; i < n_swaths; ++i) {
      res[d][0][2+2*i] = mat[d][0][2+3*i];
      res[d][0][2+2*i+1] = mat[d][0][2+3*i+2];
      int64_t mi_s = mat[d][2+3*i][2+3*i+1] + mat[d][2+3*i+1][2+3*i+2];
      int64_t mi_r = mat[d][2+3*i+2][2+3*i+1] + mat[d][2+3*i+1][2+3*i];
      res[d][2+2*i][1] = mi_s + mat[d][2+3*i+2][1];
      res[d][2+2*i+1][1] = mi_r + mat[d][2+3*i][1];

      for (size_t j = 0; j < n_swaths; ++j) {
        res[d][2+2*i][2+2*j] = mi_s + mat[d][2+3*i+2][2+3*j];
        res[d][2+2*i][2+2*j+1] = mi_s + mat[d][2+3*i+2][2+3*j+2];
        res[d][2+2*i+1][2+2*j] = mi_r + mat[d][2+3*i][2+3*j];
        res[d][2+2*i+1][2+2*j+1] = mi_r + mat[d][2+3*i][2+3*j+2];
      }
      for (size_t j = 2 + 2 * n_swaths; j < N; ++j) {
        res[d][2+2*i][j] = mi_s + mat[d][2+3*i+2][j + n_swaths];
        res[d][2+2*i+1][j] = mi_s + mat[d][2+3*i][j + n_swaths];
        res[d][j][2+2*i] = mat[d][j + n_swaths][2+3*i];
        res[d][j][2+2*i+1] = mat[d][j + n_swaths][2+3*i+2];
      }
    }
    for (size_t i = 2 + 2 * n_swaths; i < N; ++i) {
      for (size_t j = 2 + 2 * n_swaths; j < N; ++j) {
        res[d][i][j] = mat[d][i+n_swaths][j+n_swaths];
      }
    }
  }
  return res;
}


std::vector<int64_t> RoutePlannerBase::computeBestRoute2(
    const std::vector<std::vector<std::vector<int64_t>>>& demands,
    const F2CSwathsByCells& swaths,
    const F2CRobot& robot, bool show_log) const {
  int depot_id = static_cast<int>(0);
  const operations_research::RoutingIndexManager::NodeIndex depot{depot_id};
  operations_research::RoutingIndexManager manager(demands[0].size(), 1, depot);
  operations_research::RoutingModel routing(manager);

  std::vector<int> v_transit_callbacks;
  for (size_t i = 0; i < demands.size(); ++i) {
    v_transit_callbacks.emplace_back(routing.RegisterTransitCallback(
      [&demands, &manager, i] (int64_t from, int64_t to) -> int64_t {
        auto from_node = manager.IndexToNode(from).value();
        auto to_node = manager.IndexToNode(to).value();
        return demands[i][from_node][to_node];
      }));
  }
  routing.SetArcCostEvaluatorOfAllVehicles(v_transit_callbacks[0]);

  for (size_t i = 0; i < robot.getNumCapacities(); ++i) {
    routing.AddDimensionWithVehicleCapacity(
      v_transit_callbacks[i+1],    // transit callback index
      int64_t{0},               // null capacity slack
      {robot.getCapacity(i).max_capacity},  // vehicle maximum capacities
      true,                     // start cumul to zero
      robot.getCapacity(i).name);
  }
  for (int64_t i = 0; i < swaths.sizeTotal(); ++i) {
    routing.AddDisjunction({
        manager.NodeToIndex(operations_research::RoutingIndexManager::NodeIndex(2+2*i)),
        manager.NodeToIndex(operations_research::RoutingIndexManager::NodeIndex(2+2*i+1))
        }, -1);
  }
  for (int64_t i = 2 + 2 * swaths.sizeTotal(); i < demands[0].size(); ++i) {
    routing.AddDisjunction(
        {manager.NodeToIndex(operations_research::RoutingIndexManager::NodeIndex(i))}, 0);
  }
  return solToIds(solveWithOrtools(routing, show_log), manager, routing);
}

F2CRoute RoutePlannerBase::transformSolutionToRoute2(
    const std::vector<int64_t>& route_ids,
    const F2CSwathsByCells& swaths_by_cells,
    const F2CRobot& robot,
    const F2CGraph2D& coverage_graph,
    F2CGraph2D& shortest_graph) const {
  F2CRoute route;
  for (int i = 0; i < route_ids.size() - 1; ++i) {
    if (route_ids[i] < 2) {
      continue;
    }
    if (route_ids[i] >= 2 + 2 * swaths_by_cells.sizeTotal()) {
      // TODO: Add recharge points
      // TODO: FIX what happen when there are more than 1 reload point
      int64_t reload_id = (route_ids[i] - (2 + 2 * swaths_by_cells.sizeTotal())) / this->p_reload;
      route.addConnection(shortest_graph.shortestPath(route.endPoint(), robot.getCapacity(reload_id).reload_points[0]));
      continue;
    }
    F2CSwath swath = swaths_by_cells.getSwath((route_ids[i] - 2)/2).clone();
    if (route_ids[i] % 2 == 1) {
      swath.reverse();
    }
    if (route.isEmpty() && this->r_start) {
      route.addConnection(
          shortest_graph.shortestPath(*this->r_start, swath.startPoint()));
    }
    route.addSwath(swath, shortest_graph);
  }
  if (this->r_end) {
    route.addConnection(shortest_graph.shortestPath(route.endPoint(), *r_end));
  }
  return route;
}

F2CRoute RoutePlannerBase::genRoute2(
    const F2CMultiLineString& transit_lines, const F2CSwathsByCells& swaths,
    const F2CRobot& robot,
    bool show_log, double d_tol, bool redirect_swaths) const {
  F2CGraph2D shortest_graph = createShortestGraph(
      transit_lines, swaths, robot, d_tol);
  F2CGraph2D cov_graph = createCoverageGraph(
      swaths, robot, shortest_graph, d_tol, redirect_swaths);
  auto dist_prev = distMatrix(swaths, robot, cov_graph);
  auto dist = distMatrix2(dist_prev, swaths.sizeTotal());
  auto ids = computeBestRoute2(dist, swaths, robot, show_log);
  return transformSolutionToRoute2(ids, swaths, robot, cov_graph, shortest_graph);
}

F2CRoute RoutePlannerBase::genRoute2(
    const std::vector<F2CMultiLineString>& transit_lanes,
    const F2CCells& hl_area,
    const F2CSwathsByCells& swaths,
    const F2CRobot& robot,
    bool show_log, double d_tol, bool redirect_swaths) const {
  return this->genRoute2(
      F2CMultiLineString::connectMultiLineStrings(transit_lanes, hl_area),
      swaths, robot,
      show_log, d_tol, redirect_swaths);
}


}  // namespace f2c::rp


