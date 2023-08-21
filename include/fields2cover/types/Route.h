//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
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

namespace f2c::types {

enum RouteType { R_NONE = 0, R_START = 1, R_END = 2, R_START_END = 3};

struct Route {
 public:
  std::vector<Swaths> v_swaths;
  std::vector<MultiPoint> connections;
  RouteType type {RouteType::R_NONE};

 public:
  double getLength() const;

  LineString getRouteAsLine() const;

  bool isValid() const;
  bool isEmpty() const;
  Route clone() const;
};


}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_ROUTE_H_
