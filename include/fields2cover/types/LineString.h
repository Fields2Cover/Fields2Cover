//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_LINESTRING_H_
#define FIELDS2COVER_TYPES_LINESTRING_H_

#include <gdal/ogr_geometry.h>
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
  LineString(const std::initializer_list<Point>& ps);

  void operator*=(double b);

  double getX(size_t i) const;
  double getY(size_t i) const;
  double getZ(size_t i) const;
  double getLength() const;
  void reversePoints() const;
  size_t size() const;

  void getGeometry(int i, Point& point);
  void getGeometry(int i, Point& point) const;
  Point getGeometry(int i);
  const Point getGeometry(int i) const;
  void setGeometry(int i, const Point& p);

  void addPoint(double x, double y, double z = 0);
  void addPoint(const Point& p);
  void addGeometry(const Point& p) {this->addPoint(p);}

  Point StartPoint() const;

  Point EndPoint() const;
};


}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_LINESTRING_H_
