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


namespace f2c {
namespace pp {


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

  for (size_t i = 0; i < curve.size(); ++i) {
    path.points.emplace_back(curve[i].x, curve[i].y);
    path.angles.emplace_back(curve[i].theta);
    path.velocities.emplace_back(const_vel);
    path.durations.emplace_back(compute_time(i));
    path.directions.emplace_back(static_cast<types::PathDirection>(curve[i].d));
    path.type.emplace_back(types::PathSectionType::TURN);
  }
  path.task_time = std::accumulate(
      path.durations.begin(), path.durations.end(), 0.0);
  return path;
}



}  // namespace pp
}  // namespace f2c
#endif  // FIELDS2COVER_PATH_PLANNING_STEER_TO_PATH_HPP_
