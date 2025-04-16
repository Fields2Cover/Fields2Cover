//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_GRAPH2D_H_
#define FIELDS2COVER_TYPES_GRAPH2D_H_

#include <vector>
#include <optional>
#include <unordered_map>
#include "fields2cover/types/Graph.h"
#include "fields2cover/types/Point.h"
#include "fields2cover/types/LinearRing.h"
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/MultiLineString.h"
#include "fields2cover/types/Cell.h"
#include "fields2cover/types/Cells.h"
#include "fields2cover/types/Swath.h"
#include "fields2cover/types/Swaths.h"
#include "fields2cover/types/SwathsByCells.h"

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
  using Graph::getCostFromEdge;

  double getScale() const {
    return this->scale_;
  }
  void setScale(double s) {
    this->scale_ = s;
  }

  Graph2D& addDirectedEdge(
      const Point& from, const Point& to, int64_t cost);
  Graph2D& addEdge(
      const Point& i, const Point& j, int64_t cost);

  Graph2D& addDirectedEdge(
      const std::optional<Point>& op1, const Point& p2, int64_t cost);
  Graph2D& addEdge(
      const std::optional<Point>& op1, const Point& p2, int64_t cost);
  Graph2D& addDirectedEdge(
      const Point& p1, const std::optional<Point>& op2, int64_t cost);
  Graph2D& addEdge(
      const Point& p1, const std::optional<Point>& op2, int64_t cost);



  Graph2D& addDirectedEdge(const Point& from, const Point& to);
  Graph2D& addEdge(const Point& i, const Point& j);

  Graph2D& addDirectedEdge(
      const Point& from, const Point& to, Graph2D& short_path_g);
  Graph2D& addEdge(
      const Point& i, const Point& j, Graph2D& short_path_g);

  Graph2D& addDirectedEdge(const std::optional<Point>& op1, const Point& p2,
      Graph2D& short_path_g);
  Graph2D& addEdge(const std::optional<Point>& op1, const Point& p2,
      Graph2D& short_path_g);
  Graph2D& addDirectedEdge(const Point& p1, const std::optional<Point>& op2,
      Graph2D& short_path_g);
  Graph2D& addEdge(const Point& p1, const std::optional<Point>& op2,
      Graph2D& short_path_g);

  Graph2D& addDirectedEdge(const Point& from, const MultiLineString& to);
  Graph2D& addDirectedEdge(const MultiLineString& from, const Point& to);
  Graph2D& addEdge(const Point& p, const MultiLineString& lines);

  Graph2D& addDirectedEdge(const Point& from, const Cell& to);
  Graph2D& addDirectedEdge(const Cell& from, const Point& to);
  Graph2D& addEdge(const Point& p, const Cell& cell);

  Graph2D& addDirectedEdge(const Point& from, const Cells& to);
  Graph2D& addDirectedEdge(const Cells& from, const Point& to);
  Graph2D& addEdge(const Point& p, const Cells& cells);

  Graph2D& addEdge(
      const std::optional<Point>& op, const MultiLineString& lines);
  Graph2D& addEdge(
      const std::optional<Point>& op, const Cells& cells);
  Graph2D& addEdges(
      const std::vector<Point>& vp, const MultiLineString& lines);
  Graph2D& addEdges(
      const std::vector<Point>& vp, const Cells& cells);

  Graph2D& addEdges(const LineString& line);
  Graph2D& addEdges(const MultiLineString& lines);
  Graph2D& addEdges(const LinearRing& ring);
  Graph2D& addEdges(const Cell& cell);
  Graph2D& addEdges(const Cells& cells);

  Graph2D& addDirectedEdges(const Swath& swath);
  Graph2D& addEdges(const Swath& swath);
  Graph2D& addDirectedEdges(const Swaths& swaths);
  Graph2D& addEdges(const Swaths& swaths);
  Graph2D& addDirectedEdges(const SwathsByCells& swaths_by_cells);
  Graph2D& addEdges(const SwathsByCells& swaths_by_cells);

  Graph2D& addDirectedEdge(
      const Point& p, const Swath& swath, int64_t cost);
  Graph2D& addDirectedEdge(
      const Swath& swath, const Point& p, int64_t cost);
  Graph2D& addDirectedEdges(
      const Point& p, const Swaths& swaths, int64_t cost);
  Graph2D& addDirectedEdges(
      const Swaths& swaths, const Point& p, int64_t cost);
  Graph2D& addDirectedEdges(
      const Point& p, const SwathsByCells& sbc, int64_t cost);
  Graph2D& addDirectedEdges(
      const SwathsByCells& sbc, const Point& p, int64_t cost);

  Graph2D& addDirectedEdge(const Point& p,
      const Swath& swath, Graph2D& shortest_graph);
  Graph2D& addDirectedEdge(const Swath& swath,
      const Point& p, Graph2D& shortest_graph);
  Graph2D& addEdge(const Point& p,
      const Swath& swath, Graph2D& shortest_graph);
  Graph2D& addDirectedEdges(const Point& p,
      const Swaths& swaths, Graph2D& shortest_graph);
  Graph2D& addDirectedEdges(const Swaths& swaths,
      const Point& p, Graph2D& shortest_graph);
  Graph2D& addEdges(const Point& p,
      const Swaths& swaths, Graph2D& shortest_graph);
  Graph2D& addDirectedEdges(const Point& p,
      const SwathsByCells& swaths_by_cells, Graph2D& shortest_graph);
  Graph2D& addDirectedEdges(const SwathsByCells& swaths_by_cells,
      const Point& p, Graph2D& shortest_graph);
  Graph2D& addEdges(const Point& p, const SwathsByCells& swaths_by_cells,
      Graph2D& shortest_graph);


  Graph2D& addDirectedEdge(const Swath& s1, const Swath& s2,
      Graph2D& shortest_graph);
  Graph2D& addEdges(const Swath& s1, const Swath& s2,
      Graph2D& shortest_graph);
  Graph2D& addDirectedEdges(const Swaths& swaths1, const Swath& s2,
      Graph2D& shortest_graph);
  Graph2D& addEdges(const Swaths& swaths1, const Swath& s2,
      Graph2D& shortest_graph);
  Graph2D& addDirectedEdges(const SwathsByCells& sbc, const Swath& s2,
      Graph2D& shortest_graph);
  Graph2D& addEdges(const SwathsByCells& sbc1, const Swath& s2,
      Graph2D& shortest_graph);

  Graph2D& addDirectedEdges(const Swath& s1, const Swaths& swaths2,
      Graph2D& shortest_graph);
  Graph2D& addEdges(const Swath& s1, const Swaths& swaths2,
      Graph2D& shortest_graph);
  Graph2D& addDirectedEdges(const Swaths& swaths1, const Swaths& swaths2,
      Graph2D& shortest_graph);
  Graph2D& addEdges(const Swaths& swaths1, const Swaths& swaths2,
      Graph2D& shortest_graph);
  Graph2D& addDirectedEdges(const SwathsByCells& sbc, const Swaths& swaths2,
      Graph2D& shortest_graph);
  Graph2D& addEdges(const SwathsByCells& sbc1, const Swaths& swaths2,
      Graph2D& shortest_graph);

  Graph2D& addDirectedEdges(const Swath& s1, const SwathsByCells& sbc2,
      Graph2D& shortest_graph);
  Graph2D& addEdges(const Swath& s1, const SwathsByCells& sbc2,
      Graph2D& shortest_graph);
  Graph2D& addDirectedEdges(const Swaths& swaths1, const SwathsByCells& sbc2,
      Graph2D& shortest_graph);
  Graph2D& addEdges(const Swaths& swaths1, const SwathsByCells& sbc2,
      Graph2D& shortest_graph);
  Graph2D& addDirectedEdges(const SwathsByCells& sbc, const SwathsByCells& sbc2,
      Graph2D& shortest_graph);
  Graph2D& addEdges(const SwathsByCells& sbc1, const SwathsByCells& sbc2,
      Graph2D& shortest_graph);



  // This method does not include the swath connection itself,
  // just from the extremes to the cells
  Graph2D& addEdgesSwathExtremes(
      const Swath& swath, const Cells& cells);
  Graph2D& addEdgesSwathExtremes(
      const Swath& swath, const MultiLineString& lines);
  // This method does not include the swath connection itself,
  // just from the extremes to the cells
  Graph2D& addEdgesSwathExtremes(
      const Swaths& swaths, const Cells& cells);
  Graph2D& addEdgesSwathExtremes(
      const Swaths& swaths, const MultiLineString& lines);
  // This method does not include the swath connection itself,
  // just from the extremes to the cells
  Graph2D& addEdgesSwathExtremes(
      const SwathsByCells& swaths_by_cells, const Cells& cells);
  Graph2D& addEdgesSwathExtremes(
      const SwathsByCells& swaths_by_cells, const MultiLineString& lines);

  Graph2D& removeDirectedEdge(const Point& from, const Point& to);
  Graph2D& removeEdge(const Point& i, const Point& j);

  int64_t getCostFromEdge(const Point& from, const Point& to,
      int64_t INF = 1e15) const;

  int64_t getCostFromEdge(const std::optional<Point>& from, const Point& to,
      int64_t INF = 1e15) const;

  int64_t getCostFromEdge(const Point& from, const std::optional<Point>& to,
      int64_t INF = 1e15) const;

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

  Graph2D& reconnectEdges(double d_tol, size_t n = 3);

 protected:
  std::unordered_map<Point, size_t> nodes_to_index_;
  std::unordered_map<size_t, Point> index_to_nodes_;

 private:
  double scale_ {1e3};
};

}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_GRAPH2D_H_

