//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_COMPLETE_TURN_PATH_OBJ_H_
#define FIELDS2COVER_OBJECTIVES_COMPLETE_TURN_PATH_OBJ_H_

#include "fields2cover/types.h"
#include "fields2cover/objectives/path_objective.h"
#include "fields2cover/objectives/direct_dist_path_obj.h"
#include "fields2cover/path_planning/turning_base.h"

namespace f2c::obj {

/// @brief Class to compute the cost of turning from one point to another.
/// @details This is the closest result to the real cost of executing the path
/// plan, as it actually compute all the turns.
///
/// @warning Do not use this objective function with slow planners as
/// too many turns may need to be computed for a simple path.
template <class T, class R = DirectDistPathObj>
class CompleteTurnPathObj : public PathObjective {
  static_assert(std::is_base_of<pp::TurningBase, T>::value,
      "T must derive from f2c::pp::TurningBase");

 public:
  using PathObjective::PathObjective;
  explicit CompleteTurnPathObj(const F2CRobot& params);

 public:
  using PathObjective::computeCost;

  /// Compute the cost of doing the turn between p1 with angle ang1 to
  /// p2 with angle ang2
  double computeCost(
      const F2CPoint& p1, double ang1,
      const F2CPoint& p2, double ang2) override;

 public:
  /// Planner that derives from f2c::pp::TurningBase
  T turn_planner;

 private:
  R objective_;
};


template <class T, class R>
CompleteTurnPathObj<T, R>::CompleteTurnPathObj(const F2CRobot& robot) {
  turn_planner.setRobotParams(robot);
}


template <class T, class R>
double CompleteTurnPathObj<T, R>::computeCost(
    const F2CPoint& p1, double ang1,
    const F2CPoint& p2, double ang2) {
  return objective_.computeCost(turn_planner.createTurn(p1, ang1, p2, ang2));
}


}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_COMPLETE_TURN_PATH_OBJ_H_
