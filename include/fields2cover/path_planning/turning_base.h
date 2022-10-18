//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_PATH_PLANNING_TURNING_BASE_H_
#define FIELDS2COVER_PATH_PLANNING_TURNING_BASE_H_

#include <memory>
#include <map>
#include <vector>
#include <limits>
#include <functional>
#include "fields2cover/types.h"
#include "fields2cover/utils/random.h"

namespace f2c::pp {

/// Base class for turn planners
class TurningBase {
 public:
  /// @brief Create a turn that goes from one point with a certain angle to
  /// another point.
  /// @details Start and end point are connected with a line that creates the
  /// inferior border of the turn.
  /// @param start_pos Start point
  /// @param start_angle Start angle
  /// @param end_pos End point
  /// @param end_angle End angle
  /// @return Path with the computed turn
  F2CPath createTurn(const F2CRobot& robot,
      const F2CPoint& start_pos, double start_angle,
      const F2CPoint& end_pos, double end_angle,
      double max_headland_width = 1e5);

  /// @brief Generate a turn if it has not been computed before.
  /// @param dist_start_pos Distance between start and end point
  /// @param start_angle Angle when going into the headland
  /// (0 deg is the angle of the headland)
  /// @param end_angle Angle when going out of the headland
  F2CPath createTurnIfNotCached(const F2CRobot& robot, double dist_start_pos,
      double start_angle, double end_angle, double max_headland_width = 1e5);

  /// @brief Create a turn.
  /// @param dist_start_pos Distance between start and end point
  /// @param start_angle Angle when going into the headland
  /// (0 deg is the angle of the headland)
  /// @param end_angle Angle when going out of the headland
  virtual F2CPath createSimpleTurn(const F2CRobot& robot, double dist_start_pos,
      double start_angle, double end_angle,
      double max_headland_width = 1e5) = 0;

  /// @brief Transform the turn parameters representation from two points with
  /// two angles to one distance and two angles.
  /// @param start_pos Start point
  /// @param start_angle Start angle
  /// @param end_pos End point
  /// @param end_angle End angle
  /// @return Vector with the values of the new representation
  static std::vector<double> transformToNormalTurn(const F2CPoint& start_pos,
      double start_angle, const F2CPoint& end_pos, double end_angle);

  /// Check if turn is valid
  static bool isTurnValid(const F2CPath& path, double dist_start_end,
      double end_angle, double max_dist_error = 0.05,
      double max_rot_error = 0.1);


 private:
  static void correctPath(F2CPath& path,
      const F2CPoint& start_pos,
      const F2CPoint& end_pos,
      float max_error_dist = 0.05);

 public:
  bool using_cache {true};

 protected:
  // To prevent memory consumption and comparative errors because of doubles
  // ints are used multiplied by 1000.
  std::map<std::vector<int>, F2CPath> path_cache_;
};

}  // namespace f2c::pp

#endif  // FIELDS2COVER_PATH_PLANNING_TURNING_BASE_H_
