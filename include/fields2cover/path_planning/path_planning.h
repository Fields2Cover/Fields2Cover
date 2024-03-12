//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_PATH_PLANNING_PATH_PLANNING_H_
#define FIELDS2COVER_PATH_PLANNING_PATH_PLANNING_H_

#include "fields2cover/types.h"
#include "fields2cover/path_planning/turning_base.h"

namespace f2c::pp {

/// Path planning class to connect a path using a TurningBase class method
class PathPlanning {
 public:
  /// Compute the coverage path using a route as a reference.
  F2CPath planPath(const F2CRobot& robot, const F2CRoute& route,
      TurningBase& turn) const;

  /// Compute path that covers the swaths in order,
  /// using a path planner to connect swaths.
  F2CPath planPath(const F2CRobot& robot, const F2CSwaths& swaths,
      TurningBase& turn) const;

  /// Compute path that goes from the end of the last swath in a F2CSwaths
  /// to the beginning of the first one in another F2CSwaths.
  F2CPath planPathForConnection(const F2CRobot& robot,
      const F2CSwaths& s1, const F2CMultiPoint& mp, const F2CSwaths& s2,
      TurningBase& turn) const;

  /// Compute path that connects two points with angles and visit middle points.
  F2CPath planPathForConnection(const F2CRobot& robot,
      const F2CPoint& p1, double ang1,
      const F2CMultiPoint& mp,
      const F2CPoint& p2, double ang2,
      TurningBase& turn) const;

 private:
  double getSmoothTurningRadius(const F2CRobot& robot) const;
  std::vector<std::pair<F2CPoint, double>> simplifyConnection(
      const F2CRobot& robot,
      const F2CPoint& p1, double ang1,
      const F2CMultiPoint& mp,
      const F2CPoint& p2, double ang2) const;
};

}  // namespace f2c::pp

#endif  // FIELDS2COVER_PATH_PLANNING_PATH_PLANNING_H_
