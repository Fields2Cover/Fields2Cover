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
#include <algorithm>
#include <limits>
#include <stdexcept>
#include "fields2cover/route_planning/route_planner_base.h"

namespace f2c::rp {

namespace ortools = operations_research;

F2CRoute RoutePlannerBase::genRoute(
    const F2CCells& cells, const F2CSwathsByCells& swaths,
    bool show_log, double d_tol, bool redirect_swaths,
    long int time_limit_seconds, bool search_for_optimum) {
  return genRouteImpl(cells, swaths, show_log, d_tol, redirect_swaths,
      time_limit_seconds, search_for_optimum);
}

F2CRoute RoutePlannerBase::genRoute(
    const F2CCells& cells, const F2CSwathsByCells& swaths, double d_tol) const {
  return genRouteImpl(cells, swaths, false, d_tol, true, 1, false);
}

F2CRoute RoutePlannerBase::genRouteImpl(
    const F2CCells& cells, const F2CSwathsByCells& swaths,
    bool show_log, double d_tol, bool redirect_swaths,
    long int time_limit_seconds, bool search_for_optimum) const {
  F2CGraph2D shortest_graph = createShortestGraph(cells, swaths, d_tol);

  F2CGraph2D cov_graph = createCoverageGraph(
      cells, swaths, shortest_graph, d_tol, redirect_swaths);

  std::vector<long long int> v_route = computeBestRoute(
      cov_graph, show_log, time_limit_seconds, search_for_optimum);
  return transformSolutionToRoute(
      v_route, swaths, cov_graph, shortest_graph);
}

void RoutePlannerBase::setStartAndEndPoint(const F2CPoint& p) {
  this->r_start_end = p;
}

F2CGraph2D RoutePlannerBase::createShortestGraph(
    const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
    double d_tol) const {
  F2CGraph2D g;
  // Connect each swath end to the border of the cell it belongs to. An end
  // outside every cell is left out: a line to the nearest border would cross
  // whatever lies between.
  auto addBorderEdge = [&g, &cells, d_tol] (const F2CPoint& p) {
    F2CCell cell = cells.getCellWherePoint(p, d_tol);
    if (!cell.isEmpty()) {
      g.addEdge(p, cell.closestPointOnBorderTo(p));
    }
  };
  for (auto&& swaths : swaths_by_cells) {
    for (auto&& s : swaths) {
      addBorderEdge(s.startPoint());
      addBorderEdge(s.endPoint());
    }
  }

  // Add points in the border
  for (auto&& cell : cells) {
    for (auto&& ring : cell) {
      for (size_t i = 0; i < ring.size()-1; ++i) {
        g.addEdge(ring.getGeometry(i), ring.getGeometry(i+1));
      }
    }
  }

  // Add start and end point if they exists. Unlike a swath end, this point
  // is allowed to sit outside the field, so it always gets its border edge.
  if (this->r_start_end) {
    g.addEdge(*r_start_end, cells.closestPointOnBorderTo(*r_start_end));
  }

  std::vector<F2CPoint> nodes = g.getNodes();

  // Connect nodes that are near other edges.
  for (int i = 0; i < 2; ++i) {
    auto edges = g.getEdges();
    for (auto&& edge : edges) {
      size_t from = edge.first;
      for (auto&& e : edge.second) {
        size_t to = e.first;

        F2CLineString border {g.indexToNode(from), g.indexToNode(to)};
        for (auto&& n : nodes) {
          if (n != border[0] && n != border[1] && n.distance(border) < d_tol) {
            g.addEdge(border[0], n);
            g.addEdge(n, border[1]);
            g.removeEdge(border[0], border[1]);
          }
        }
      }
    }
  }
  return g;
}


F2CGraph2D RoutePlannerBase::createCoverageGraph(
    const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
    F2CGraph2D& shortest_graph,
    double d_tol, bool redirect_swaths) const {
  F2CGraph2D g;
  for (auto&& swaths : swaths_by_cells) {
    for (auto&& s : swaths) {
      F2CPoint mid_p {(s.startPoint() + s.endPoint()) * 0.5};
      if (redirect_swaths) {
        g.addEdge(s.startPoint(), mid_p, 0);
        g.addEdge(s.endPoint(), mid_p, 0);
      } else {
        g.addDirectedEdge(s.startPoint(), mid_p, 0);
        g.addDirectedEdge(mid_p, s.endPoint(), 0);
      }
    }
  }

  for (const auto& swaths1 : swaths_by_cells) {
    for (const auto& s1 : swaths1) {
      auto s1_s = s1.startPoint();
      auto s1_e = s1.endPoint();
      for (const auto& swaths2 : swaths_by_cells) {
        for (const auto& s2 : swaths2) {
          auto s2_s = s2.startPoint();
          auto s2_e = s2.endPoint();
          if (redirect_swaths) {
            g.addEdge(s1_s, s2_s, shortest_graph);
            g.addEdge(s1_e, s2_e, shortest_graph);
            g.addEdge(s1_s, s2_e, shortest_graph);
            g.addEdge(s1_e, s2_s, shortest_graph);
          } else {
            g.addDirectedEdge(s1_e, s2_s, shortest_graph);
          }
        }
      }
    }
  }

  F2CPoint deposit(-1e8, -1e8);  // Arbitrary point
  if (this->r_start_end) {
    deposit = *this->r_start_end;
  }

  for (auto&& swaths : swaths_by_cells) {
    for (auto&& s : swaths) {
      if (this->r_start_end) {
        g.addEdge(s.startPoint(), deposit, shortest_graph);
        g.addEdge(s.endPoint(), deposit, shortest_graph);
      } else {
        g.addEdge(s.startPoint(), deposit, 0);
        g.addEdge(s.endPoint(), deposit, 0);
      }
    }
  }
  return g;
}

std::vector<long long int> RoutePlannerBase::computeBestRoute(
    const F2CGraph2D& cov_graph, bool show_log, long int time_limit_seconds,
    bool use_guided_local_search) const {
  const size_t n_nodes = cov_graph.numNodes();
  if (n_nodes < 2) {
    // Without any swath there is nothing to route, and OR-Tools would abort
    // the whole process (CHECK failure on depot index -1) instead of throwing.
    throw std::invalid_argument(
        "RoutePlannerBase: no swaths to route (empty coverage graph)");
  }
  int depot_id = static_cast<int>(n_nodes - 1);
  const ortools::RoutingIndexManager::NodeIndex depot{depot_id};
  ortools::RoutingIndexManager manager(static_cast<int>(n_nodes), 1, depot);
  ortools::RoutingModel routing(manager);

  // The transit callback is evaluated millions of times during local search.
  // Look the costs up in a dense row-major matrix instead of the nested hash
  // maps of the graph (same values: missing edges keep the default INF).
  const size_t n_mat = std::max(cov_graph.matrixSize(), n_nodes);
  std::vector<int64_t> cost_mat(n_mat * n_mat,
      cov_graph.getCostFromEdge(n_mat, n_mat));  // default INF of the graph
  for (const auto& from : cov_graph.getEdges()) {
    for (const auto& to : from.second) {
      cost_mat[from.first * n_mat + to.first] = to.second;
    }
  }
  const int transit_callback_index = routing.RegisterTransitCallback(
      [&cost_mat, &manager, n_mat] (long long int from, long long int to)
      -> long long int {
        const size_t from_node = manager.IndexToNode(from).value();
        const size_t to_node = manager.IndexToNode(to).value();
        return cost_mat[from_node * n_mat + to_node];
      });
  routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);
  ortools::RoutingSearchParameters searchParameters =
    ortools::DefaultRoutingSearchParameters();
  searchParameters.set_use_full_propagation(false);
  searchParameters.set_first_solution_strategy(
    ortools::FirstSolutionStrategy::AUTOMATIC);
  if (use_guided_local_search) {
    searchParameters.set_local_search_metaheuristic(
      ortools::LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH);
  } else {
    searchParameters.set_local_search_metaheuristic(
      ortools::LocalSearchMetaheuristic::AUTOMATIC);
  }
  searchParameters.mutable_time_limit()->set_seconds(time_limit_seconds);
  searchParameters.set_log_search(show_log);
  const ortools::Assignment* solution =
    routing.SolveWithParameters(searchParameters);
  if (solution == nullptr) {
    throw std::runtime_error(
        "RoutePlannerBase: OR-Tools found no route within the time limit");
  }

  long long int index = routing.Start(0);
  std::vector<long long int> v_id;

  index = solution->Value(routing.NextVar(index));

  while (!routing.IsEnd(index)) {
    v_id.emplace_back(manager.IndexToNode(index).value());
    index = solution->Value(routing.NextVar(index));
  }
  return v_id;
}

F2CRoute RoutePlannerBase::transformSolutionToRoute(
    const std::vector<long long int>& route_ids,
    const F2CSwathsByCells& swaths_by_cells,
    const F2CGraph2D& coverage_graph,
    F2CGraph2D& shortest_graph) const {
  F2CRoute route;
  const size_t NS = swaths_by_cells.sizeTotal();
  for (int i = 0; i < route_ids.size()-2; ++i) {
    F2CPoint p_s = coverage_graph.indexToNode(route_ids[i]);
    F2CPoint p_e = coverage_graph.indexToNode(route_ids[i+2]);
    for (int j = 0; j < NS; ++j) {
      F2CSwath swath = swaths_by_cells.getSwath(j).clone();
      if (p_s == swath.startPoint() && p_e == swath.endPoint()) {
        if (route.isEmpty() && r_start_end) {
          route.addConnection(shortest_graph.shortestPath(
                *r_start_end, swath.startPoint()));
        }
        route.addSwath(swath, shortest_graph);
        break;
      } else if (p_e == swath.startPoint() && p_s == swath.endPoint()) {
        swath.reverse();
        if (route.isEmpty() && r_start_end) {
          route.addConnection(shortest_graph.shortestPath(
                *r_start_end, swath.startPoint()));
        }
        route.addSwath(swath, shortest_graph);
        break;
      }
    }
  }
  if (r_start_end) {
    route.addConnection(shortest_graph.shortestPath(
          route.endPoint(), *r_start_end));
  }
  return route;
}

}  // namespace f2c::rp


