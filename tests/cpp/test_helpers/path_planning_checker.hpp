//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef PATH_PLANNING_CHECKER_HPP_
#define PATH_PLANNING_CHECKER_HPP_

#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>
#include <limits>
#include <vector>
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

// Distance from `q` to the polyline through the path's states. Straight runs
// are a single state, so measuring to the states alone would miss the track
// in between.
inline double distanceToPath(const F2CPath& path, const F2CPoint& q) {
  double best = std::numeric_limits<double>::max();
  for (size_t i = 1; i < path.size(); ++i) {
    best = std::min(best,
        q.distance(F2CLineString(path[i - 1].point, path[i].point)));
  }
  return best;
}

// Largest heading jump between consecutive states.
inline double maxHeadingStep(const F2CPath& path) {
  double worst = 0.0;
  for (size_t i = 1; i < path.size(); ++i) {
    worst = std::max(worst,
        F2CPoint::getAngleDiffAbs(path[i].angle, path[i - 1].angle));
  }
  return worst;
}

// Widest heading excursion of any single turn: how far its running signed
// heading change spreads. A corner or an S stays inside half a revolution;
// a loop runs past a full one. Length and maxHeadingStep both miss a loop,
// because it is smooth and only 2*pi*R longer.
inline double maxTurnSweep(const F2CPath& path) {
  double worst = 0.0;
  size_t i = 0;
  while (i < path.size()) {
    if (path[i].type != f2c::types::PathSectionType::TURN) {
      ++i;
      continue;
    }
    double run = 0.0, lo = 0.0, hi = 0.0;
    for (++i; i < path.size() &&
        path[i].type == f2c::types::PathSectionType::TURN; ++i) {
      double d = path[i].angle - path[i - 1].angle;
      run += atan2(sin(d), cos(d));       // signed, wrap-safe
      lo = std::min(lo, run);
      hi = std::max(hi, run);
    }
    worst = std::max(worst, hi - lo);
  }
  return worst;
}

// First point of each maximal run of TURN-type states, in order -- the
// entry pose of each connection turn the path was rounded through.
inline std::vector<F2CPoint> turnStarts(const F2CPath& path) {
  std::vector<F2CPoint> starts;
  bool in_turn = false;
  for (auto&& s : path) {
    if (s.type == f2c::types::PathSectionType::TURN) {
      if (!in_turn) {
        starts.push_back(s.point);
      }
      in_turn = true;
    } else {
      in_turn = false;
    }
  }
  return starts;
}

#endif  // PATH_PLANNING_CHECKER_HPP_
