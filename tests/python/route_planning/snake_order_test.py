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


def _two_cell_swaths():
  sbc = f2c.SwathsByCells();
  for c in range(2):
    swaths = f2c.Swaths();
    for i in range(3):
      swaths.append(f2c.LineString(f2c.VectorPoint(  \
          [f2c.Point(10*c + i, 0), f2c.Point(10*c + i, 5)])), 1);
    sbc.emplace_back(swaths);
  return sbc;


def test_fields2cover_route_snake_genSortedSwathsByCells():
  sbc = _two_cell_swaths();
  swath_sorter = f2c.RP_Snake();

  sorted_swaths = swath_sorter.genSortedSwaths(sbc);

  assert sorted_swaths.size() == 2;
  for c in range(sorted_swaths.size()):
    assert sorted_swaths.at(c).size() == 3;
    for i in range(sorted_swaths.at(c).size()):
      assert abs(sorted_swaths.at(c).at(i).startPoint().getX() - 10*c) <= 2;


def test_fields2cover_route_snake_genRoute():
  cell = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint(  \
      [f2c.Point(-5, -5), f2c.Point(20, -5), f2c.Point(20, 10),  \
       f2c.Point(-5, 10), f2c.Point(-5, -5)]))));
  sbc = _two_cell_swaths();
  swath_sorter = f2c.RP_Snake();

  route = swath_sorter.genRoute(cell, sbc);

  assert not route.isEmpty();
  assert route.sizeVectorSwaths() == route.sizeConnections();
  assert route.length() > 0;


def test_fields2cover_route_flatten_gives_unique_ids():
  flat = _two_cell_swaths().flatten();
  assert [flat.at(i).getId() for i in range(flat.size())] == [0, 1, 2, 3, 4, 5];
