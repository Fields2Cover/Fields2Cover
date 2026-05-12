//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_GRAPH_H_
#define FIELDS2COVER_TYPES_GRAPH_H_

#include <cstdint>
#include <stdexcept>
#include <vector>
#include <functional>
#include <utility>
#include <unordered_map>

namespace f2c::types {

typedef std::unordered_map<size_t, std::unordered_map<size_t, int64_t>>
  map_to_map_to_int;
typedef std::vector<std::vector<std::vector<size_t>>>  short_path_container_t;

class Graph {
 public:
  Graph& addDirectedEdge(size_t from, size_t to, int64_t cost);
  Graph& addEdge(size_t i, size_t j, int64_t cost);
  Graph& removeDirectedEdge(size_t from, size_t to);
  Graph& removeEdge(size_t i, size_t j);

  size_t numNodes() const;
  size_t numEdges() const;
  map_to_map_to_int getEdges() const;

  std::vector<size_t> getEdgesFrom(size_t s) const;

  int64_t getCostFromEdge(size_t from, size_t to,
      int64_t INF = 1e15) const;

  std::vector<std::vector<size_t>> allPathsBetween(
      size_t from, size_t to) const;

  void shortestPathsAndCosts(int64_t INF = 1e15);

  void initializeWeights(std::vector<std::vector<int64_t>>& cost_dest,
      std::vector<std::vector<int64_t>>& next_dest,
      int64_t INF = 1e15);

  void initializeNext(std::vector<std::vector<int64_t>>& next_dest, int64_t INF = 1e15);

  void reconstructPaths(std::vector<std::vector<int64_t>>& next_src,
  short_path_container_t& paths_dest,
    int64_t INF);

  std::vector<std::vector<int64_t>>& getCosts();

  short_path_container_t& getPaths();

  std::vector<size_t> shortestPath(size_t from, size_t to,
        int64_t INF = 1e15);

  std::vector<size_t> reconstructPath(size_t from,
      size_t to,
      std::vector<std::vector<int64_t>>& next) const;

  int64_t shortestPathCost(size_t from, size_t to,
        int64_t INF = 1e15);

  void onlyPathsOfSwaths(bool flag);

  std::vector<size_t> getOnlyNodesOfSwaths() const;

  void useTwoPass(bool flag);

  bool getTwoPassFlag() const;

  void incrementPassCounter();

 protected:
  void DFS(size_t from, size_t to,
    std::vector<std::vector<size_t>>& routes,
    std::vector<bool>& visited,
    int& route_index) const;

 protected:
  map_to_map_to_int edges_;
  short_path_container_t shortest_paths_;       // sequence of nodes
  std::vector<std::vector<int64_t>> costs_sp_;  // costs of shortest paths
  std::vector<size_t> only_nodes_of_swaths_;
  bool only_nodes_of_swaths_flag_ = false;
  bool two_pass_flag_ = false;
  uint8_t pass_counter_ = 1;
  bool selective_path_reconstruct_flag_ = false;
  std::vector<std::vector<int64_t>> next_;      // only saved when selective_path_reconstruct
};

}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_GRAPH_H_
