#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_route_boustrophedon_genSortedSwaths():
  n = 5;
  swaths = f2c.Swaths();
  for i in range(1, n):
    swaths.push_back(f2c.Swath(f2c.LineString(f2c.VectorPoint(  \
        [f2c.Point(i, 0), f2c.Point(i, 1)])), i, i));

  swath_sorter = f2c.RP_Boustrophedon();
  objective = f2c.OBJ_DirectDistPathObj();

  swaths = swath_sorter.genSortedSwaths(swaths);
  near(swaths.size(), n - 1);
  near(swaths[0].startPoint().getY(), 0);
  for i in range(1, n):
    near(swaths[i - 1].getWidth(), i);
  near(objective.computeCost(swaths), 2*(n-1)-1);
  swaths = swath_sorter.genSortedSwaths(swaths, 1);
  near(swaths[0].startPoint().getY(), 0);
  near(swaths[0].getWidth(), n-1);
  near(swaths[1].getWidth(), n-2);
  near(swaths[2].getWidth(), n-3);
  near(objective.computeCost(swaths), 2*(n-1)-1);
  swaths = swath_sorter.genSortedSwaths(swaths, 2);
  near(swaths[0].startPoint().getY(), 1);
  near(swaths[0].getWidth(), 1);
  near(swaths[1].getWidth(), 2);
  near(swaths[2].getWidth(), 3);
  near(objective.computeCost(swaths), 2*(n-1)-1);
  swaths = swath_sorter.genSortedSwaths(swaths, 3);
  near(swaths[0].startPoint().getY(), 1);
  near(swaths[0].getWidth(), n-1);
  near(swaths[1].getWidth(), n-2);
  near(swaths[2].getWidth(), n-3);
  near(objective.computeCost(swaths), 2*(n-1)-1);
