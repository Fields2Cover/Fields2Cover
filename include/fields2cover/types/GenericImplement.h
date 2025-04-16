//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_GENERICIMPLEMENT_H_
#define FIELDS2COVER_TYPES_GENERICIMPLEMENT_H_

#include <gdal/ogr_geometry.h>
#include <utility>
#include <vector>
#include <optional>
#include "fields2cover/types/Point.h"
#include "fields2cover/types/RobotWheel.h"

namespace f2c::types {

struct GenericImplement {
 public:
  /// Geometric center of the implement from the attachment point in the robot
  Point c_impl {0, 0};

  /// Center of the coverage region from the geometric center of the implement
  Point c_cov {0, 0};

  /// Width of the implement
  double width {0.0};
  /// Length of the implement
  double length {0.0};
  /// Width covered by the implement (If not set, width is used)
  std::optional<double> cov_width;
  /// Length covered by the implement (If not set, length is used)
  std::optional<double> cov_length;

  /// Velocity when implement is being used
  std::optional<double> vel_impl_on;
  /// Velocity when implement is not being used
  std::optional<double> vel_impl_off;

  /// Maximum instantaneous curvature change when not using the implement
  std::optional<double> max_icc_impl_off;  // [1/m]
  /// Maximum instantaneous curvature change when using the implement
  std::optional<double> max_icc_impl_on;  // [1/m]

  /// Maximum linear curvature change when not using the implement
  std::optional<double> linear_curv_change_impl_off;  // [1/m^2]
  /// Maximum linear curvature change when using the implement
  std::optional<double> linear_curv_change_impl_on;  // [1/m^2]

  /// Vector of the position of the center of the wheels, and the wheel data.
  std::vector<std::pair<Point, RobotWheel>> wheels;

 protected:
  bool fixed_pos {true};  // Pulled implement not yet implemented
};


}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_GENERICIMPLEMENT_H_
