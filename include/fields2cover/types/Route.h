//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_ROUTE_H_
#define FIELDS2COVER_TYPES_ROUTE_H_

#include <gdal/ogr_geometry.h>
#include <vector>
#include <numeric>
#include "fields2cover/types/Swath.h"
#include "fields2cover/types/Swaths.h"
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/MultiPoint.h"

namespace f2c {
namespace types {

enum RouteType { R_NONE = 0, R_START = 1, R_END = 2, R_START_END = 3};

struct Route {
 public:
  std::vector<Swaths> v_swaths;
  std::vector<MultiPoint> connections;
  RouteType type;

 public:
  double getLength(void) const;

  LineString getRouteAsLine(void) const;

  bool isValid(void) const;
  bool isEmpty(void) const;
  Route clone(void) const;
};


}  // namespace types
}  // namespace f2c

#endif  //  FIELDS2COVER_TYPES_ROUTE_H_
