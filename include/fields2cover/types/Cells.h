//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_CELLS_H_
#define FIELDS2COVER_TYPES_CELLS_H_

#include <gdal/ogr_geometry.h>
#include "fields2cover/types/Geometry.h"
#include "fields2cover/types/Geometries.h"
#include "fields2cover/types/Point.h"
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/Cell.h"

namespace f2c::types {

struct Cells : public Geometries<Cells, OGRMultiPolygon, wkbMultiPolygon,
    Cell> {
 public:
  using Geometries<Cells, OGRMultiPolygon, wkbMultiPolygon, Cell>::Geometries;
  Cells();
  explicit Cells(const OGRGeometry* geom);

  explicit Cells(const Cell& c);

  void operator*=(double b);

  void addGeometry(const Cell& c);
  void addRing(size_t i, const LinearRing& ring);

  size_t size() const;

  void getGeometry(size_t i, Cell& cell);
  void getGeometry(size_t i, Cell& cell) const;

  Cell getGeometry(size_t i);

  const Cell getGeometry(size_t i) const;

  void setGeometry(size_t i, const Cell& cell);

  const Cell getCell(size_t i) const;

  const LinearRing getCellBorder(size_t i) const;
  const LinearRing getInteriorRing(size_t i_cell, size_t i_ring) const;

  bool isConvex() const;

  Cell ConvexHull() const;

  static Cells Intersection(const Cell& cell, const Cell& c);
  Cells Intersection(const Cell& c) const;
  Cells Intersection(const Cells& c) const;

  Cells Difference(const Cells& c) const;

  Cells Union(const Cells& c) const;

  Cells UnionCascaded() const;

  Cells splitByLine(const LineString& line) const;

  Cells splitByLine(const MultiLineString& lines) const;

  LineString getSemiLongCurve(const Point& point, double angle) const;

  LineString getStraightLongCurve(const Point& point, double angle) const;

  MultiLineString getLinesInside(const LineString& line) const;

  MultiLineString getLinesInside(const MultiLineString& lines) const;

  Cells getCellsInside(const Cells& cell) const;

  bool isPointInBorder(const Point& p) const;

  bool isPointIn(const Point& p) const;

  const Cell getCellWherePoint(const Point& p) const;

  LineString createLineUntilBorder(
      const f2c::types::Point& p, double ang) const;

  template <class T, OGRwkbGeometryType R>
  static Cells Buffer(const Geometry<T, R>& geom, double width,
      int side = 0);

  Cells Buffer(double width) const;
};



template <class T, OGRwkbGeometryType R>
Cells Cells::Buffer(const Geometry<T, R>& geom, double width, int side) {
  OGRGeometry* buffer = geom.OGRBuffer(width, side);
  Cells cells {buffer};
  OGRGeometryFactory::destroyGeometry(buffer);
  return cells;
}


}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_CELLS_H_
