#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b, error = 1e-7):
  assert abs(a - b) < error


def test_fields2cover_obj_direct_dist_path_obj_computeCost():
  swath1 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0.0, 1.0), f2c.Point(4.0, 1.0)])));
  swath2 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(4.0, 3.0), f2c.Point(0.0, 3.0)])));
  swath3 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0.0, 0.0), f2c.Point(1.0, 1.0)])));

  def createRoute(s):
    route = f2c.Route();
    route.addConnection();
    route.addSwaths(s);
    route.addConnection();
    return route;

  swaths1 = f2c.Swaths();
  [swaths1.push_back(i) for i in [swath1]]
  swaths2 = f2c.Swaths();
  [swaths2.push_back(i) for i in [swath2]]
  swaths4 = f2c.Swaths();
  [swaths4.push_back(i) for i in [swath1, swath2, swath1, swath2]]

  r1 = createRoute(swaths1);
  r2 = createRoute(swaths2);
  r4 = createRoute(swaths4);

  length = f2c.OBJ_DirectDistPathObj();

  p1 = f2c.Point(0, 0);
  p2 = f2c.Point(4, 0);

  near(length.computeCost(p1, p2), 4.0);

  ps1 = f2c.VectorPoint([p1, p2]);
  ps2 = f2c.MultiPoint();
  ps2.addGeometry(p1);
  ps2.addGeometry(p2);

  near(length.computeCost(ps1), 4.0);
  near(length.computeCost(ps2), 4.0);
  near(length.computeCost(p1, p2, 0.0), 4.0);
  near(length.computeCost(p1, 0.0, p2), 4.0);
  near(length.computeCost(p1, 0.0, p2, 0.0), 4.0);
  near(length.computeCost(p1, swath2), 5.0);
  near(length.computeCost(swath2, p1), 3.0);
  near(length.computeCost(p1, 0.0, swath2), 5.0);
  near(length.computeCost(swath2, p1, 0.0), 3.0);

  near(length.computeCost(swath2, ps2), 3.0);
  near(length.computeCost(swaths2, ps2), 3.0);
  near(length.computeCost(ps2, swath2), 3.0);
  near(length.computeCost(ps2, swaths2), 3.0);

  near(length.computeCost(swath2), 4.0);
  near(length.computeCost(swaths4), 22.0);

  near(length.computeCost(f2c.MultiPoint(), swath2), 0.0);
  near(length.computeCost(swath2, f2c.MultiPoint()), 0.0);

  near(length.computeCost(r1), 4.0);
  near(length.computeCostWithMinimizingSign(r1), length.computeCost(r1));
  near(length.computeCost(r2), 4.0);
  near(length.computeCostWithMinimizingSign(r2), length.computeCost(r2));
  near(length.computeCost(r4), 22.0);
  near(length.computeCostWithMinimizingSign(r4), length.computeCost(r4));

def test_fields2cover_obj_direct_dist_path_obj_paramsCheck():
  objective = f2c.OBJ_DirectDistPathObj();

  assert not (objective.isMaximizing());
  assert (objective.isMinimizing());

