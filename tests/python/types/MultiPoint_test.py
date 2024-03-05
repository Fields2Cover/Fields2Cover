#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_types_multipoint_init():
  ps1 = f2c.MultiPoint();
  near(ps1.size(), 0);
  ps1.addPoint(1,1);
  ps1.addPoint(2,1);
  near(ps1.size(), 2);
  ps2 = f2c.MultiPoint(f2c.VectorPoint(
      [f2c.Point(1,2), f2c.Point(3,2), f2c.Point(1,3)]));
  near(ps2.size(), 3);

  ps3 = f2c.MultiPoint();
  for i in range(10):
    ps3.addPoint( i, -i, 3 * i);
  near(ps3.size(), 10);
  i = 0;
  for i in range(ps3.size()):
    p = ps3.getGeometry(i);
    near(p.getX(), i);
    near(p.getY(), -i);
    near(p.getZ(), 3*i);


def test_fields2cover_types_multipoint_getGeometry():
  ps1 = f2c.MultiPoint(f2c.VectorPoint([
      f2c.Point(1,2), f2c.Point(3,2,5), f2c.Point(1,3), f2c.Point(9,4, 0.2)]));
  near(ps1.size(), 4);
  near(ps1.getGeometry(0).getX(), 1);
  near(ps1.getGeometry(0).getY(), 2);
  near(ps1.getGeometry(0).getZ(), 0);
  near(ps1.getGeometry(3).getX(), 9);
  near(ps1.getGeometry(3).getY(), 4);
  near(ps1.getGeometry(3).getZ(), 0.2);

  ps2 = ps1.clone();
  near(ps2.size(), 4);
  near(ps2.getGeometry(1).getX(), 3);
  near(ps2.getGeometry(1).getY(), 2);
  near(ps2.getGeometry(1).getZ(), 5);
  near(ps2.getGeometry(2).getX(), 1);
  near(ps2.getGeometry(2).getY(), 3);
  near(ps2.getGeometry(2).getZ(), 0);

