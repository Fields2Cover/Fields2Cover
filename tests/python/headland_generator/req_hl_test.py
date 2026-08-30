#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import math
import pytest
import fields2cover as f2c

def near(a, b, error = 1e-2):
  assert abs(a - b) < error


def test_fields2cover_hg_req_gen_headland_width_follows_the_border_angle():
  cell = f2c.Cell(f2c.LinearRing(f2c.VectorPoint([
    f2c.Point(0,0), f2c.Point(100,0), f2c.Point(100,100),
    f2c.Point(0,100), f2c.Point(0,0)])));
  robot = f2c.Robot(3.0, 3.0);
  robot.setMinTurningRadius(5.0);
  hl_gen = f2c.HG_Req_gen();

  near(hl_gen.requiredHeadlandDist(robot, 0.0, 0.0), 6.5);
  near(hl_gen.requiredHeadlandDist(robot, 0.0, 0.5 * math.pi), 11.5);

  # Swaths along x: only the borders they end on keep the widest headland.
  hl = hl_gen.generateHeadlands(cell, robot, 0.0);
  near(hl.area(), (100 - 2 * 11.5) * (100 - 2 * 6.5));

  const_hl_gen = f2c.HG_Const_gen();
  const_hl = const_hl_gen.generateHeadlands(cell, robot, 0.0);
  near(const_hl.area(), (100 - 2 * 11.5) * (100 - 2 * 11.5));
  assert hl.area() > const_hl.area();
