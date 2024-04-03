//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_PATH_PLANNING_REEDS_SHEPP_CURVES_HC_H_
#define FIELDS2COVER_PATH_PLANNING_REEDS_SHEPP_CURVES_HC_H_

#include "fields2cover/types.h"
#include "fields2cover/path_planning/turning_base.h"

namespace f2c::pp {

/// Reeds-Shepp's curves planner with continuous curves
class ReedsSheppCurvesHC : public TurningBase {
 public:
  F2CPath createSimpleTurn(const F2CRobot& robot,
      double dist_start_pos, double start_angle, double end_angle) override;
};

}  // namespace f2c::pp

#endif  // FIELDS2COVER_PATH_PLANNING_REEDS_SHEPP_CURVES_HC_H_
