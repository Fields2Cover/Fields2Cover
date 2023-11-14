//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_CELL_H_
#define FIELDS2COVER_TYPES_CELL_H_

#include <gdal/ogr_geometry.h>
#include <string>
#include <boost/math/constants/constants.hpp>
#include "fields2cover/types/Geometries.h"
#include "fields2cover/types/Point.h"
#include "fields2cover/types/LinearRing.h"
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/MultiLineString.h"

namespace f2c::types {

// Function to concat string easily with numbers
template < typename... Args >
inline std::string sstr(Args &&... args) {
  std::ostringstream sstr;
  sstr << std::dec;
  (sstr <<  ...  << args);
  return sstr.str();
}


struct Cell : public Geometries<Cell, OGRPolygon, wkbPolygon, LinearRing> {
 public:
  using Geometries<Cell, OGRPolygon, wkbPolygon, LinearRing>::Geometries;
  Cell();
  explicit Cell(const OGRGeometry* geom);

  explicit Cell(const f2c::types::LinearRing& ring);

  void getGeometry(size_t i, LinearRing& ring);

  void getGeometry(size_t i, LinearRing& ring) const;

  LinearRing getGeometry(size_t i);

  const LinearRing getGeometry(size_t i) const;

  void setGeometry(size_t i, const LinearRing& ring);

  size_t size() const;


  /// Scale this Cell by a scale factor
  void operator*=(double b);

  static Cell Buffer(const Cell& geom, double width);

  static Cell Buffer(const LineString& geom, double width);

  static Cell Buffer(const LinearRing& ring, double width);

  static Cell Buffer(const Point& geom, double width);

  Cell ConvexHull() const;

  void addRing(const LinearRing& ring);
  void addGeometry(const LinearRing& ring);

  const LinearRing getExteriorRing() const;
  const LinearRing getInteriorRing(size_t i_ring) const;

  /// Check if the Cell is convex
  bool isConvex() const;

  /// Get a line that starts from a custom point with a custom angle.
  /// If the point is in this geometry, the line crosses the border.
  LineString getSemiLongCurve(const Point& point, double angle) const;

  /// Get a line that goes through a custom point with a custom angle.
  /// If the point is in this geometry, the line also crosses it.
  LineString getStraightLongCurve(const Point& point, double angle) const;

  /// Compute the sections of a LineString that is inside this Cell
  MultiLineString getLinesInside(const LineString& line) const;

  /// Compute the sections of a MultiLineString that is inside this Cell
  MultiLineString getLinesInside(const MultiLineString& lines) const;

  /// Check if a point is in the border of this Cell
  bool isPointInBorder(const Point& p) const;

  bool isPointIn(const Point& p) const;

  /// Generate a line from a point to the border of this Cell
  LineString createLineUntilBorder(const Point& p, double ang) const;
};

}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_CELL_H_
