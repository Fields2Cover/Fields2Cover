//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_ROBOT_H_
#define FIELDS2COVER_TYPES_ROBOT_H_

#include <gdal/ogr_geometry.h>
#include <vector>
#include <optional>
#include "fields2cover/types/Point.h"

namespace f2c::types {

struct Robot {
 public:
  double op_width {0.0};
  double robot_width {0.0};  // Distance between wheels
  std::optional<Point> start_point;
  std::optional<Point> end_point;
  double cruise_speed {1.0};
  double max_icc {1.0};  // [1/m]
  double linear_curv_change {2.0};  // [1/m^2]
  std::optional<double> max_accel;
  std::optional<double> min_accel;
  std::optional<double> max_vel;
  std::optional<double> min_vel;
  std::optional<double> max_u_front;
  std::optional<double> min_u_front;
  std::optional<double> max_u_rot;
  std::optional<double> min_u_rot;

 public:
  explicit Robot(
      double robot_width_p,
      double op_width_p) : op_width(op_width_p), robot_width(robot_width_p) {
    if (op_width_p < robot_width_p) {
      throw std::out_of_range(
          "Operational width have to be equal or greater than Robot width.");
    }
    if (robot_width_p <= 0.0) {
      throw std::out_of_range("Robot width has to be greater than 0.");
    }
  }
  Robot() = default;
  ~Robot() = default;
  Robot(const Robot&) = default;
  Robot(Robot &&) = default;
  Robot &operator=(const Robot&) = default;
  Robot &operator=(Robot&&) = default;

  double getRobotWidth() const { return robot_width;}
  void setMinRadius(double rad) {max_icc = 1.0 / fabs(rad + 1e-7);}
};

}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_ROBOT_H_
