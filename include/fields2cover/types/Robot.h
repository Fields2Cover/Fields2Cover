//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_ROBOT_H_
#define FIELDS2COVER_TYPES_ROBOT_H_

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

struct Robot {
 public:
  explicit Robot(double width, double cov_width = 0.0,
      double max_curv = 1.0, double max_diff_curv = 0.3);
  Robot();
  ~Robot();
  Robot(const Robot&);
  Robot(Robot &&);
  Robot &operator=(const Robot&);
  Robot &operator=(Robot&&);

  std::string getName() const;
  void setName(const std::string& str);

  double getWidth() const;
  void setWidth(double);

  double getCovWidth() const;
  void setCovWidth(double);

  double getCruiseVel() const;
  void setCruiseVel(double);

  double getTurnVel() const;
  void setTurnVel(double);

  double getMinTurningRadius() const;
  void setMinTurningRadius(double rad);

  double getMaxCurv() const;
  void setMaxCurv(double);

  double getMaxDiffCurv() const;
  void setMaxDiffCurv(double);

 private:
  std::string name_;

  /// Width of the robot
  double width_ {0.0};
  /// Width of the coverage area of the robot
  double cov_width_ {0.0};

  /// Velocity of the robot when not doing turns.
  double cruise_speed_ {1.0};

  /// Maximum instantaneous curvature change
  double max_icc_ {1.0};  // [1/m]

  /// Maximum linear curvature change
  double linear_curv_change_ {0.3};  // [1/m^2]

  /// Velocity of the robot when doing turns. If not set, cruise_speed_ is used
  std::optional<double> turn_vel_;
};

}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_ROBOT_H_
