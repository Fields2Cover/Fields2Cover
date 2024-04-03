//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_ROUTE_H_
#define FIELDS2COVER_TYPES_ROUTE_H_

#include <gdal/ogr_geometry.h>
#include <vector>
#include <numeric>
#include <optional>
#include "fields2cover/types/Swath.h"
#include "fields2cover/types/Swaths.h"
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/MultiPoint.h"
#include "fields2cover/types/Graph2D.h"

namespace f2c::types {

struct Route {
 public:
  const std::vector<Swaths>& getVectorSwaths() const;

  Swaths& getSwaths(size_t i);
  const Swaths& getSwaths(size_t i) const;
  void setSwaths(size_t i, const Swaths& s);
  void addSwaths(const Swaths& s = {});
  void addConnectedSwaths(const MultiPoint& mp = {}, const Swaths& s = {});

  void addSwath(const Swath& s);
  void addSwath(const Swath& s, Graph2D& g);
  void addConnection();
  void addConnection(const Point& p);
  void addConnection(const MultiPoint& mp);
  void addConnection(const std::vector<Point>& vp);

  MultiPoint& getLastConnection();
  const MultiPoint& getLastConnection() const;
  Swaths& getLastSwaths();
  const Swaths& getLastSwaths() const;

  const std::vector<MultiPoint>& getConnections() const;
  MultiPoint& getConnection(size_t i);
  const MultiPoint& getConnection(size_t i) const;
  void setConnection(size_t i, const MultiPoint& mp);

  Point startPoint() const;
  Point endPoint() const;

  size_t sizeVectorSwaths() const;
  size_t sizeConnections() const;


  double length() const;

  LineString asLineString() const;

  bool isEmpty() const;
  Route clone() const;

 private:
  std::vector<Swaths> v_swaths_;
  std::vector<MultiPoint> connections_;
};


}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_ROUTE_H_
