//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include <numeric>
#include <limits>
#include <queue>
#include "fields2cover/types/Graph.h"

namespace f2c::types {

Graph& Graph::addDirectedEdge(size_t from, size_t to, int64_t cost) {
  this->edges_[from][to] =  cost;
  if (this->shortest_paths_.size() > 0) {
    this->shortest_paths_.clear();
  }
  return *this;
}

Graph& Graph::addEdge(size_t i, size_t j, int64_t cost) {
  return this->addDirectedEdge(i, j, cost).addDirectedEdge(j, i, cost);
}


Graph& Graph::removeDirectedEdge(size_t from, size_t to) {
  if (this->edges_.count(from) == 0 || this->edges_.at(from).count(to) == 0) {
    return *this;
  }
  this->edges_.at(from).erase(to);
  return *this;
}
Graph& Graph::removeEdge(size_t i, size_t j) {
  return this->removeDirectedEdge(i, j).removeDirectedEdge(j, i);
}

size_t Graph::numNodes() const {
  return this->edges_.size();
}

size_t Graph::numEdges() const {
  return std::accumulate(this->edges_.begin(), this->edges_.end(), 0,
      [](int sum, const auto& e) {return sum + e.second.size();});
}

map_to_map_to_int Graph::getEdges() const {
  return this->edges_;
}

std::vector<size_t> Graph::getEdgesFrom(size_t s) const {
  if (this->edges_.count(s) == 0) {
    return {};
  }
  std::vector<size_t> connections;
  for (auto&& i : this->edges_.at(s)) {
    connections.emplace_back(i.first);
  }
  return connections;
}

int64_t Graph::getCostFromEdge(size_t from, size_t to, int64_t INF) const {
  if (this->edges_.count(from) == 0 || this->edges_.at(from).count(to) == 0) {
    return INF;
  }
  return this->edges_.at(from).at(to);
}

std::vector<std::vector<size_t>> Graph::allPathsBetween(
    size_t from, size_t to) const {
  std::vector<bool> visited(this->numNodes(), false);
  std::vector<std::vector<size_t>> routes(1);
  int route_index = 0;
  this->DFS(from, to, routes, visited, route_index);
  routes.erase(std::remove_if(routes.begin(), routes.end(),
      [&to] (const std::vector<size_t> x) {
          return (x.size() < 1 || x.back() != to);
      }), routes.end());
  return routes;
}

void Graph::DFS(
    size_t from, size_t to,
    std::vector<std::vector<size_t>>& routes,
    std::vector<bool>& visited,
    int& route_index) const {
  routes.emplace_back(routes[route_index]);
  routes.back().emplace_back(from);
  int i_route = routes.size() - 1;
  if (from != to) {
    visited[from] = true;
    for (auto&& i : this->getEdgesFrom(from)) {
      if (!visited[i]) {
        this->DFS(i, to, routes, visited, i_route);
      }
    }
    visited[from] = false;
  }
}

std::vector<std::vector<pair_vec_size__int>>
    Graph::shortestPathsAndCosts(int64_t INF) {
  const size_t N = this->numNodes();
  std::vector<std::vector<int64_t>> dist(N, std::vector<int64_t>(N, INF));
  std::vector<std::vector<int64_t>> next(N, std::vector<int64_t>(N, -1));


  // Initialize distances and paths for direct connections
  for (const auto& src : this->edges_) {
    for (const auto& dest : src.second) {
      dist[src.first][dest.first] = dest.second;
      next[src.first][dest.first] = dest.first;
    }
    dist[src.first][src.first] = 0;
  }


  // Floyd-Warshall
  for (size_t k = 0; k < N; ++k) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        if (dist[i][k] < INF && \
            dist[k][j] < INF && \
            dist[i][j] > dist[i][k] + dist[k][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
          next[i][j] = next[i][k];
        }
      }
    }
  }


  // Reconstruct paths
  std::vector<std::vector<pair_vec_size__int>>
      paths(N, std::vector<pair_vec_size__int>(N));
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      if (i != j && next[i][j] != -1) {
        std::vector<size_t> path = {i};
        size_t current = i;
        while (current != j) {
          current = next[current][j];
          path.push_back(current);
        }
        paths[i][j] = std::make_pair(path, dist[i][j]);
      } else if (i != j && next[i][j] == -1) {
        paths[i][j].second = INF;
      }
    }
  }
  this->shortest_paths_ = std::move(paths);
  return this->shortest_paths_;
}

std::vector<size_t> Graph::shortestPath(size_t from, size_t to, int64_t INF,
    bool using_dijkstra) {
  if (this->shortest_paths_.size() > from &&
      this->shortest_paths_[from].size() > to &&
      this->shortest_paths_[from][to].second < std::numeric_limits<int64_t>::max()) {
    return this->shortest_paths_[from][to].first;
  }
  if (using_dijkstra) {
    if (this->shortest_paths_.empty()) {
      const size_t N = this->numNodes();
      this->shortest_paths_ =
          std::vector<std::vector<std::pair<std::vector<size_t>, int64_t>>>(
              N, std::vector<std::pair<std::vector<size_t>, int64_t>>(
                N, std::pair<std::vector<size_t>, int64_t>(
                  {}, std::numeric_limits<int64_t>::max())));
    }
    auto path = shortestPathDijkstra(from, to, INF);
    this->shortest_paths_[from][to] = path;
    return path.first;
  }

  if (this->numNodes() > 0 && this->shortest_paths_.size() == 0) {
    this->shortestPathsAndCosts(INF);
  }
  return this->shortest_paths_[from][to].first;
}

int64_t Graph::shortestPathCost(size_t from, size_t to, int64_t INF,
    bool using_dijkstra) {
  if (this->shortest_paths_.size() > from &&
      this->shortest_paths_[from].size() > to &&
      this->shortest_paths_[from][to].second < std::numeric_limits<int64_t>::max()) {
    return this->shortest_paths_[from][to].second;
  }
  if (using_dijkstra) {
    if (this->shortest_paths_.empty()) {
      const size_t N = this->numNodes();
      this->shortest_paths_ =
          std::vector<std::vector<std::pair<std::vector<size_t>, int64_t>>>(
              N, std::vector<std::pair<std::vector<size_t>, int64_t>>(
                N, std::pair<std::vector<size_t>, int64_t>(
                  {}, std::numeric_limits<int64_t>::max())));
    }
    auto path = shortestPathDijkstra(from, to, INF);
    this->shortest_paths_[from][to] = path;
    return path.second;
  }

  if (this->numNodes() > 0 && this->shortest_paths_.size() == 0) {
    this->shortestPathsAndCosts(INF);
  }
  return this->shortest_paths_[from][to].second;
}


std::pair<std::vector<size_t>, int64_t> Graph::shortestPathDijkstra(
    size_t from, size_t to, int64_t INF) const {
  std::priority_queue<std::pair<int64_t, size_t>,
      std::vector<std::pair<int64_t, size_t>>, std::greater<>> pq;
  std::unordered_map<size_t, int64_t> dists;
  std::unordered_map<size_t, size_t> prev_node;

  for (const auto& pair : this->edges_) {
    dists[pair.first] = std::numeric_limits<int64_t>::max();
  }

  dists[from] = 0;
  pq.emplace(0, from);

  while (!pq.empty()) {
    auto [current_dist, current_node] = pq.top();
    pq.pop();

    if (current_node == to) {
      break;
    }

    if (current_dist > dists[current_node]) {
      continue;
    }

    for (const auto& [neighbor, cost] : edges_.at(current_node)) {
      int64_t new_dist = current_dist + cost;
      if (new_dist < dists[neighbor]) {
        dists[neighbor] = new_dist;
        prev_node[neighbor] = current_node;
        pq.emplace(new_dist, neighbor);
      }
    }
  }

  std::vector<size_t> path;
  if (dists[to] == std::numeric_limits<int64_t>::max()) {
    return {path, INF};
  }

  for (size_t at = to; at != from; at = prev_node[at]) {
    path.push_back(at);
  }
  path.push_back(from);

  std::reverse(path.begin(), path.end());
  return {path, dists[to]};
}





}  // namespace f2c::types


