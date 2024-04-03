#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_types_linestring_init():
  line1 = f2c.LineString();
  line1.addPoint(1,1);
  line1.addPoint(2,1);
  near(line1.size(), 2);
  line2 = f2c.LineString(f2c.VectorPoint([f2c.Point(1,2), f2c.Point(3,2), f2c.Point(1,3)]));
  near(line2.size(), 3);
  near(line2.getX(0), 1);
  near(line2.getX(1), 3);

def test_fields2cover_types_linestring_add():
  p1 = f2c.Point(1,2,3)
  p2 = f2c.Point(2,1,3)
  p3 = f2c.Point(0,0,0)
  line = f2c.LineString()
  line.addPoint(p2)
  line.addPoint(p3)
  line2 = line + p1
  near(line2.size(), 2);
  near(line2.getX(0), 3);
  near(line2.getX(1), 1);
  near(line2.getY(0), 3);
  near(line2.getY(1), 2);
  near(line2.getZ(0), 6);
  near(line2.getZ(1), 3);






