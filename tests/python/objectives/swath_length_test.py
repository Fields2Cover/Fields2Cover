#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


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
  	
  swaths1 = f2c.Swaths([swath1]);
  swaths2 = f2c.Swaths([swath3]);
  swaths3 = f2c.Swaths([swath1, swath2]);
  swaths4 = f2c.Swaths([swath1, swath2, swath1, swath2]);
  
  objective = f2c.OBJ_SwathLength();


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

