//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_LINEARRING_H_
#define FIELDS2COVER_TYPES_LINEARRING_H_

#include <ogr_geometry.h>
#include <vector>
#include "fields2cover/types/Geometries.h"
#include "fields2cover/types/Point.h"

namespace f2c::types {

struct LineString;

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
  bool isClosed() const;
  LinearRing& closeRing();

  Point closestPointTo(const Point& p) const;

  LinearRing& removePoint(size_t i);

  LineString getSegment(size_t i) const;
  LineString getLastSegment() const;
  double segmentLength(size_t i) const;
  double segmentAng(size_t i) const;

  /// Remove the crossings a ring gets when its segments are offset.
  LinearRing& filterSelfIntersections();

  /// Generate a parallel curve with an offset d on every segment.
  LinearRing getParallelLine(double d) const;

  /// Generate a parallel curve with an offset d[i] for the i^th segment.
  LinearRing getParallelLine(const std::vector<double>& d) const;

  /// Offset each segment away from the interior of the ring.
  LinearRing& bufferOutwards(const std::vector<double>& d);

  /// Offset each segment towards the interior of the ring.
  LinearRing& bufferInwards(const std::vector<double>& d);
};


}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_LINEARRING_H_
