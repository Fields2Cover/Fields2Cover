#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_route_snake_genSortedSwathsEven():
  n = 11;
  swaths = f2c.Swaths();
  for i in range(1, n):
    swaths.push_back(f2c.Swath(f2c.LineString(f2c.VectorPoint(  \
        [f2c.Point(0, i), f2c.Point(1, i)])), i, i));

  swath_sorter = f2c.RP_Snake();
  objective = f2c.OBJ_DirectDistPathObj();

  swaths = swath_sorter.genSortedSwaths(swaths, 0);

  near(swaths[0].getWidth(), 1);
  near(swaths[1].getWidth(), 3);
  near(swaths[2].getWidth(), 5);
  near(swaths.back().getWidth(), 2);

  near(objective.computeCost(swaths), 3*((n-1)-1));


def test_fields2cover_route_snake_genSortedSwathsOdd():
  n = 100;
  swaths = f2c.Swaths();
  for i in range(1, n):
    swaths.push_back(f2c.Swath(f2c.LineString(f2c.VectorPoint(  \
        [f2c.Point(0, i), f2c.Point(1, i)])), i, i));

  swath_sorter = f2c.RP_Snake();
  objective = f2c.OBJ_DirectDistPathObj();

  swaths = swath_sorter.genSortedSwaths(swaths, 0);

  near(swaths[0].getWidth(), 1);
  near(swaths[1].getWidth(), 3);
  near(swaths[2].getWidth(), 5);
  near(swaths.back().getWidth(), 2);
  near(objective.computeCost(swaths), 3*((n-1)-1));
