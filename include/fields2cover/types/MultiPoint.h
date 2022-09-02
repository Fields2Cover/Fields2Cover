//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_MULTIPOINT_H_
#define FIELDS2COVER_TYPES_MULTIPOINT_H_

#include <gdal/ogr_geometry.h>
#include "fields2cover/types/Geometries.h"
#include "fields2cover/types/Point.h"

namespace f2c::types {

struct MultiPoint :
  public Geometries<MultiPoint, OGRMultiPoint, wkbMultiPoint, Point> {
 public:
  using Geometries<MultiPoint, OGRMultiPoint, wkbMultiPoint, Point>::Geometries;
  MultiPoint();
  MultiPoint(const std::vector<Point>& ps);
  MultiPoint(const std::initializer_list<Point>& ps);

  size_t size() const;

  void getGeometry(int i, Point& point);

  void getGeometry(int i, Point& point) const;

  Point getGeometry(int i);

  const Point getGeometry(int i) const;

  void setGeometry(int i, const Point& p);

  void addGeometry(const Point& p);
  void addPoint(const Point& p);
  void addPoint(double x, double y, double z = 0);

  void operator*=(double b);
};

}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_MULTIPOINT_H_
