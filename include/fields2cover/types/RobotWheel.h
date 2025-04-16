//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_ROBOTWHEEL_H_
#define FIELDS2COVER_TYPES_ROBOTWHEEL_H_

#include <gdal/ogr_geometry.h>
#include <utility>
#include <string>
#include <vector>
#include <optional>
#include "fields2cover/types/Point.h"
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/MultiLineString.h"
#include "fields2cover/types/Cell.h"
#include "fields2cover/types/Cells.h"
#include "fields2cover/types/Path.h"

namespace f2c::types {

struct RobotWheel {
 public:
  void setPressurePoints(const LineString& line);
  void addPressurePoint(const Point& p);
  Point getPressurePoint(size_t i) const;
  LineString getPressurePoints() const;
  void setWidth(double w);
  double getWidth() const;

  /// Return the number of pressure points
  size_t size() const;
  //virtual double getAngleWheel(double diff_ang_states) const;

 private:
  /// Pressure points are referenced from the geometric center of the wheel
  LineString pressure_points;
  /// Width of the wheel
  double width;
  /// Length of the wheel
  double length;
};

}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_ROBOTWHEEL_H_
