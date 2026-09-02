#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import fields2cover as f2c


def gen_cell():
  return f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
      [f2c.Point(0, 0), f2c.Point(60, 0), f2c.Point(60, 40), f2c.Point(0, 40),
       f2c.Point(0, 0)]))));


def gen_swaths(cell):
  swaths = f2c.Swaths();
  for x in range(5, 60, 10):
    swaths.append(f2c.LineString(f2c.VectorPoint(
        [f2c.Point(x, 1), f2c.Point(x, 39)])), cell, 10);
  sbc = f2c.SwathsByCells();
  sbc.push_back(swaths);
  return sbc;


def test_fields2cover_route_generator_base_every_planner_answers_the_same_call():
  cell = gen_cell();
  sbc = gen_swaths(cell);
  n_swaths = sbc.flatten().size();

  order = f2c.VectorSize([i for i in range(n_swaths)]);

  planners = [
      f2c.RP_Boustrophedon(),
      f2c.RP_Snake(),
      f2c.RP_Spiral(2),
      f2c.RP_CustomOrder(order),
      f2c.RP_RoutePlannerBase(),
  ];

  for planner in planners:
    # The point of the test: every planner is the same kind of thing on the
    # python side too, not just a class that happens to have genRoute.
    assert isinstance(planner, f2c.RP_Route_generator_base_class);
    route = planner.genRoute(cell, sbc);
    assert not route.isEmpty();
    assert route.sizeVectorSwaths() == route.sizeConnections();
