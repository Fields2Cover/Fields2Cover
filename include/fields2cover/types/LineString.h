//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_LINESTRING_H_
#define FIELDS2COVER_TYPES_LINESTRING_H_

#include <gdal/ogr_geometry.h>
#include <vector>
#include "fields2cover/types/Geometries.h"
#include "fields2cover/types/Point.h"
#include "fields2cover/types/LinearRing.h"

namespace f2c::types {

struct LineString : public Geometries<LineString, OGRLineString, wkbLineString,
  Point> {
 public:
  using Geometries<LineString, OGRLineString, wkbLineString, Point>::Geometries;
  LineString();
  explicit LineString(const LinearRing& ring);
  explicit LineString(const std::vector<Point>& ps);
  explicit LineString(const Point& p1, const Point& p2);
  explicit LineString(const std::initializer_list<Point>& ps);

  void operator*=(double b);

  double X(size_t i) const;
  double Y(size_t i) const;
  double Z(size_t i) const;
  double getX(size_t i) const;
  double getY(size_t i) const;
  double getZ(size_t i) const;
  double length() const;
  void reversePoints();
  size_t size() const;

  void getGeometry(size_t i, Point& point);
  void getGeometry(size_t i, Point& point) const;
  Point getGeometry(size_t i);
  const Point getGeometry(size_t i) const;
  void setGeometry(size_t i, const Point& p);

  void addPoint(double x, double y, double z = 0);
  void addPoint(const Point& p);
  void addGeometry(const Point& p);

  const Point startPoint() const;
  double startAngle() const;

  const Point endPoint() const;
  double endAngle() const;

  Point closestPointTo(const Point& p) const;
};


}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_LINESTRING_H_
