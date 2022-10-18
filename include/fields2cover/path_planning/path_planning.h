//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
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
  /// Connect one swath to the next one in order using a Turning algorithm
  F2CPath searchBestPath(const F2CRobot& robot, const F2CSwaths& swaths,
      TurningBase& turn);

 public:
  double turn_point_dist {0.0};
};

}  // namespace f2c::pp

#endif  // FIELDS2COVER_PATH_PLANNING_PATH_PLANNING_H_
