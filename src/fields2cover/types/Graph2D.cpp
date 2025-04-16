//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <numeric>
#include "fields2cover/types/Graph2D.h"

namespace f2c::types {

Graph2D& Graph2D::addDirectedEdge(
    const Point& from, const Point& to, int64_t cost) {
  size_t p_from = this->nodes_to_index_.insert(
      std::make_pair(from, nodes_to_index_.size())).first->second;
  size_t p_to   = this->nodes_to_index_.insert(
      std::make_pair(to,   nodes_to_index_.size())).first->second;
  this->index_to_nodes_.insert({{p_from, from}, {p_to, to}});
  this->addDirectedEdge(p_from, p_to, cost);
  return *this;
}

Graph2D& Graph2D::addEdge(const Point& i, const Point& j, int64_t cost) {
  this->addDirectedEdge(i, j, cost);
  return this->addDirectedEdge(j, i, cost);
}

Graph2D& Graph2D::addDirectedEdge(
    const std::optional<Point>& op1, const Point& p2, int64_t cost) {
  if (op1) {
    this->addDirectedEdge(*op1, p2, cost);
  }
  return *this;
}

Graph2D& Graph2D::addEdge(
    const std::optional<Point>& op1, const Point& p2, int64_t cost) {
  if (op1) {
    this->addEdge(*op1, p2, cost);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdge(
    const Point& p1, const std::optional<Point>& op2, int64_t cost) {
  if (op2) {
    this->addDirectedEdge(p1, *op2, cost);
  }
  return *this;
}

Graph2D& Graph2D::addEdge(
    const Point& p1, const std::optional<Point>& op2, int64_t cost) {
  if (op2) {
    this->addEdge(p1, *op2, cost);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdge(
    const std::optional<Point>& op1, const Point& p2, Graph2D& short_path_g) {
  if (op1) {
    this->addDirectedEdge(*op1, p2, short_path_g);
  }
  return *this;
}

Graph2D& Graph2D::addEdge(
    const std::optional<Point>& op1, const Point& p2, Graph2D& short_path_g) {
  if (op1) {
    this->addEdge(*op1, p2, short_path_g);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdge(
    const Point& p1, const std::optional<Point>& op2, Graph2D& short_path_g) {
  if (op2) {
    this->addDirectedEdge(p1, *op2, short_path_g);
  }
  return *this;
}

Graph2D& Graph2D::addEdge(
    const Point& p1, const std::optional<Point>& op2, Graph2D& short_path_g) {
  if (op2) {
    this->addEdge(p1, *op2, short_path_g);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdge(const Point& from, const Point& to) {
  return addDirectedEdge(from, to, int64_t(scale_ * from.distance(to)));
}

Graph2D& Graph2D::addEdge(const Point& i, const Point& j) {
  return addEdge(i, j, int64_t(scale_ * i.distance(j)));
}

Graph2D& Graph2D::addDirectedEdge(
    const Point& from, const Point& to, Graph2D& short_path_g) {
  return addDirectedEdge(from, to, short_path_g.shortestPathCost(from, to));
}

Graph2D& Graph2D::addEdge(
    const Point& i, const Point& j, Graph2D& short_path_g) {
  addDirectedEdge(i, j, short_path_g.shortestPathCost(i, j));
  return addDirectedEdge(j, i, short_path_g.shortestPathCost(j, i));
}


Graph2D& Graph2D::addDirectedEdge(
    const Point& from, const MultiLineString& to) {
  return addDirectedEdge(from, to.closestPointTo(from));
}

Graph2D& Graph2D::addDirectedEdge(
    const MultiLineString& from, const Point& to) {
  return addDirectedEdge(from.closestPointTo(to), to);
}

Graph2D& Graph2D::addEdge(const Point& from, const MultiLineString& to) {
  return addEdge(from, to.closestPointTo(from));
}

Graph2D& Graph2D::addDirectedEdge(const Point& from, const Cell& to) {
  return addDirectedEdge(from, to.closestPointOnBorderTo(from));
}

Graph2D& Graph2D::addDirectedEdge(const Cell& from, const Point& to) {
  return addDirectedEdge(from.closestPointOnBorderTo(to), to);
}

Graph2D& Graph2D::addEdge(const Point& from, const Cell& to) {
  return addEdge(from, to.closestPointOnBorderTo(from));
}

Graph2D& Graph2D::addDirectedEdge(const Point& from, const Cells& to) {
  return addDirectedEdge(from, to.closestPointOnBorderTo(from));
}

Graph2D& Graph2D::addDirectedEdge(const Cells& from, const Point& to) {
  return addDirectedEdge(from.closestPointOnBorderTo(to), to);
}

Graph2D& Graph2D::addEdge(
    const Point& from, const Cells& to) {
  return addEdge(from, to.closestPointOnBorderTo(from));
}

Graph2D& Graph2D::addEdge(
    const std::optional<Point>& op, const MultiLineString& lines) {
  if (op) {
    this->addEdge(*op, lines);
  }
  return *this;
}

Graph2D& Graph2D::addEdge(
    const std::optional<Point>& op, const Cells& cells) {
  if (op) {
    this->addEdge(*op, cells);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(
    const std::vector<Point>& vp, const MultiLineString& lines) {
  for (size_t i = 0; i < vp.size(); ++i) {
    this->addEdge(vp[i], lines);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const std::vector<Point>& vp, const Cells& cells) {
  for (size_t i = 0; i < vp.size(); ++i) {
    this->addEdge(vp[i], cells);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const LineString& line) {
  for (size_t i = 0; i < line.size()-1; ++i) {
    this->addEdge(line[i], line[i+1]);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const MultiLineString& lines) {
  for (size_t i = 0; i < lines.size(); ++i) {
    this->addEdges(lines[i]);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const LinearRing& ring) {
  for (size_t i = 0; i < ring.size()-1; ++i) {
    this->addEdge(ring[i], ring[i+1]);
  }
  return *this;
}
Graph2D& Graph2D::addEdges(const Cell& cell) {
  for (auto&& ring : cell) {
    this->addEdges(ring);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const Cells& cells) {
  for (auto&& c : cells) {
    this->addEdges(c);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(const Swath& s) {
  Point mid_p {(s.startPoint() + s.endPoint()) * 0.5};
  double s_len = s.length();
  this->addDirectedEdge(s.startPoint(), mid_p, int64_t(scale_ * (0.5 * s_len)));
  this->addDirectedEdge(mid_p, s.endPoint(), int64_t(scale_ * (0.5 * s_len)));
  return *this;
}

Graph2D& Graph2D::addEdges(const Swath& s) {
  Point mid_p {(s.startPoint() + s.endPoint()) * 0.5};
  double s_len = s.length();
  this->addEdge(s.startPoint(), mid_p, int64_t(scale_ * (0.5 * s_len)));
  this->addEdge(mid_p, s.endPoint(), int64_t(scale_ * (0.5 * s_len)));
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(const Swaths& swaths) {
  for (auto&& s : swaths) {
    this->addDirectedEdges(s);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const Swaths& swaths) {
  for (auto&& s : swaths) {
    this->addEdges(s);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(const SwathsByCells& swaths) {
  for (auto&& s : swaths) {
    this->addDirectedEdges(s);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const SwathsByCells& swaths) {
  for (auto&& s : swaths) {
    this->addEdges(s);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdge(
    const Point& p, const Swath& swath, int64_t cost) {
  this->addDirectedEdge(p, swath.startPoint(), cost);
  this->addDirectedEdge(p, swath.endPoint(), cost);
  return *this;
}

Graph2D& Graph2D::addDirectedEdge(
    const Swath& swath, const Point& p, int64_t cost) {
  this->addDirectedEdge(swath.startPoint(), p, cost);
  this->addDirectedEdge(swath.endPoint(), p, cost);
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(
    const Point& p, const Swaths& swaths, int64_t cost) {
  for (auto&& s : swaths) {
    this->addDirectedEdge(p, s, cost);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(
    const Swaths& swaths, const Point& p, int64_t cost) {
  for (auto&& s : swaths) {
    this->addDirectedEdge(s, p, cost);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(
    const Point& p, const SwathsByCells& sbc, int64_t cost) {
  for (auto&& s : sbc) {
    this->addDirectedEdges(p, s, cost);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(
    const SwathsByCells& sbc, const Point& p, int64_t cost) {
  for (auto&& s : sbc) {
    this->addDirectedEdges(s, p, cost);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdge(
    const Point& p, const Swath& swath, Graph2D& shortest_graph) {
  this->addDirectedEdge(p, swath.startPoint(), shortest_graph);
  this->addDirectedEdge(p, swath.endPoint(), shortest_graph);
  return *this;
}

Graph2D& Graph2D::addDirectedEdge(
    const Swath& swath, const Point& p, Graph2D& shortest_graph) {
  this->addDirectedEdge(swath.startPoint(), p, shortest_graph);
  this->addDirectedEdge(swath.endPoint(), p, shortest_graph);
  return *this;
}

Graph2D& Graph2D::addEdge(
    const Point& p, const Swath& swath, Graph2D& shortest_graph) {
  this->addEdge(p, swath.startPoint(), shortest_graph);
  this->addEdge(p, swath.endPoint(), shortest_graph);
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(
    const Point& p, const Swaths& swaths, Graph2D& shortest_graph) {
  for (auto&& s : swaths) {
    this->addDirectedEdge(p, s, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(
    const Swaths& swaths, const Point& p, Graph2D& shortest_graph) {
  for (auto&& s : swaths) {
    this->addDirectedEdge(s, p, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(
    const Point& p, const Swaths& swaths, Graph2D& shortest_graph) {
  for (auto&& s : swaths) {
    this->addEdge(p, s, shortest_graph);
  }
  return *this;
}


Graph2D& Graph2D::addDirectedEdges(
    const Point& p, const SwathsByCells& swaths, Graph2D& shortest_graph) {
  for (auto&& s : swaths) {
    this->addDirectedEdges(p, s, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(
    const SwathsByCells& swaths, const Point& p, Graph2D& shortest_graph) {
  for (auto&& s : swaths) {
    this->addDirectedEdges(s, p, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(
    const Point& p, const SwathsByCells& swaths, Graph2D& shortest_graph) {
  for (auto&& s : swaths) {
    this->addEdges(p, s, shortest_graph);
  }
  return *this;
}


Graph2D& Graph2D::addDirectedEdge(const Swath& s1, const Swath& s2,
    Graph2D& shortest_graph) {
  return this->addDirectedEdge(s1.endPoint(), s2.startPoint(), shortest_graph);
}

Graph2D& Graph2D::addEdges(const Swath& s1, const Swath& s2,
    Graph2D& shortest_graph) {
  this->addEdge(s1.startPoint(), s2.startPoint(), shortest_graph);
  this->addEdge(s1.startPoint(), s2.endPoint(),   shortest_graph);
  this->addEdge(s1.endPoint(),   s2.startPoint(), shortest_graph);
  this->addEdge(s1.endPoint(),   s2.endPoint(),   shortest_graph);
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(const Swaths& swaths1, const Swath& s2,
    Graph2D& shortest_graph) {
  for (auto&& s : swaths1) {
    this->addDirectedEdge(s, s2, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const Swaths& swaths1, const Swath& s2,
    Graph2D& shortest_graph) {
  for (auto&& s : swaths1) {
    this->addEdges(s, s2, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(const SwathsByCells& sbc1, const Swath& s2,
    Graph2D& shortest_graph) {
  for (auto&& s : sbc1) {
    this->addDirectedEdges(s, s2, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const SwathsByCells& sbc1, const Swath& s2,
    Graph2D& shortest_graph) {
  for (auto&& s : sbc1) {
    this->addEdges(s, s2, shortest_graph);
  }
  return *this;
}


Graph2D& Graph2D::addDirectedEdges(const Swath& s1, const Swaths& swaths2,
    Graph2D& shortest_graph) {
  for (auto&& s : swaths2) {
    this->addDirectedEdge(s1, s, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const Swath& s1, const Swaths& swaths2,
    Graph2D& shortest_graph) {
  for (auto&& s : swaths2) {
    this->addEdges(s1, s, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(
    const Swaths& swaths1, const Swaths& swaths2,
    Graph2D& shortest_graph) {
  for (auto&& s : swaths2) {
    this->addDirectedEdges(swaths1, s, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const Swaths& swaths1, const Swaths& swaths2,
    Graph2D& shortest_graph) {
  for (auto&& s : swaths2) {
    this->addEdges(swaths1, s, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(
    const SwathsByCells& sbc1, const Swaths& swaths2,
    Graph2D& shortest_graph) {
  for (auto&& s : swaths2) {
    this->addDirectedEdges(sbc1, s, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const SwathsByCells& sbc1, const Swaths& swaths2,
    Graph2D& shortest_graph) {
  for (auto&& s : swaths2) {
    this->addEdges(sbc1, s, shortest_graph);
  }
  return *this;
}


Graph2D& Graph2D::addDirectedEdges(const Swath& s1, const SwathsByCells& sbc2,
    Graph2D& shortest_graph) {
  for (auto&& s : sbc2) {
    this->addDirectedEdges(s1, s, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const Swath& s1, const SwathsByCells& sbc2,
    Graph2D& shortest_graph) {
  for (auto&& s : sbc2) {
    this->addEdges(s1, s, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(
    const Swaths& swaths1, const SwathsByCells& sbc2, Graph2D& shortest_graph) {
  for (auto&& s : sbc2) {
    this->addDirectedEdges(swaths1, s, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const Swaths& swaths1, const SwathsByCells& sbc2,
    Graph2D& shortest_graph) {
  for (auto&& s : sbc2) {
    this->addEdges(swaths1, s, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addDirectedEdges(
    const SwathsByCells& sbc1, const SwathsByCells& sbc2,
    Graph2D& shortest_graph) {
  for (auto&& s : sbc2) {
    this->addDirectedEdges(sbc1, s, shortest_graph);
  }
  return *this;
}

Graph2D& Graph2D::addEdges(const SwathsByCells& sbc1, const SwathsByCells& sbc2,
    Graph2D& shortest_graph) {
  for (auto&& s : sbc2) {
    this->addEdges(sbc1, s, shortest_graph);
  }
  return *this;
}


Graph2D& Graph2D::addEdgesSwathExtremes(const Swath& s, const Cells& cells) {
  return this->addEdge(s.startPoint(), cells).addEdge(s.endPoint(), cells);
}

Graph2D& Graph2D::addEdgesSwathExtremes(
    const Swath& s, const MultiLineString& lines) {
  return this->addEdge(s.startPoint(), lines).addEdge(s.endPoint(), lines);
}

Graph2D& Graph2D::addEdgesSwathExtremes(
    const Swaths& swaths, const Cells& cells) {
  for (auto&& s : swaths) {
    this->addEdgesSwathExtremes(s, cells);
  }
  return *this;
}
Graph2D& Graph2D::addEdgesSwathExtremes(
    const Swaths& swaths, const MultiLineString& lines) {
  for (auto&& s : swaths) {
    this->addEdgesSwathExtremes(s, lines);
  }
  return *this;
}

Graph2D& Graph2D::addEdgesSwathExtremes(
    const SwathsByCells& swaths, const MultiLineString& lines) {
  for (auto&& s : swaths) {
    this->addEdgesSwathExtremes(s, lines);
  }
  return *this;
}

Graph2D& Graph2D::addEdgesSwathExtremes(
    const SwathsByCells& swaths, const Cells& cells) {
  for (auto&& s : swaths) {
    this->addEdgesSwathExtremes(s, cells);
  }
  return *this;
}


Graph2D& Graph2D::removeDirectedEdge(const Point& from, const Point& to) {
  this->removeDirectedEdge(nodeToIndex(from), nodeToIndex(to));
  return *this;
}

Graph2D& Graph2D::removeEdge(const Point& i, const Point& j) {
  return this->removeDirectedEdge(i, j).removeDirectedEdge(j, i);
}

size_t Graph2D::numNodes() const {
  return this->nodes_to_index_.size();
}

std::vector<Point> Graph2D::getNodes() const {
  std::vector<Point> nodes;
  for (auto&& n : nodes_to_index_) {
    nodes.emplace_back(n.first);
  }
  return nodes;
}

size_t Graph2D::nodeToIndex(const Point& p) const {
  return this->nodes_to_index_.at(p);
}

Point Graph2D::indexToNode(size_t id) const {
  return this->index_to_nodes_.at(id);
}

std::vector<std::vector<Point>> Graph2D::allPathsBetween(
    const Point& from, const Point& to) const {
  auto int_routes = this->allPathsBetween(nodeToIndex(from), nodeToIndex(to));

  std::vector<std::vector<Point>> routes;
  for (auto&& i_route : int_routes) {
    routes.emplace_back();
    for (auto&& i : i_route) {
      routes.back().emplace_back(indexToNode(i));
    }
  }
  return routes;
}

std::vector<Point> Graph2D::shortestPath(
    const Point& from, const Point& to, int64_t INF) {
  auto i_path = this->shortestPath(nodeToIndex(from), nodeToIndex(to), INF);
  std::vector<Point> path;
  for (auto&& i : i_path) {
    path.emplace_back(indexToNode(i));
  }
  return path;
}

int64_t Graph2D::shortestPathCost(
    const Point& from, const Point& to, int64_t INF) {
  return this->shortestPathCost(nodeToIndex(from), nodeToIndex(to), INF);
}


Graph2D& Graph2D::reconnectEdges(double d_tol, size_t n) {
  std::vector<Point> nodes = this->getNodes();
  for (size_t i = 0; i < n; ++i) {
    for (auto&& edge : this->getEdges()) {
      size_t from = edge.first;
      for (auto&& e : edge.second) {
        size_t to = e.first;
        LineString border {this->indexToNode(from), this->indexToNode(to)};
        for (auto&& node : nodes) {
          if (node != border[0] && node != border[1] &&
              node.distance(border) < d_tol) {
            this->addEdge(border[0], node);
            this->addEdge(node, border[1]);
            this->removeEdge(border[0], border[1]);
          }
        }
      }
    }
  }
  return *this;
}


int64_t Graph2D::getCostFromEdge(
    const Point& from, const Point& to, int64_t INF) const {
  if ((nodes_to_index_.find(from) == nodes_to_index_.end()) ||
      (nodes_to_index_.find(to)   == nodes_to_index_.end())) {
    return INF;
  }
  return getCostFromEdge(nodeToIndex(from), nodeToIndex(to), INF);
}

int64_t Graph2D::getCostFromEdge(
    const std::optional<Point>& from, const Point& to, int64_t INF) const {
  if (!from) {
    return INF;
  }
  return getCostFromEdge(*from, to, INF);
}

int64_t Graph2D::getCostFromEdge(
    const Point& from, const std::optional<Point>& to, int64_t INF) const {
  if (!to) {
    return INF;
  }
  return getCostFromEdge(from, *to, INF);
}


}  // namespace f2c::types


