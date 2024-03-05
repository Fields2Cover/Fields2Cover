#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c
from os.path import exists

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)

def create_polygon():
  poly = f2c.Cell();
  line = f2c.LinearRing();
  line.addPoint(-10, 0);
  line.addPoint(-10, 20);
  line.addPoint(10, 20);
  line.addPoint(10, 0);
  line.addPoint(-10, 0);
  poly.addRing(line);
  return poly;

def test_fields2cover_utils_visualizer_plotCell():
  line1 = f2c.LineString();
  line1.addPoint( 0.0, 1.0);
  line1.addPoint( 1.0, 1.0);
  line1.addPoint( 1.0, 4.0);
  swath1 = f2c.Swath(line1);
  path1 = f2c.Path();
  path1.appendSwath(swath1, 2.0);

  f2c.Visualizer.figure();
  f2c.Visualizer.plot(path1);
  f2c.Visualizer.plot(create_polygon());
  f2c.Visualizer.show();

