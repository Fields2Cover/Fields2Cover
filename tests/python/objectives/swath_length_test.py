#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b, error = 1e-7):
  assert abs(a - b) < error


def test_fields2cover_obj_swath_length_computeCost():
  path1 = f2c.LineString();
  path2 = f2c.LineString();
  path3 = f2c.LineString();
  path1.addPoint( 0.0, 1.0);
  path1.addPoint( 4.0, 1.0);
  path2.addPoint( 0.0, 3.0);
  path2.addPoint( 4.0, 3.0);
  path3.addPoint( 0.0, 0.0);
  path3.addPoint( 1.0, 1.0);
  swath1 = f2c.Swath(path1);
  swath2 = f2c.Swath(path2);
  swath3 = f2c.Swath(path3);

  swaths1 = f2c.Swaths();
  [swaths1.push_back(i) for i in [swath1]]
  swaths2 = f2c.Swaths();
  [swaths2.push_back(i) for i in [swath3]]
  swaths3 = f2c.Swaths();
  [swaths3.push_back(i) for i in [swath1, swath2]]
  swaths4 = f2c.Swaths();
  [swaths4.push_back(i) for i in [swath1, swath2, swath1, swath2]]

  length = f2c.OBJ_SwathLength();


  near(length.computeCost(swath1), 4.0);

  near(length.computeCost(swaths1), 4.0);
  near(length.computeCostWithMinimizingSign(swaths1), length.computeCost(swaths1));

  near(length.computeCost(swaths2)*length.computeCost(swaths2), 2.0, 0.001);
  near(length.computeCostWithMinimizingSign(swaths2), length.computeCost(swaths2));

  near(length.computeCost(swaths3), 8.0);
  near(length.computeCostWithMinimizingSign(swaths3), length.computeCost(swaths3));

  near(length.computeCost(swaths4), 16.0);
  near(length.computeCostWithMinimizingSign(swaths4), length.computeCost(swaths4));

def test_fields2cover_obj_swath_length_params_check():
  objective = f2c.OBJ_SwathLength();

  assert not (objective.isMaximizing());
  assert (objective.isMinimizing());

