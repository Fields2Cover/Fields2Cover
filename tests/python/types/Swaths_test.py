#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_types_swaths_append():
  ring = f2c.LinearRing();
  ring.addPoint(-10, 0); 
  ring.addPoint(-10, 20); 
  ring.addPoint(10, 20); 
  ring.addPoint(10, 0); 
  ring.addPoint(-10, 0); 
  polygon = f2c.Cell();
  polygon.addRing(ring);
  polys = f2c.Cells();
  polys.addGeometry(polygon);
  line = f2c.LineString();
  line.addPoint(-20, -10); 
  line.addPoint(-5, 5); 

  swaths = f2c.Swaths();
  near(swaths.size(), 0);
  swaths.append(line, polys, 1.0);
  near(swaths.size(), 1);
  swaths.append(line, polys, 2.0);
  near(swaths.size(), 2);
  line.addPoint(-19.9, -10); 
  line.addPoint(-5, 5.1); 
  swaths.append(line, polys, 3.0);
  near(swaths.size(), 4);
  lines = f2c.MultiLineString();
  lines.addGeometry(line);
  lines.addGeometry(f2c.LineString(f2c.VectorPoint([f2c.Point(-10,0),f2c.Point(10,20)])));
  swaths.append(lines, polygon, 4);
  near(swaths.size(), 8);
  swaths.append(lines, polys, 5);
  near(swaths.size(), 12);
  lines.addGeometry(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(1e5, 1e5),f2c.Point(1e5+1, 1e5+1)])));
  swaths.append(lines, polys, 6);
  near(swaths.size(), 16);


def test_fields2cover_types_swaths_sort():
  n = 5;
  swaths = f2c.Swaths();
  for i in range(n - 1, 0, -1):
    swaths.emplace_back(f2c.LineString(f2c.VectorPoint(
        [f2c.Point(i, i), f2c.Point(i, i + 1)])), i, i);
  swaths.sort();
  for i in range(1, n):
    near(swaths[i-1].getWidth(), i);


def test_fields2cover_types_swaths_reverseDirOddSwaths():
  n = 10;
  swaths = f2c.Swaths();
  for i in range(1, n + 1):
    swaths.emplace_back(f2c.LineString(f2c.VectorPoint(
            [f2c.Point(i, i), f2c.Point(i, i + 1)])), i);
  swaths.reverseDirOddSwaths();
  for i in range(1, n):
    assert not (swaths[i].hasSameDir(swaths[i-1]));





