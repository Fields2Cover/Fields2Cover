//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_PATH_PLANNING_DUBINS_CURVES_CC_H_
#define FIELDS2COVER_PATH_PLANNING_DUBINS_CURVES_CC_H_

#include "fields2cover/types.h"
#include "fields2cover/path_planning/turning_base.h"

namespace f2c::pp {

/// Dubins' curves planner with continuous curves
class DubinsCurvesCC : public TurningBase {
 public:
  F2CPath createSimpleTurn(const F2CRobot& robot,
      double dist_start_pos, double start_angle,
      double end_angle) override;
  F2CPath createConstrainedTurn(const F2CRobot& robot,
      const F2CPoint& start_pos, double start_angle, double start_curvature,
      const F2CPoint& end_pos, double end_angle, double end_curvature, const bool fix_loops=false);
};

}  // namespace f2c::pp

#endif  // FIELDS2COVER_PATH_PLANNING_DUBINS_CURVES_CC_H_
