//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef PATH_PLANNING_CHECKER_HPP_
#define PATH_PLANNING_CHECKER_HPP_

#include <gtest/gtest.h>
#include <fstream>
#include "fields2cover.h"


inline testing::AssertionResult isPathCorrect(const F2CPath& path) {
  if (path.size() < 1) {
    return testing::AssertionFailure() <<
      "Error 3001: The path do not have any state";
  } else if (path.size() > 1) {

    int errors_big_dist {0};
    int errors_big_angle {0};
    for (size_t i = 0; i < path.size() - 1; ++i) {
      F2CPoint p2 = path[i].atEnd();
      if (p2.distance(path[i + 1].point) > 5e-3) {
        std::cerr << "Path checker Error 3009: dist between end of state " << i <<
          " " << p2 << " and start of next one " <<
          path[i + 1].point << " is " <<
          p2.distance(path[i + 1].point) << std::endl;
        ++errors_big_dist;
      }
      if (F2CPoint::getAngleDiffAbs(
            path[i + 1].angle, path[i].angle) > M_PI / 8.0) {
        std::cerr << "Path checker Error 3010: Angle between state " << i <<
          " " << p2 << " and start of next one " <<
          path[i + 1].point << " is " <<
          F2CPoint::getAngleDiffAbs(
              path[i + 1].angle, path[i].angle) << std::endl;
        ++errors_big_angle;
      }
    }
    if (errors_big_dist) {
      return testing::AssertionFailure() <<
        "Error 3009: The end of one state and the next one doesn't connect on" <<
        errors_big_dist << " states";
    }
    if (errors_big_angle) {
      return testing::AssertionFailure() <<
        "Error 3010: The end of one state and the next one doesn't connect on" <<
        errors_big_angle << " states";
    }
  }
  return testing::AssertionSuccess();
}

inline testing::AssertionResult IsPathCorrect(
    const F2CPath& path, F2CPoint start, double start_ang,
    F2CPoint end, double end_ang, bool check_y_lower_limit = true) {
  if (path.size() < 1) {
    return testing::AssertionFailure() <<
      "Error 3001: The path do not have any state";
  } else if (check_y_lower_limit &&
      std::any_of(path.begin(), path.end(),
        [] (const f2c::types::PathState& p) {return p.point.getY() < -0.05;})) {
    return testing::AssertionFailure() <<
      "Error 3002: Lower limit on Y axis is crossed.";
  } else if (path[0].point.distance(start) > 1e-2) {
    return testing::AssertionFailure() <<
      "Error 3003: Start point should be " << start << ", but is " <<
      path[0].point <<".";
  } else if (path.atEnd().distance(end) > 1e-2) {
    return testing::AssertionFailure() <<
      "Error 3004: End point should be " << end << ", but is " <<
      path.atEnd() <<".";
  } else if (F2CPoint::getAngleDiffAbs(path[0].angle, start_ang) > 0.1) {
    return testing::AssertionFailure() <<
      "Error 3006: Start angle should be " << start_ang <<
      ", but computed angle is "  << path[0].angle << ".";
  } else if (F2CPoint::getAngleDiffAbs(path.back().angle, end_ang) > 0.1) {
    return testing::AssertionFailure() <<
      "Error 3007: End angle should be " << end_ang <<
      ", but computed angle is "  << path.back().angle << ".";
  } else if (start.distance(end) > path.length()) {
    return testing::AssertionFailure() <<
      "Error 3008: Length of the curve (" << path.length() <<
      ") is smaller than the distance between start to end points (" <<
      start.distance(end) << ").";
  }
  return isPathCorrect(path);
}

#endif  // PATH_PLANNING_CHECKER_HPP_
