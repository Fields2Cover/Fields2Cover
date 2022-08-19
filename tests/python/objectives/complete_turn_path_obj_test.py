#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c
import math

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_obj_complete_turn_path_obj_computeCost():
  swath1 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0.0, 0.0), f2c.Point(0.0, 1.0)])));
  swath2 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(3.0, 1.0), f2c.Point(3.0, 0.0)])));
  swaths1 = f2c.Swaths(swath1);
  swaths2 = f2c.Swaths(swath2);

  robot = f2c.Robot(2.1, 2.5);
  robot.max_icc = 2.0;
 
  length = f2c.OBJ_CompleteTurnPathObj_Dubins(robot);

  # Precision is quite low so this is just checking a prediction is done
  near(length.computeCost(swath1, swath2), math.pi, 0.1);

def test_fields2cover_complete_pred_turn_path_obj_paramsCheck():
  robot = f2c.Robot(2.1, 2.5);
  objective = f2c.OBJ_CompleteTurnPathObj_Dubins(robot);
  assert not (objective.isMaximizing());
  assert (objective.isMinimizing());

