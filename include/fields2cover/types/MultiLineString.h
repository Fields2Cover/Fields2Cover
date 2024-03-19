//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_MULTILINESTRING_H_
#define FIELDS2COVER_TYPES_MULTILINESTRING_H_

#include <gdal/ogr_geometry.h>
#include <utility>
#include "fields2cover/types/Geometries.h"
#include "fields2cover/types/LineString.h"

namespace f2c::types {

struct MultiLineString :
    public Geometries<MultiLineString, OGRMultiLineString,
            wkbMultiLineString, LineString> {
 public:
  using Geometries<MultiLineString, OGRMultiLineString,
            wkbMultiLineString, LineString>::Geometries;
  MultiLineString();
  explicit MultiLineString(const OGRGeometry* geom);
  explicit MultiLineString(const LineString& line);
  explicit MultiLineString(const std::initializer_list<LineString>& ls);

  size_t size() const;

  double length() const;

  void operator*=(double b);

  void getGeometry(size_t i, LineString& line);

  void getGeometry(size_t i, LineString& line) const;

  LineString getGeometry(size_t i);

  const LineString getGeometry(size_t i) const;

  void setGeometry(size_t i, const LineString& line);

  void append(const OGRGeometry* geom);

  void addGeometry(const LineString& line);
  void addGeometry(const MultiLineString& lines);

  static MultiLineString getLineSegments(const LineString& line);
  static MultiLineString getLineSegments(const LinearRing& line);

  template <class T, OGRwkbGeometryType R>
  MultiLineString intersection(const Geometry<T, R>& g) const;

  template <class T, OGRwkbGeometryType R>
  static MultiLineString intersection(
      const LineString& line, const Geometry<T, R>& g);
};


template <class T, OGRwkbGeometryType R>
MultiLineString MultiLineString::intersection(const Geometry<T, R>& g) const {
  auto inter = this->data_->Intersection(g.get());
  f2c::types::MultiLineString lines(inter);
  OGRGeometryFactory::destroyGeometry(inter);
  return lines;
}


template <class T, OGRwkbGeometryType R>
MultiLineString MultiLineString::intersection(
    const LineString& line, const Geometry<T, R>& g) {
  auto inter = line->Intersection(g.get());
  f2c::types::MultiLineString lines(inter);
  OGRGeometryFactory::destroyGeometry(inter);
  return lines;
}


}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_MULTILINESTRING_H_
