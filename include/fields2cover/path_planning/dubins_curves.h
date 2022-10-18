//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_PATH_PLANNING_DUBINS_CURVES_H_
#define FIELDS2COVER_PATH_PLANNING_DUBINS_CURVES_H_

#include "fields2cover/types.h"
#include "fields2cover/path_planning/turning_base.h"

namespace f2c::pp {

/// Dubins' curves planner
class DubinsCurves : public TurningBase {
 public:
  F2CPath createSimpleTurn(const F2CRobot& robot,
      double dist_start_pos, double start_angle,
      double end_angle, double max_headland_width = 1e5) override;

 public:
  double discretization {0.01};
};

}  // namespace f2c::pp

#endif  // FIELDS2COVER_PATH_PLANNING_DUBINS_CURVES_H_
