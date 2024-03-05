//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_GRAPH2D_H_
#define FIELDS2COVER_TYPES_GRAPH2D_H_

#include <vector>
#include <unordered_map>
#include "fields2cover/types/Graph.h"
#include "fields2cover/types/Point.h"

namespace f2c::types {

class Graph2D : public Graph {
 public:
  using Graph::addDirectedEdge;
  using Graph::addEdge;
  using Graph::removeDirectedEdge;
  using Graph::removeEdge;
  using Graph::allPathsBetween;
  using Graph::shortestPath;
  using Graph::shortestPathCost;

  Graph2D& addDirectedEdge(const Point& from, const Point& to, int64_t cost);
  Graph2D& addEdge(const Point& i, const Point& j, int64_t cost);
  Graph2D& addDirectedEdge(const Point& from, const Point& to);
  Graph2D& addEdge(const Point& i, const Point& j);
  Graph2D& addDirectedEdge(
      const Point& from, const Point& to, Graph2D& short_path_g);
  Graph2D& addEdge(
      const Point& i, const Point& j, Graph2D& short_path_g);

  Graph2D& removeDirectedEdge(const Point& from, const Point& to);
  Graph2D& removeEdge(const Point& i, const Point& j);

  size_t numNodes() const;
  std::vector<Point> getNodes() const;

  size_t nodeToIndex(const Point& p) const;
  Point indexToNode(size_t id) const;

  std::vector<std::vector<Point>> allPathsBetween(
      const Point& from, const Point& to) const;

  std::vector<Point> shortestPath(const Point& from, const Point& to,
        int64_t INF = 1<<30);

  int64_t shortestPathCost(const Point& from, const Point& to,
        int64_t INF = 1<<30);

 protected:
  std::unordered_map<Point, size_t> nodes_to_index_;
  std::unordered_map<size_t, Point> index_to_nodes_;

 private:
  double scale_ {1e3};
};

}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_GRAPH2D_H_

