#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_obj_optimization_class_computeCost():
  swaths50 = f2c.Swaths(50);

  opt = f2c.OBJ_Optimization_NSwath();

  near(opt.computeCost(swaths50), 50.0);
  swaths50.push_back(f2c.Swath());
  near(opt.computeCost(swaths50), 51.0);
