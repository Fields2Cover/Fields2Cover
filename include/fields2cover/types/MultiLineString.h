//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
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

  size_t size() const;

  double getLength() const;

  void operator*=(double b);

  void getGeometry(int i, LineString& line);

  void getGeometry(int i, LineString& line) const;

  LineString getGeometry(int i);

  const LineString getGeometry(int i) const;

  void setGeometry(int i, const LineString& line);

  void append(const OGRGeometry* geom);

  void addGeometry(const LineString& line);

  static MultiLineString getLineSegments(const LineString& line);
  static MultiLineString getLineSegments(const LinearRing& line);

  template <class T, OGRwkbGeometryType R>
  MultiLineString Intersection(const Geometry<T, R>& g) const;

  template <class T, OGRwkbGeometryType R>
  static MultiLineString Intersection(
      const LineString& line, const Geometry<T, R>& g);
};


template <class T, OGRwkbGeometryType R>
MultiLineString MultiLineString::Intersection(const Geometry<T, R>& g) const {
  auto inter = data->Intersection(g.get());
  f2c::types::MultiLineString lines(inter);
  OGRGeometryFactory::destroyGeometry(inter);
  return lines;
}


template <class T, OGRwkbGeometryType R>
MultiLineString MultiLineString::Intersection(
    const LineString& line, const Geometry<T, R>& g) {
  return std::move(MultiLineString(line.get()).Intersection(g));
}


}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_MULTILINESTRING_H_
