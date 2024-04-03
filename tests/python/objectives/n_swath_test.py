#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)

def test_fields2cover_obj_n_swath_getBestValue():
  swaths = f2c.Swaths(2);

  obj = f2c.OBJ_NSwath()
  near(obj.computeCost(swaths), 2)

  for i in range(10):
    swaths.push_back(f2c.Swath());
    near(obj.computeCost(swaths), 3 + i)


def test_fields2cover_obj_n_swath_computeCost():
  swaths1 = f2c.Swaths(1);
  swaths2 = f2c.Swaths(2);
  swaths3 = f2c.Swaths(2);
  swaths_cells = f2c.SwathsByCells();
  swaths_cells.push_back(swaths2);
  swaths_cells.push_back(swaths3);

  obj = f2c.OBJ_NSwath();

  near(obj.computeCost(swaths1), 1.0);
  near(obj.computeCostWithMinimizingSign(swaths1), 1.0);

  near(obj.computeCost(swaths2), 2.0);
  near(obj.computeCostWithMinimizingSign(swaths2), 2.0);

  near(obj.computeCost(swaths3), 2.0);
  near(obj.computeCostWithMinimizingSign(swaths3), 2.0);

  near(obj.computeCost(swaths_cells), 4.0);
  near(obj.computeCostWithMinimizingSign(swaths_cells), 4.0);

  swaths3.emplace_back(f2c.Swath());
  near(obj.computeCost(swaths3), 3.0);

  swaths3.emplace_back(f2c.Swath());
  near(obj.computeCost(swaths3), 4.0);

def test_fields2cover_obj_n_swath_paramsCheck():
  objective = f2c.OBJ_NSwath();

  assert not (objective.isMaximizing());
  assert (objective.isMinimizing());
