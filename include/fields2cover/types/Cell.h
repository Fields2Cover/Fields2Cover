//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
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

/// Function to concatenate strings with numbers
template < typename... Args >
inline std::string sstr(Args &&... args) {
  std::ostringstream sstr;
  sstr << std::dec;
  (sstr <<  ...  << args);
  return sstr.str();
}

/// Polygon made by one outer LinearRing and zero or more inner LinearRing
struct Cell : public Geometries<Cell, OGRPolygon, wkbPolygon, LinearRing> {
 public:
  using Geometries<Cell, OGRPolygon, wkbPolygon, LinearRing>::Geometries;
  Cell();
  /// Constructor using a pointer to OGRGeometry
  explicit Cell(const OGRGeometry* geom);
  /// Constructor using only the outer LinearRing
  explicit Cell(const f2c::types::LinearRing& ring);
  ~Cell() = default;

  /// Get the i^th LinearRing, being 0 the outer one.
  void getGeometry(size_t i, LinearRing& ring);
  /// Get the i^th LinearRing, being 0 the outer one.
  void getGeometry(size_t i, LinearRing& ring) const;

  /// Get the i^th LinearRing, being 0 the outer one.
  LinearRing getGeometry(size_t i);

  /// Get the i^th LinearRing, being 0 the outer one.
  const LinearRing getGeometry(size_t i) const;

  /// Set the i^th LinearRing, being 0 the outer one.
  /// If i is bigger than the number of LinearRings that exist in this Cell,
  /// new empty LinearRings are created until Cell.size() == i+1.
  void setGeometry(size_t i, const LinearRing& ring);

  /// Return the number of LinearRings contained in the Cell
  size_t size() const;

  /// Scale this Cell by a scale factor
  void operator*=(double b);

  /// Buffer a Cell outwards. If width is negative, it buffers it inwards
  static Cell buffer(const Cell& geom, double width);

  /// Buffer a LineString a distance width from the line
  static Cell buffer(const LineString& geom, double width);

  /// Buffer a Cell made with the LinearRing as its outer border
  static Cell buffer(const LinearRing& ring, double width);

  /// Buffer a point
  static Cell buffer(const Point& geom, double width);

  /// Compute the convex Hull of the Cell
  Cell convexHull() const;

  /// Compute the convex Hull of a Geometry
  template <class T, OGRwkbGeometryType R>
  static Cell convexHull(const Geometry<T, R>& geom);

  /// Add a new LinearRing to the Cell
  void addRing(const LinearRing& ring);

  /// Add a new LinearRing to the Cell
  void addGeometry(const LinearRing& ring);

  /// Get the outer LinearRing
  const LinearRing getExteriorRing() const;
  /// Get the i^th inner LinearRing, starting at 0.
  const LinearRing getInteriorRing(size_t i_ring) const;

  /// Check if the Cell is convex
  bool isConvex() const;

  /// Get a line that starts from a custom point with a custom angle.
  /// If the point is in this geometry, the line crosses the border.
  LineString createSemiLongLine(const Point& point, double angle) const;

  /// Get a line that goes through a custom point with a custom angle.
  /// If the point is in this geometry, the line also crosses it.
  LineString createStraightLongLine(const Point& point, double angle) const;

  /// Compute the sections of a LineString that is inside this cell
  MultiLineString getLinesInside(const LineString& line) const;

  /// Compute the sections of a MultiLineString that is inside this cell
  MultiLineString getLinesInside(const MultiLineString& lines) const;

  /// Check if a point is in the border of this cell
  bool isPointInBorder(const Point& p) const;

  /// Check if a point is inside this cell
  bool isPointIn(const Point& p) const;

  /// Generate a line from a point to the border of this cell
  LineString createLineUntilBorder(const Point& p, double ang) const;

  /// Find the closest point from a point to the border of the field
  Point closestPointOnBorderTo(const Point& p) const;
};

template <class T, OGRwkbGeometryType R>
Cell Cell::convexHull(const Geometry<T, R>& geom) {
  OGRGeometry* c_hull = geom->ConvexHull();
  Cell cell {c_hull};
  OGRGeometryFactory::destroyGeometry(c_hull);
  return cell;
}


}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_CELL_H_
