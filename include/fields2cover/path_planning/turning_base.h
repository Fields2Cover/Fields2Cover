//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_PATH_PLANNING_TURNING_BASE_H_
#define FIELDS2COVER_PATH_PLANNING_TURNING_BASE_H_

#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <limits>
#include <functional>
#include "fields2cover/types.h"
#include "fields2cover/objectives/optimization_class.h"
#include "fields2cover/utils/random.h"

namespace f2c {
namespace pp {

/// Base class for turn planners
class TurningBase {
 public:
  /// @cond DOXYGEN_SHOULD_SKIP_THIS
  virtual ~TurningBase() = default;
  /// @endcond


 public:
  /// Setter for robot parameters
  void setRobotParams(const F2CRobot& _params);

  /// @brief Create a turn that goes from one point with a certain angle to
  /// another point.
  /// @details Start and end point are connected with a line that creates the
  /// inferior border of the turn. The top border is declared in the robot
  /// parameters.
  /// @param _start_pos Start point
  /// @param _start_angle Start angle
  /// @param _end_pos End point
  /// @param _end_angle End angle
  /// @return Path with the computed turn
  F2CPath createTurn(const F2CPoint& _start_pos, double _start_angle,
      const F2CPoint& _end_pos, double _end_angle);

  /// @brief Generate a turn if it has not been computed before.
  /// @param _dist_start_pos Distance between start and end point
  /// @param _start_angle Angle when going into the headland
  /// (0 deg is the angle of the headland)
  /// @param _end_angle Angle when going out of the headland
  F2CPath createTurnIfNotCached(double _dist_start_pos,
      double _start_angle, double _end_angle);

  /// @brief Create a turn.
  /// @param _dist_start_pos Distance between start and end point
  /// @param _start_angle Angle when going into the headland
  /// (0 deg is the angle of the headland)
  /// @param _end_angle Angle when going out of the headland
  virtual F2CPath createSimpleTurn(double _dist_start_pos, double _start_angle,
      double _end_angle) = 0;

  /// @brief Transform the turn parameters representation from two points with
  /// two angles to one distance and two angles.
  /// @param _start_pos Start point
  /// @param _start_angle Start angle
  /// @param _end_pos End point
  /// @param _end_angle End angle
  /// @return Vector with the values of the new representation
  static std::vector<double> transformToNormalTurn(const F2CPoint& _start_pos,
      double _start_angle, const F2CPoint& _end_pos, double _end_angle);

  /// Check if turn is valid
  static bool isTurnValid(const F2CPath& _path, double _dist_start_end,
      double _end_angle, double _max_dist_error = 0.05,
      double _max_rot_error = 0.1);


 private:
  static void correctPath(F2CPath& _path,
      const F2CPoint& _start_pos,
      const F2CPoint& _end_pos,
      float _max_error_dist = 0.05);

 public:
  F2CRobot robot;
  double max_headland_width {std::numeric_limits<double>::max()};
  bool using_cache {true};

 protected:
  // To prevent memory consumption and comparative errors because of doubles
  // ints are used multiplied by 1000.
  std::map<std::vector<int>, F2CPath> path_cache_;
  Random rand_;
};

}  // namespace pp
}  // namespace f2c

#endif  // FIELDS2COVER_PATH_PLANNING_TURNING_BASE_H_
