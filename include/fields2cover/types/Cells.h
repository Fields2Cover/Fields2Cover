//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_CELLS_H_
#define FIELDS2COVER_TYPES_CELLS_H_

#include <gdal/ogr_geometry.h>
#include <vector>
#include "fields2cover/types/Geometry.h"
#include "fields2cover/types/Geometries.h"
#include "fields2cover/types/Point.h"
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/MultiLineString.h"
#include "fields2cover/types/Cell.h"

namespace f2c::types {


/// Collection of Cell geometries
struct Cells : public Geometries<Cells, OGRMultiPolygon, wkbMultiPolygon,
    Cell> {
 public:
  using Geometries<Cells, OGRMultiPolygon, wkbMultiPolygon, Cell>::Geometries;

  Cells();
  /// Constructor using OGRGeometry.
  explicit Cells(const OGRGeometry* geom);
  /// Constructor using one Cell
  explicit Cells(const Cell& c);
  ~Cells() = default;

  /// Get the i^th Cell
  void getGeometry(size_t i, Cell& cell);
  /// Get the i^th Cell
  void getGeometry(size_t i, Cell& cell) const;
  /// Get the i^th Cell
  Cell getGeometry(size_t i);
  /// Get the i^th Cell
  const Cell getGeometry(size_t i) const;
  /// Set the i^th Cell. If i is bigger than size(), the size is expanded to fit
  void setGeometry(size_t i, const Cell& cell);

  /// Get the i^th Cell
  const Cell getCell(size_t i) const;

  /// Get the outer LinearRing of the i^th Cell
  const LinearRing getCellBorder(size_t i) const;
  /// Get the i_ring^th inner LinearRing of the i_cell^th Cell
  const LinearRing getInteriorRing(size_t i_cell, size_t i_ring) const;


  /// Scale the Cells by a factor
  void operator*=(double b);

  /// Add a new Cell
  void addGeometry(const Cell& c);

  /// Add a new LinearRing to the i^th Cell
  void addRing(size_t i, const LinearRing& ring);
  /// Append all the Cell in cs to this Cells
  void append(const Cells& cs);

  /// Get number of Cell
  size_t size() const;

  /// Check if this Cells only has one Cell and that Cell is convex
  bool isConvex() const;

  /// Compute the convex hull of this Cells
  Cell convexHull() const;

  /// Compute the intersection between two Cell
  static Cells intersection(const Cell& cell, const Cell& c);

  /// Compute the intersection between a Cell and this Cells
  Cells intersection(const Cell& c) const;
  /// Compute the intersection between a Cells and this Cells
  Cells intersection(const Cells& c) const;

  /// Compute the difference between a Cell and this Cells
  Cells difference(const Cell& c) const;
  /// Compute the difference between a Cells and this Cells
  Cells difference(const Cells& c) const;

  /// Compute the union between a Cell and this Cells
  Cells unionOp(const Cell& c) const;
  /// Compute the union between a Cells and this Cells
  Cells unionOp(const Cells& c) const;

  /// Compute the union between all the Cell in this Cells
  Cells unionCascaded() const;

  Cells splitByLine(const LineString& line) const;

  Cells splitByLine(const MultiLineString& lines) const;

  LineString createSemiLongLine(const Point& point, double angle) const;

  LineString createStraightLongLine(const Point& point, double angle) const;

  MultiLineString getLinesInside(const LineString& line) const;

  MultiLineString getLinesInside(const MultiLineString& lines) const;

  Cells getCellsInside(const Cells& cell) const;

  bool isPointInBorder(const Point& p) const;

  bool isPointIn(const Point& p) const;

  const Cell getCellWherePoint(const Point& p) const;

  LineString createLineUntilBorder(
      const f2c::types::Point& p, double ang) const;

  template <class T, OGRwkbGeometryType R>
  static Cells buffer(const Geometry<T, R>& geom, double width,
      int side = 0);

  Cells buffer(double width) const;

  Point closestPointOnBorderTo(const Point& p) const;

  MultiLineString getLineSections() const;
};



template <class T, OGRwkbGeometryType R>
Cells Cells::buffer(const Geometry<T, R>& geom, double width, int side) {
  OGRGeometry* buffer = geom.OGRBuffer(width, side);
  Cells cells {buffer};
  OGRGeometryFactory::destroyGeometry(buffer);
  return cells;
}


}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_CELLS_H_
