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


Graph2D& Graph2D::removeDirectedEdge(const Point& from, const Point& to) {
  if (!hasNode(from) || !hasNode(to)) {
    return *this;
  }
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
  // In id order. Both maps are unordered, so walking either one hands back an
  // order that is not the ids' and is not the same on another build. An id is
  // the node count at the time the node was first seen and nodes are never
  // erased, so the ids are dense over 0..numNodes()-1 and at() cannot throw.
  std::vector<Point> nodes;
  nodes.reserve(this->numNodes());
  for (size_t i = 0; i < this->numNodes(); ++i) {
    nodes.emplace_back(index_to_nodes_.at(i));
  }
  return nodes;
}

bool Graph2D::hasNode(const Point& p) const {
  return this->nodes_to_index_.count(p) > 0;
}

size_t Graph2D::nodeToIndex(const Point& p) const {
  return this->nodes_to_index_.at(p);
}

Point Graph2D::indexToNode(size_t id) const {
  return this->index_to_nodes_.at(id);
}

std::vector<std::vector<Point>> Graph2D::allPathsBetween(
    const Point& from, const Point& to) const {
  if (!hasNode(from) || !hasNode(to)) {
    return {};
  }
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
  if (!hasNode(from) || !hasNode(to)) {
    return {};
  }
  auto i_path = this->shortestPath(nodeToIndex(from), nodeToIndex(to), INF);
  std::vector<Point> path;
  for (auto&& i : i_path) {
    path.emplace_back(indexToNode(i));
  }
  return path;
}

int64_t Graph2D::shortestPathCost(
    const Point& from, const Point& to, int64_t INF) {
  if (!hasNode(from) || !hasNode(to)) {
    return INF;
  }
  return this->shortestPathCost(nodeToIndex(from), nodeToIndex(to), INF);
}


}  // namespace f2c::types


