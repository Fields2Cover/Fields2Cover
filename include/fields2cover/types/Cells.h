//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
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

namespace f2c {
namespace types {

struct Cells : public Geometries<Cells, OGRMultiPolygon, wkbMultiPolygon,
    Cell> {
 public:
  using Geometries<Cells, OGRMultiPolygon, wkbMultiPolygon, Cell>::Geometries;
  Cells();
  explicit Cells(const OGRGeometry* geom);

  explicit Cells(const Cell& c);

  void operator*=(double b);

  void addGeometry(const Cell& c);
  void addRing(int i, const LinearRing& ring);

  size_t size() const;

  void getGeometry(int i, Cell& cell);
  void getGeometry(int i, Cell& cell) const;

  Cell getGeometry(int i);

  const Cell getGeometry(int i) const;

  void setGeometry(int i, const Cell& cell);

  Cell getCell(int i) const;

  LinearRing getCellBorder(int i) const;
  LinearRing getInteriorRing(int i_cell, int i_ring) const;

  bool isConvex() const;

  Cells Intersection(const Cells& c) const;

  Cells Difference(const Cells& c) const;

  Cells splitByLine(const LineString& line) const;

  Cells splitByLine(const MultiLineString& lines) const;

  Cells Intersection(const Cell& c) const;

  static Cells Intersection(
      const Cell& cell, const Cell& c);

  LineString getSemiLongCurve(const Point& point, double angle) const;

  LineString getStraightLongCurve(const Point& point, double angle) const;

  MultiLineString getLinesInside(const LineString& line) const;

  MultiLineString getLinesInside(const MultiLineString& lines) const;

  Cells getCellsInside(const Cells& cell) const;

  bool isPointInBorder(const Point& p) const;

  bool isPointIn(const Point& p) const;

  Cell getCellWherePoint(const Point& p) const;

  LineString createLineUntilBorder(
      const f2c::types::Point& p, double ang) const;

  template <class T, OGRwkbGeometryType R>
  static Cells Buffer(const Geometry<T, R>& geom, double width);
};



template <class T, OGRwkbGeometryType R>
Cells Cells::Buffer(const Geometry<T, R>& geom, double width) {
  auto buffer = geom->Buffer(width);
  Cells cells {buffer};
  OGRGeometryFactory::destroyGeometry(buffer);
  return cells;
}


}  // namespace types
}  // namespace f2c

#endif  // FIELDS2COVER_TYPES_CELLS_H_
