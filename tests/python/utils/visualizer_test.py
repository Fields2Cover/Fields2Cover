#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
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

def test_fields2cover_utils_visualizer_saveField():
  rand = f2c.Random(1);
  field = rand.generateRandField(5, 1e4);
  robot = f2c.Robot();
  optim = f2c.OptimizationParams();

  robot.start_point.emplace(f2c.Point(-55, -22));
  robot.end_point.emplace(f2c.Point(35, 43));
  robot.op_width = 3.0;
  optim.headland_width = 2.0 * robot.op_width;

  hl_gen = f2c.HG_Const_gen();
  no_headlands = hl_gen.generateHeadlands(field.field, optim.headland_width);
  
  swath_gen = f2c.SG_BruteForce();
  swaths = swath_gen.generateSwaths(0.1, robot.op_width, no_headlands);

  f2c.Visualizer.show(False);
  f2c.Visualizer.figure(1000);
  f2c.Visualizer.plot(field.field);
  f2c.Visualizer.plot(no_headlands);
  f2c.Visualizer.plot(swaths);
  f2c.Visualizer.plot(robot);
  f2c.Visualizer.save("/tmp/test_cell.png");
  assert exists("/tmp/test_cell.png");



def test_fields2cover_utils_visualizer_plotCell():
  line1 = f2c.LineString();
  line1.addPoint( 0.0, 1.0);
  line1.addPoint( 1.0, 1.0);
  line1.addPoint( 1.0, 4.0);
  swath1 = f2c.Swath(line1);
  path1 = f2c.Path();
  path1.appendSwath(swath1, 2.0);

  f2c.Visualizer.figure(300);
  f2c.Visualizer.plot(path1);
  f2c.Visualizer.plot(create_polygon());
  f2c.Visualizer.show(False);

