//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_LINEARRING_H_
#define FIELDS2COVER_TYPES_LINEARRING_H_

#include <gdal/ogr_geometry.h>
#include <vector>
#include "fields2cover/types/Geometries.h"
#include "fields2cover/types/Point.h"

namespace f2c::types {

struct LinearRing : public Geometries<LinearRing, OGRLinearRing, wkbLinearRing,
  Point> {
 public:
  using Geometries<LinearRing, OGRLinearRing, wkbLinearRing, Point>::Geometries;
  LinearRing();
  explicit LinearRing(const std::vector<Point>& ps);
  explicit LinearRing(const std::initializer_list<Point>& ps);

  void operator*=(double b);

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
  const Point endPoint() const;
  bool isClockwise() const;

  Point closestPointTo(const Point& p) const;
};


}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_LINEARRING_H_
