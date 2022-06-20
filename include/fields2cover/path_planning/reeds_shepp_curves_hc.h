//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_PATH_PLANNING_REEDS_SHEPP_CURVES_HC_H_
#define FIELDS2COVER_PATH_PLANNING_REEDS_SHEPP_CURVES_HC_H_

#include "fields2cover/types.h"
#include "fields2cover/path_planning/turning_base.h"

namespace f2c {
namespace pp {

/// Reeds-Shepp's curves planner with continuous curves
class ReedsSheppCurvesHC : public TurningBase {
 public:
  explicit ReedsSheppCurvesHC(const F2CRobot& params);
  /// @cond DOXYGEN_SHOULD_SKIP_THIS
  ~ReedsSheppCurvesHC() = default;
  ReedsSheppCurvesHC(ReedsSheppCurvesHC &&) = default;
  ReedsSheppCurvesHC() = default;
  ReedsSheppCurvesHC(const ReedsSheppCurvesHC&) = default;
  ReedsSheppCurvesHC &operator=(const ReedsSheppCurvesHC&) = default;
  ReedsSheppCurvesHC &operator=(ReedsSheppCurvesHC&&) = default;
  /// @endcond

  F2CPath createSimpleTurn(double dist_start_pos, double start_angle,
      double end_angle) override;

 public:
  double discretization {0.01};
};

}  // namespace pp
}  // namespace f2c

#endif  // FIELDS2COVER_PATH_PLANNING_REEDS_SHEPP_CURVES_HC_H_
