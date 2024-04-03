//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_COMPLETE_TURN_PATH_OBJ_H_
#define FIELDS2COVER_OBJECTIVES_COMPLETE_TURN_PATH_OBJ_H_

#include "fields2cover/types.h"
#include "fields2cover/objectives/pp_obj/pp_objective.h"
#include "fields2cover/objectives/rp_obj/rp_objective.h"
#include "fields2cover/objectives/rp_obj/direct_dist_path_obj.h"
#include "fields2cover/path_planning/turning_base.h"

namespace f2c::obj {

/// @brief Class to compute the cost of turning from one point to another.
/// @details This is the closest result to the real cost of executing the path
/// plan, as it actually compute all the turns.
///
/// @warning Do not use this objective function with slow planners as
/// too many turns may need to be computed for a simple path.
template <class T, class R = PPObjective>
class CompleteTurnPathObj : public RPObjective {
  static_assert(std::is_base_of<pp::TurningBase, T>::value,
      "T must derive from f2c::pp::TurningBase");
  static_assert(std::is_base_of<PPObjective, R>::value,
      "R must derive from f2c::obj::PPObjective");

 public:
  using RPObjective::RPObjective;
  explicit CompleteTurnPathObj(const F2CRobot& params);
  explicit CompleteTurnPathObj(const F2CRobot& params, const T& turn_planner);

 public:
  using RPObjective::computeCost;

  /// Compute the cost of doing the turn between p1 with angle ang1 to
  /// p2 with angle ang2
  double computeCost(
      const F2CPoint& p1, double ang1,
      const F2CPoint& p2, double ang2) override;

  void setRobot(const F2CRobot& robot);
  void setTurnPlanner(const T& turner);

 private:
  /// Planner that derives from f2c::pp::TurningBase
  T turn_planner;
  F2CRobot robot;
  R pp_objective;
};


template <class T, class R>
CompleteTurnPathObj<T, R>::CompleteTurnPathObj(const F2CRobot& robot) {
  this->setRobot(robot);
}

template <class T, class R>
CompleteTurnPathObj<T, R>::CompleteTurnPathObj(
    const F2CRobot& _robot, const T& _turner) :
        turn_planner(_turner), robot(_robot) {}

template <class T, class R>
void CompleteTurnPathObj<T, R>::setRobot(const F2CRobot& params) {
  this->robot = params;
}

template <class T, class R>
void CompleteTurnPathObj<T, R>::setTurnPlanner(const T& turner) {
  this->turn_planner = turner;
}


template <class T, class R>
double CompleteTurnPathObj<T, R>::computeCost(
    const F2CPoint& p1, double ang1,
    const F2CPoint& p2, double ang2) {
  return pp_objective.computeCost(
      this->turn_planner.createTurn(this->robot, p1, ang1, p2, ang2));
}


}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_COMPLETE_TURN_PATH_OBJ_H_
