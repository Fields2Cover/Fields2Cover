#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_types_multilinestring_init():
  lines = f2c.MultiLineString();
  for i in range(3):
    line = f2c.LineString();
    for j in range(5):
      line.addPoint(f2c.Point(i, j, -i * j));
    lines.addGeometry(line);

  for i in range(lines.size()):
    l = lines.getGeometry(i);
    for j in range(5 - i):
      print(i,j)
      near(l.getX(j), i);
      near(l.getY(j), j);
      near(l.getZ(j), -i * j);

def test_fields2cover_types_multilinestring_getGeometry():
  lines = f2c.MultiLineString();
  for i in range(3):
    line = f2c.LineString();
    for j in range(i, 5):
      line.addPoint(f2c.Point(0, j));
    lines.addGeometry(line);

  const_lines = lines.clone();
  for i in range(lines.size()):
    near(lines.getGeometry(i).size(), 5-i);
    near(lines.getGeometry(i).length(), 5-i-1);
    near(const_lines.getGeometry(i).size(), 5-i);
    near(const_lines.getGeometry(i).length(), 5-i-1);




