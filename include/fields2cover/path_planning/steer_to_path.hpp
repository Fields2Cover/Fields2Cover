//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_PATH_PLANNING_STEER_TO_PATH_HPP_
#define FIELDS2COVER_PATH_PLANNING_STEER_TO_PATH_HPP_

#include <vector>
#include "steering_functions/steering_functions.hpp"
#include "fields2cover/types.h"


namespace f2c::pp {


/// Cast steer::State type from steering_functions library to
/// Path type
inline types::Path steerStatesToPath(
    const std::vector<steer::State>& curve, double const_vel) {
  types::Path path;
  auto compute_time = [&](int i) {
    return ((i + 1 < curve.size()) ?
        F2CPoint(curve[i].x, curve[i].y).Distance(
        F2CPoint(curve[i + 1].x, curve[i + 1].y)) / const_vel : 0);
  };

  f2c::types::PathState state;
  for (size_t i = 0; i < curve.size(); ++i) {
    state.point = F2CPoint(curve[i].x, curve[i].y);
    state.angle = curve[i].theta;
    state.velocity = const_vel;
    state.duration = compute_time(i);
    state.dir = static_cast<types::PathDirection>(curve[i].d);
    state.type = types::PathSectionType::TURN;
    path.states.emplace_back(state);
  }
  path.task_time = std::accumulate(
      path.states.begin(), path.states.end(), 0.0,
      [] (double d, const f2c::types::PathState& s) {return s.duration + d;});
  return path;
}

// Function loop_detected contributed by Phact (https://phact.nl/) company
inline bool loop_detected(const std::vector<steer::Control>& controls) {
  double drTotal = 0;
  for (auto&& c : controls) {
    if (c.kappa != 0) {
      double dr = 0.5 * c.delta_s * c.kappa / M_PI;
      if (fabs(dr) > 0.9) {
        return true;
      }
      drTotal += dr;
    }
  }
  return (fabs(drTotal) > 0.9);
}
// Function loop_detected contributed by Phact (https://phact.nl/) company



}  // namespace f2c::pp

#endif  // FIELDS2COVER_PATH_PLANNING_STEER_TO_PATH_HPP_
