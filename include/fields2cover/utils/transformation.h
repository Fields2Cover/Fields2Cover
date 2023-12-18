//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_UTILS_TRANSFORMATION_H_
#define FIELDS2COVER_UTILS_TRANSFORMATION_H_

#include <gdal/ogr_spatialref.h>
#include <memory>
#include <utility>
#include <string>
#include <algorithm>
#include "fields2cover/types.h"

namespace f2c {

class Transform {
 public:
  static std::unique_ptr<OGRCoordinateTransformation,
    void(*)(OGRCoordinateTransformation*)>
      generateCoordTransf(
          const std::string& coord_sys_from, const std::string& coord_sys_to);

  static F2CPath transformPathWithFieldRef(const F2CPath& path,
      const F2CField& field, const std::string& coord_sys_to);
  static F2CSwath transformSwathWithFieldRef(const F2CSwath& swath,
      const F2CField& field, const std::string& coord_sys_to);
  static F2CSwaths transformSwathsWithFieldRef(const F2CSwaths& swaths,
      const F2CField& field, const std::string& coord_sys_to);


  static void transform(F2CField& field, const std::string& coord_sys_to);

  static void transformToUTM(F2CField& field, bool is_etrs89_opt = true);
  static void transformToPrevCRS(F2CField& field);

  static F2CPath transformToPrevCRS(const F2CPath& p, const F2CField& field);
  static F2CStrip transformToPrevCRS(const F2CStrip& s, const F2CField& field);
  static F2CStrips transformToPrevCRS(const F2CStrips& s, const F2CField& field);
  static F2CSwath transformToPrevCRS(const F2CSwath& s, const F2CField& field);
  static F2CSwaths transformToPrevCRS(const F2CSwaths& s, const F2CField& field);

  static F2CPath transformPath(const F2CPath& p,
      const std::string& coord_sys_from, const std::string& coord_sys_to);
  static F2CStrip transformStrip(const F2CStrip& s,
      const std::string& coord_sys_from, const std::string& coord_sys_to);
  static F2CStrips transformStrips(const F2CStrips& s,
      const std::string& coord_sys_from, const std::string& coord_sys_to);
  static F2CSwath transformSwath(const F2CSwath& s,
      const std::string& coord_sys_from, const std::string& coord_sys_to);
  static F2CSwaths transformSwaths(const F2CSwaths& s,
      const std::string& coord_sys_from, const std::string& coord_sys_to);

  template <class T>
  static T transform(const T& t,
      const std::string& coord_sys_from, const std::string& coord_sys_to);

  template <class T>
  static T transform(const T& t, const F2CPoint& ref_point,
      const std::string& coord_sys_from, const std::string& coord_sys_to);

  static F2CPoint getRefPointInGPS(const F2CField& field);

  static std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)>
        createSptRef(const std::string& coord_sys, bool fail_silently = false);

  static std::unique_ptr<OGRCoordinateTransformation,
    void(*)(OGRCoordinateTransformation*)> createCoordTransf(
    std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)> in,
    std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)> out);
};

template <class T>
T Transform::transform(const T& t, const F2CPoint& ref_point,
    const std::string& coord_sys_from, const std::string& coord_sys_to) {
  return transform(t + ref_point, coord_sys_from, coord_sys_to);
}

template <class T>
T Transform::transform(const T& t,
    const std::string& coord_sys_from, const std::string& coord_sys_to) {
  T new_t = t.clone();
  new_t->transform(generateCoordTransf(coord_sys_from, coord_sys_to).get());
  return new_t;
}

}  // namespace f2c

#endif  // FIELDS2COVER_UTILS_TRANSFORMATION_H_
