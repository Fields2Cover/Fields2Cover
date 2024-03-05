#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c
import math
import numpy as np
from .PP_checker import IsPathCorrect

def near(a, b, error = 1e-7):
  assert abs(a - b) < error

def test_fields2cover_pp_reeds_shepp_turnDist():
  robot = f2c.Robot(2.1, 2.5);
  robot.setCruiseVel(2.0);
  robot.setMaxCurv(1.0);
  robot.setMaxDiffCurv(1.0);

  turn = f2c.PP_ReedsSheppCurves();
  start = f2c.Point(0.0, 0.0);
  end = f2c.Point(-3.0, 0.0);
  path = turn.createTurn(robot, start, 0.5 * math.pi, end, 1.5 * math.pi);
  IsPathCorrect(path, start, 0.5 * math.pi, end, 1.5 * math.pi, False);

def test_fields2cover_pp_reeds_shepp_randomPoints():
  robot = f2c.Robot(2.1, 2.5);
  robot.setCruiseVel(2.0);
  robot.setMaxCurv(1.0);
  robot.setMaxDiffCurv(1.0);
  turn = f2c.PP_ReedsSheppCurves();
  turn.using_cache = False;
  for ang in np.arange(0.25, math.pi, 0.25):
    start = f2c.Point(0.0, 0.0);
    end = f2c.Point(4.0, 0.0);
    path = turn.createTurn(robot, start, ang, end, ang + math.pi);
    IsPathCorrect(path, start, ang, end, ang + math.pi, False);

