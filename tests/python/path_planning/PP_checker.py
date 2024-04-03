#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c


def IsPathCorrect(path, start, start_ang, end, end_ang, check_y_lower_limit = True):
  assert not (path.size() < 3),                              \
      "Error 3001: The path has only " + str(path.size()) +  \
      "points. Expected at least 3 points.";

  assert not (check_y_lower_limit and
          any([s.point for s in path.getStates() if s.point.getY() < -0.05])),       \
       "Error 3002: Lower limit on Y axis is crossed.";
  assert not ((abs(path.getState(0).point.getX() - start.getX()) > 1e-1) or
    (abs(path.getState(0).point.getX() - start.getX()) > 1e-1)),            \
      "Error 3003: Start point should be " + str(start) + ", but is " +  \
      path[0].point + ".";
  assert not ((abs(path.getState(0).point.getX() - start.getX()) > 1e-1) or
    (abs(path.getState(0).point.getX() - start.getX()) > 1e-1)),            \
      "Error 3004: End point should be " + str(end) + ", but is " +      \
      str(path.back().point) +".";
  assert not (f2c.Point.getAngleDiffAbs(path.getState(0).angle, start_ang) > 0.1),  \
      "Error 3006: Start angle should be " + str(start_ang) +                    \
      ", but computed angle is " + str(path.getState(0).angle) + ".";
  assert not (f2c.Point.getAngleDiffAbs(path.back().angle, end_ang) > 0.1), \
      "Error 3007: End angle should be " + str(end_ang) +                         \
      ", but computed angle is " + str(path.back().angle) + ".";
  assert not (start.distance(end) > path.length()),                        \
      "Error 3008: Length of the curve (" + str(path.length()) +                \
      ") is smaller than the distance between start to end points (" +     \
      str(start.distance(end)) + ").";
  return True

