#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)

def test_fields2cover_types_route_init():
  assert (f2c.Route().getRouteAsLine().isEmpty());
  route = f2c.Route();
  route.type = 3;
  route.connections.push_back(f2c.MultiPoint(f2c.VectorPoint([f2c.Point(0, 0)])));
  swaths1 = f2c.Swaths();
  swaths1.push_back(f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0, 0), f2c.Point(1, 0)])), 4));
  route.v_swaths.push_back(swaths1);
  route.connections.push_back(f2c.MultiPoint(f2c.VectorPoint(
      [f2c.Point(1, 0), f2c.Point(1, 1)])));
  swaths2 = f2c.Swaths();
  swaths2.push_back(f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(1, 1), f2c.Point(0, 1)])), 4));
  route.v_swaths.push_back(swaths2);
  route.connections.push_back(f2c.MultiPoint());
  assert not (route.getRouteAsLine().isEmpty());
  near(route.getRouteAsLine().getLength(), 3);


