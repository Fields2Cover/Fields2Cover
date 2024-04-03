#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import math
import fields2cover as f2c

def near(a, b, error = 1e-7):
  assert abs(a - b) < error


def test_fields2cover_pp_pp_turnDist():
  path1 = f2c.LineString();
  path2 = f2c.LineString();
  path3 = f2c.LineString();
  path1.addPoint( 0.0, 0.0);
  path1.addPoint( 0.0, 1.0);
  path2.addPoint( 3.0, 1.0);
  path2.addPoint( 3.0, 0.0);
  swath1 = f2c.Swath(path1);
  swath2 = f2c.Swath(path2);
  swaths = f2c.Swaths();
  [swaths.push_back(i) for i in [swath1, swath2]]

  robot = f2c.Robot();
  robot.setMaxCurv(1.0/1.5);
  path_planner = f2c.PP_PathPlanning();
  dubins = f2c.PP_DubinsCurves();

  path = path_planner.planPath(robot,swaths, dubins);
  # Precision is quite low so this is just checking a prediction is done
  near(path.length(), 2 + 1.5 * math.pi, 0.1);


