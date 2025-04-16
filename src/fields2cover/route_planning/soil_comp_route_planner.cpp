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
#include "fields2cover/route_planning/soil_comp_route_planner.h"


namespace f2c::rp {

namespace ortools = operations_research;

F2CSwathsByCells flatten(const std::vector<F2CSwathsByCells>& v_swaths) {
  F2CSwaths flatten_swaths;
  for (auto&& s : v_swaths) {
    flatten_swaths.append(s.flatten());
  }
  return F2CSwathsByCells({flatten_swaths});
}

std::vector<F2CRoute> SoilCompRoutePlanner::genRoute(
    const F2CCells& cells, const std::vector<F2CSwathsByCells>& v_swaths,
    bool show_log, double d_tol) const {
  std::vector<F2CRoute> routes(v_swaths.size());
  std::vector<F2CRoute> best_routes(v_swaths.size());
  int64_t first_cost = 1e15;
  int64_t best_cost = 1e15;
  int64_t first_length = 1e15;
  int64_t best_length = 1e15;

  auto cells_lines = cells.getLineSections();

  F2CGraph2D shortest_graph = createShortestGraph(
      cells_lines, flatten(v_swaths), d_tol);


  int max_i {0};
  int count {1};
  for (int i = 0; i < 20; ++i) {
    max_i = i;
    auto s_graph = updateCosts(shortest_graph, routes);
    for (int s = 0; s < v_swaths.size(); ++s) {
      F2CGraph2D cov_graph = createCoverageGraph(
          v_swaths[s], s_graph, d_tol);
      std::vector<int64_t> v_route = computeBestRoute(cov_graph, show_log);
      routes[s] = transformSolutionToRoute(
          v_route, v_swaths[s], cov_graph, shortest_graph);
    }
    double length {0.0};
    for (auto&& r : routes) {
      length += r.length();
    }
    length *= 1e3;

    auto soil_comp_cost = computeSoilCompaction(shortest_graph, routes);

    if (i == 0) {
      first_cost = soil_comp_cost;
      first_length = length;
      first_routes = routes;
    }

    if (soil_comp_cost < best_cost or
       (soil_comp_cost == best_cost and length < best_length)) {
      best_routes = routes;
      best_cost = soil_comp_cost;
      best_length = length;
      count = 0;
    } else {
      ++count;
      if (count > 2) {
        break;
      }
    }

    if (i > 2) {
      std::cerr << "Path it:" << i << ", length: " << length<<" soil: " << best_cost<<" =?= "<< soil_comp_cost<<"  lenghts by route:  ";
      for (auto&& r : routes) {
        std::cerr<<"  "<< r.length();
      }
      std::cerr<<std::endl;
    }
  }

  results.clear();
  results.emplace_back(max_i);
  results.emplace_back(first_cost);
  results.emplace_back(best_cost);
  results.emplace_back(first_length);
  results.emplace_back(best_length);

  return best_routes;
}

F2CRoute SoilCompRoutePlanner::genRoute(
    const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
    bool show_log, double d_tol, bool redirect_swaths) const {
  return genRoute(cells, std::vector<F2CSwathsByCells>({swaths_by_cells}),
      show_log, d_tol).at(0);
}




F2CGraph2D SoilCompRoutePlanner::updateCosts(
    const F2CGraph2D& shortest_graph, const F2CRoute& route) const {
  return updateCosts(shortest_graph, std::vector<F2CRoute>({route}));
}

F2CGraph2D SoilCompRoutePlanner::updateCosts(
    const F2CGraph2D& shortest_graph,
    const std::vector<F2CRoute>& routes) const {
  const size_t N = shortest_graph.numNodes();
  std::vector<std::vector<size_t>> passes(N, std::vector<size_t>(N, 0));
  for (auto&& route : routes) {
    for (auto&& c : route.getConnections()) {
      if (!c.isEmpty()) {
        for (size_t i = 0; i < c.size() - 1; ++i) {
          size_t ci  = shortest_graph.nodeToIndex(c[i]);
          size_t ci1 = shortest_graph.nodeToIndex(c[i+1]);
          ++passes[ci][ci1];
          ++passes[ci1][ci];
        }
      }
    }
  }
  F2CGraph2D s_graph;
  for (auto&& edges : shortest_graph.getEdges()) {
    for (auto&& e : edges.second) {
      s_graph.addEdge(
          shortest_graph.indexToNode(edges.first),
          shortest_graph.indexToNode(e.first),
          e.second * soil_costs[std::min(
              passes[edges.first][e.first], soil_costs.size()-1)]);
    }
  }
  return s_graph;
}


int64_t SoilCompRoutePlanner::computeSoilCompaction(
    const F2CGraph2D& shortest_graph,
    const std::vector<F2CRoute>& routes) const {
  const size_t N = shortest_graph.numNodes();
  std::vector<std::vector<size_t>> passes(N, std::vector<size_t>(N, 0));
  for (auto&& route : routes) {
    for (auto&& c : route.getConnections()) {
      if (!c.isEmpty()) {
        for (size_t i = 0; i < c.size() - 1; ++i) {
          size_t ci  = shortest_graph.nodeToIndex(c[i]);
          size_t ci1 = shortest_graph.nodeToIndex(c[i+1]);
          ++passes[ci][ci1];
          ++passes[ci1][ci];
        }
      }
    }
  }
  F2CGraph2D s_graph;
  int64_t cost {0};
  for (auto&& edges : shortest_graph.getEdges()) {
    for (auto&& e : edges.second) {
      for (size_t i = 0; i < passes[edges.first][e.first]; ++i) {
        cost += e.second * soil_costs[std::min(
              i, soil_costs.size()-1)];
      }
    }
  }
  return cost;
}

}  // namespace f2c::rp


