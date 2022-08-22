#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
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

def test_fields2cover_pp_dubins_turnDist():
  robot = f2c.Robot(2.1, 2.5);
  robot.cruise_speed = 2.0;
  robot.max_icc = 1.0;
  robot.linear_curv_change = 1.0;

  turn = f2c.PP_DubinsCurves(robot);
  start = f2c.Point(0.0, 0.0);
  end = f2c.Point(-3.0, 0.0);
  path = turn.createTurn(start, 0.5 * math.pi, end, 1.5 * math.pi);
  IsPathCorrect(path, start, 0.5 * math.pi, end, 1.5 * math.pi);

def test_fields2cover_pp_dubins_randomPoints():
  robot = f2c.Robot(2.1, 2.5);
  robot.cruise_speed = 2.0;
  robot.max_icc = 1.0;
  robot.linear_curv_change = 1.0;
  turn = f2c.PP_DubinsCurves(robot);
  turn.using_cache = False;
  step = 0.01;
  for ang in np.arange(step, math.pi, step):
    start = f2c.Point(0.0, 0.0);
    end = f2c.Point(4.0, 0.0);
    path = turn.createTurn(start, ang, end, ang + math.pi);
    IsPathCorrect(path, start, ang, end, ang + math.pi);


def test_fields2cover_pp_dubins_cachedTurn():
  robot = f2c.Robot(2.1, 2.5);
  robot.cruise_speed = 2.0;
  robot.max_icc = 0.3;
  robot.linear_curv_change = 1.0;
  turn = f2c.PP_DubinsCurves(robot);
  turn.using_cache = True;
  path1 = turn.createTurn(
      f2c.Point(-331.021, 418.463), 4.71239,
      f2c.Point(-328.021, 404.284), 1.5708);
  #path4 = turn.createTurnIfNotCached(14.4929, 0.208505, 3.35);
  path2 = turn.createTurn(
      f2c.Point(-331.021, 418.463), 4.71239,
      f2c.Point(-328.021, 404.284), 1.5708);
  #path3 = turn.createSimpleTurn(14.4929, 0.208505, 3.35);
  assert path1.points.size() == path2.points.size();
  for i in range(path1.points.size()):
    assert (path1.points[i]==path2.points[i]);

