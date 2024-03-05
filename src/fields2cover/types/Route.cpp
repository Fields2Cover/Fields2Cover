//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Route.h"

namespace f2c::types {

const std::vector<Swaths>& Route::getVectorSwaths() const {
  return this->v_swaths_;
}

Swaths& Route::getSwaths(size_t i) {
  return this->v_swaths_[i];
}

const Swaths& Route::getSwaths(size_t i) const {
  return this->v_swaths_[i];
}

void Route::setSwaths(size_t i, const Swaths& s) {
  this->v_swaths_[i] = s;
}

void Route::addSwaths(const Swaths& s) {
  if (this->sizeConnections() == 0) {
    this->connections_.emplace_back();
  }
  if (this->sizeConnections() > this->sizeVectorSwaths()) {
    this->v_swaths_.emplace_back(s);
  } else {
    this->v_swaths_.back().append(s);
  }
}

void Route::addConnectedSwaths(const MultiPoint& mp, const Swaths& s) {
  this->addConnection(mp);
  this->addSwaths(s);
}

void Route::addSwath(const Swath& s) {
  size_t nc = this->sizeConnections();
  const size_t ns = this->sizeVectorSwaths();
  if (nc == 0) {
    this->connections_.emplace_back();
    ++nc;
  }
  if (nc > ns || ns == 0) {
    this->v_swaths_.emplace_back();
  }
  this->v_swaths_.back().emplace_back(s);
}

void Route::addSwath(const Swath& s, Graph2D& g) {
  if (!this->isEmpty()) {
    this->addConnection(g.shortestPath(this->endPoint(), s.startPoint()));
  }
  this->addSwath(s);
}

void Route::addConnection() {
  if (this->sizeConnections() <= this->sizeVectorSwaths()) {
    this->connections_.emplace_back();
  }
}

void Route::addConnection(const Point& p) {
  this->addConnection();
  this->connections_.back().addPoint(p);
}

void Route::addConnection(const MultiPoint& mp) {
  if (mp.size() < 1) {
    this->addConnection();
  }
  for (auto&& p : mp) {
    this->addConnection(p);
  }
}
void Route::addConnection(const std::vector<Point>& vp) {
  if (vp.size() < 1) {
    this->addConnection();
  }
  for (auto&& p : vp) {
    this->addConnection(p);
  }
}

MultiPoint& Route::getLastConnection() {
  return this->connections_.back();
}

const MultiPoint& Route::getLastConnection() const {
  return this->connections_.back();
}

Swaths& Route::getLastSwaths() {
  return this->v_swaths_.back();
}

const Swaths& Route::getLastSwaths() const {
  return this->v_swaths_.back();
}

const std::vector<MultiPoint>& Route::getConnections() const {
  return this->connections_;
}

MultiPoint& Route::getConnection(size_t i) {
  return this->connections_[i];
}

const MultiPoint& Route::getConnection(size_t i) const {
  return this->connections_[i];
}

void Route::setConnection(size_t i, const MultiPoint& mp) {
  this->connections_[i] = mp;
}

Point Route::startPoint() const {
  if (sizeConnections() > 0 && connections_[0].size() > 0) {
    return connections_[0].getFirstPoint();
  } else if (sizeVectorSwaths() > 0 && v_swaths_[0].size() > 0) {
    return v_swaths_[0][0].startPoint();
  }
  return {};
}

Point Route::endPoint() const {
  if (sizeConnections() > sizeVectorSwaths() &&
      connections_.back().size() > 0) {
    return connections_.back().getLastPoint();
  } else if (sizeVectorSwaths() > 0 && v_swaths_.back().size() > 0) {
    return v_swaths_.back().back().endPoint();
  }
  return {};
}

size_t Route::sizeVectorSwaths() const {
  return this->v_swaths_.size();
}

size_t Route::sizeConnections() const {
  return this->connections_.size();
}

double Route::length() const {
  return asLineString().length();
}

LineString Route::asLineString() const {
  if (isEmpty()) {
    return LineString();
  }
  LineString line;
  if (connections_.size() > 0) {
    for (auto&& point : connections_[0]) {
      line.addPoint(point);
    }
  }
  for (size_t i = 0; i < v_swaths_.size(); ++i) {
    for (auto&& swath : v_swaths_[i]) {
      for (const auto& point : swath.getPath()) {
        line.addPoint(point);
      }
    }
    if ((connections_.size() > i + 1)) {
      for (auto&& point : connections_[i+1]) {
        line.addPoint(point);
      }
    }
  }
  return line;
}

bool Route::isEmpty() const {
  return v_swaths_.empty() && connections_.empty();
}

Route Route::clone() const {
  Route new_r;
  for (auto&& s : this->v_swaths_) {
    new_r.v_swaths_.emplace_back(s.clone());
  }
  for (auto&& c : this->connections_) {
    new_r.connections_.emplace_back(c.clone());
  }
  return new_r;
}


}  // namespace f2c::types

