//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <numeric>
#include <algorithm>
#include "fields2cover/types/Graph.h"

#include <iostream>
#include <ostream>

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

void Graph::shortestPathsAndCosts(int64_t INF) {
  const bool single_pass = !two_pass_flag_;

  const bool fist_pass = (two_pass_flag_ &&  pass_counter_ == 1);
  const bool second_pass = (two_pass_flag_ &&  pass_counter_ == 2);

  if ((single_pass && pass_counter_ > 1)|| (two_pass_flag_ && pass_counter_ > 2)) {
    std::cout << "enough passes" << std::endl;
    return;
  }


  const size_t N = this->numNodes();
  std::vector<std::vector<int64_t>> dist; //(N, std::vector<int64_t>(N, INF));
  std::vector<std::vector<int64_t>> next(N, std::vector<int64_t>(N, -1));

  if (single_pass && pass_counter_ ==1) {
    std::cout << "first and only pass"  << std::endl;

    this->costs_sp_.assign(N, std::vector<int64_t>(N, INF));
    this->shortest_paths_.assign(N,std::vector<std::vector<size_t>>(N));
    this->initializeWeights(costs_sp_, next, INF);
    this->reconstructPaths(next,this->shortest_paths_, INF);
    return;
  }

  if (two_pass_flag_) {
    if (fist_pass) {
      std::cout << "first/two pass"  << std::endl;

      costs_sp_.assign(N, std::vector<int64_t>(N, INF));
      this->initializeWeights(costs_sp_, next, INF);
      return;
    } else if (second_pass) {
      std::cout << "second/two pass"  << std::endl;

      this->initializeNext(next, INF);
      this->shortest_paths_.assign(N,std::vector<std::vector<size_t>>(N));
      this->reconstructPaths(next, this->shortest_paths_, INF);
      return;
    }
  }
}


void Graph::initializeWeights(std::vector<std::vector<int64_t>>& dist,
    std::vector<std::vector<int64_t>>& next,
    int64_t INF) {

  const size_t N = this->numNodes();

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
}

void Graph::initializeNext(std::vector<std::vector<int64_t>>& next, int64_t INF) {
  const size_t N = this->numNodes();

  std::vector<std::vector<int64_t>> dist(N, std::vector<int64_t>(N, INF));

  for (const auto& src : this->edges_) {
    for (const auto& dest : src.second) {
      dist[src.first][dest.first] = dest.second;
      next[src.first][dest.first] = dest.first;
    }
    dist[src.first][src.first] = 0;
  }

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

  // // Floyd-Warshall
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

}

void Graph::reconstructPaths(std::vector<std::vector<int64_t>>& next,
  short_path_container_t& paths,
  int64_t INF) {
  const size_t N = this->numNodes();

  size_t M = only_nodes_of_swaths_flag_ ? only_nodes_of_swaths_.size() : N;
  for (size_t idx = 0; idx < M; ++idx) {
    const size_t i = only_nodes_of_swaths_flag_ ? only_nodes_of_swaths_.at(idx) : idx;
    for (size_t jdx = 0; jdx < M; ++jdx) {
      const size_t j = only_nodes_of_swaths_flag_ ? only_nodes_of_swaths_.at(jdx) : jdx;
      if (i != j && next[i][j] != -1) {
        std::vector<size_t> path = {i};
        size_t current = i;
        while (current != j) {
          current = next[current][j];
          path.push_back(current);
        }
        paths[i][j] = path;
      } else if (i != j && next[i][j] == -1) {
        costs_sp_[i][j] = INF;
      }
    }
  }

}
std::vector<std::vector<int64_t>>& Graph::getCosts() {
  return this->costs_sp_;
}

short_path_container_t& Graph::getPaths() {
  return this->shortest_paths_;
}

std::vector<size_t> Graph::shortestPath(size_t from, size_t to, int64_t INF) {
  if (this->numNodes() > 0 && this->shortest_paths_.size() == 0) {
    this->shortestPathsAndCosts(INF);
  }

  if (this->two_pass_flag_ && this->pass_counter_ >= 1) {

     if (this->next_.size() == 0) {
       std::cout << "next not initialized!" << std::endl;
       size_t N = this->numNodes();
       this->next_.assign(N, std::vector<int64_t>(N, -1));
       this->initializeNext(this->next_, INF);
       std::cout << "next initialized succ" << std::endl;

     }
    std::cout << "reconstructing single path" << std::endl;

    return this->reconstructPath(from, to, next_);
  }
  return this->shortest_paths_[from][to];
}

std::vector<size_t> Graph::reconstructPath(size_t from,
    size_t to, std::vector<std::vector<int64_t>>& next) const {
  if (from != to && next[from][to] != -1) {
    std::vector<size_t> path = {from};
    size_t current = from;
    while (current != to) {
      current = next[current][to];
      path.push_back(current);
    }
    return path;
  } else {
    return {};
  }
}

int64_t Graph::shortestPathCost(size_t from, size_t to, int64_t INF) {
  if (this->numNodes() > 0 && this->costs_sp_.size() == 0) {
    this->shortestPathsAndCosts(INF);
  }
  return this->costs_sp_[from][to];
}

void Graph::onlyPathsOfSwaths(bool flag)
{
  this->only_nodes_of_swaths_flag_ = flag;
}

std::vector<size_t> Graph::getOnlyNodesOfSwaths() const
{
  return this->only_nodes_of_swaths_;
}

void Graph::useTwoPass(bool flag) {
  this->two_pass_flag_ = flag;
}

bool Graph::getTwoPassFlag() const {
  return this->two_pass_flag_;
}
void Graph::incrementPassCounter() {
  this->pass_counter_++;
}

}  // namespace f2c::types


