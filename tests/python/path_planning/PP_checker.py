#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c


def IsPathCorrect(path, start, start_ang, end, end_ang, check_y_lower_limit = True):
  assert not (path.points.size() < 3),                              \
      "Error 3001: The path has only " + str(path.points.size()) +  \
      "points. Expected at least 3 points.";
  
  assert not (check_y_lower_limit and
          any([p for p in path.points if p.getY() < -0.05])),       \
       "Error 3002: Lower limit on Y axis is crossed.";
  assert not ((abs(path.points[0].getX() - start.getX()) > 1e-1) or
    (abs(path.points[0].getX() - start.getX()) > 1e-1)),            \
      "Error 3003: Start point should be " + str(start) + ", but is " +  \
      path.points[0] + ".";
  assert not ((abs(path.points[0].getX() - start.getX()) > 1e-1) or
    (abs(path.points[0].getX() - start.getX()) > 1e-1)),            \
      "Error 3004: End point should be " + str(end) + ", but is " +      \
      str(path.points.back()) +".";
  assert (path.isValid()), "Error 3005: Path is not valid.";
  assert not (f2c.Point.getAngleDiffAbs(path.angles[0], start_ang) > 0.1),  \
      "Error 3006: Start angle should be " + str(start_ang) +                    \
      ", but computed angle is " + str(path.angles[0]) + ".";
  assert not (f2c.Point.getAngleDiffAbs(path.angles.back(), end_ang) > 0.1), \
      "Error 3007: End angle should be " + str(end_ang) +                         \
      ", but computed angle is " + str(path.angles.back()) + ".";
  assert not (start.Distance(end) > path.length()),                        \
      "Error 3008: Length of the curve (" + str(path.length()) +                \
      ") is smaller than the distance between start to end points (" +     \
      str(start.Distance(end)) + ").";
  return True

