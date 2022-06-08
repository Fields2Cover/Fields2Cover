//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef PATH_PLANNING_CHECKER_HPP_
#define PATH_PLANNING_CHECKER_HPP_

#include <gtest/gtest.h>
#include <fstream>
#include "fields2cover.h"


inline testing::AssertionResult IsPathCorrect(
    const F2CPath& path, F2CPoint start, double start_ang,
    F2CPoint end, double end_ang, bool check_y_lower_limit = true) {
  if (path.points.size() < 3) {
    return testing::AssertionFailure() <<
      "Error 3001: The path has only " << path.points.size() <<
      "points. Expected at least 3 points.";
  } else if (check_y_lower_limit &&
      std::any_of(path.points.begin(), path.points.end(),
        [] (const F2CPoint& p) {return p.getY() < -0.05;})) {
    return testing::AssertionFailure() <<
      "Error 3002: Lower limit on Y axis is crossed.";
  } else if ((fabs(path.points.begin()->getX() - start.getX()) > 1e-1) ||
    (fabs(path.points.begin()->getX() - start.getX()) > 1e-1)) {
    return testing::AssertionFailure() <<
      "Error 3003: Start point should be " << start << ", but is " <<
      path.points[0] <<".";
  } else if ((fabs(path.points.begin()->getX() - start.getX()) > 1e-1) ||
    (fabs(path.points.begin()->getX() - start.getX()) > 1e-1)) {
    return testing::AssertionFailure() <<
      "Error 3004: End point should be " << end << ", but is " <<
      path.points.back() <<".";
  } else if (!path.isValid()) {
    return testing::AssertionFailure() << "Error 3005: Path is not valid.";
  } else if (F2CPoint::getAngleDiffAbs(path.angles[0], start_ang) > 0.1) {
    return testing::AssertionFailure() <<
      "Error 3006: Start angle should be " << start_ang <<
      ", but computed angle is "  << path.angles[0] << ".";
  } else if (F2CPoint::getAngleDiffAbs(path.angles.back(), end_ang) > 0.1) {
    return testing::AssertionFailure() <<
      "Error 3007: End angle should be " << end_ang <<
      ", but computed angle is "  << path.angles.back() << ".";
  } else if (start.Distance(end) > path.length()) {
    return testing::AssertionFailure() <<
      "Error 3008: Length of the curve (" << path.length() <<
      ") is smaller than the distance between start to end points (" <<
      start.Distance(end) << ").";
  }
  return testing::AssertionSuccess();
}

#endif  // PATH_PLANNING_CHECKER_HPP_
