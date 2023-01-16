#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b, error = 1e-7):
  assert abs(a - b) < error

def test_fields2cover_types_path_appendSwath():
  line1 = f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0.0, 1.0), f2c.Point(1.0, 1.0), f2c.Point(1.0, 4.0)]));
  swath1 = f2c.Swath(line1);
  path1 = f2c.Path();
  path1.appendSwath(swath1, 2.0);
  near(path1.states[0].point.getX(), 0.0);
  near(path1.states[0].point.getY(), 1.0);
  near(path1.states[0].velocity, 2.0);
  near(path1.states[0].duration, 0.5);
  near(path1.states[0].type, f2c.PathSectionType_SWATH);
  near(path1.states[1].point.getX(), 1.0);
  near(path1.states[1].point.getY(), 1.0);
  near(path1.states[1].velocity, 2.0);
  near(path1.states[1].duration, 1.5);
  near(path1.states[1].type, f2c.PathSectionType_SWATH);
  near(path1.states[2].point.getX(), 1.0);
  near(path1.states[2].point.getY(), 4.0);
  near(path1.states[2].velocity, 2.0);
  near(path1.states[2].duration, 0.0);
  near(path1.states[2].type, f2c.PathSectionType_SWATH);
  near(path1.task_time, 2.0);

def test_fields2cover_types_path_opPlusEqual():
  swath1 = f2c.Swath(f2c.LineString(f2c.VectorPoint([f2c.Point(0.0, 1.0), f2c.Point(1.0, 1.0), f2c.Point(1.0, 4.0)])));
  swath2 = f2c.Swath(f2c.LineString(f2c.VectorPoint([f2c.Point(1.0, 4.0), f2c.Point(1.0, 0.0), f2c.Point(0.0, 0.0)])));

  path1 = f2c.Path();
  path2 = f2c.Path();
  path1.appendSwath(swath1, 2.0);
  path2.appendSwath(swath2, 1.0);
  path1 += path2;
  near(path1.states[0].point.getX(), 0.0);
  near(path1.states[0].point.getY(), 1.0);
  near(path1.states[0].velocity, 2.0);
  near(path1.states[0].duration, 0.5);
  near(path1.states[0].type, f2c.PathSectionType_SWATH);
  near(path1.states[1].point.getX(), 1.0);
  near(path1.states[1].point.getY(), 1.0);
  near(path1.states[1].velocity, 2.0);
  near(path1.states[1].duration, 1.5);
  near(path1.states[1].type, f2c.PathSectionType_SWATH);
  near(path1.states[2].point.getX(), 1.0);
  near(path1.states[2].point.getY(), 4.0);
  near(path1.states[2].velocity, 2.0);
  near(path1.states[2].duration, 0.0);
  near(path1.states[2].type, f2c.PathSectionType_SWATH);
  near(path1.states[3].point.getX(), 1.0);
  near(path1.states[3].point.getY(), 4.0);
  near(path1.states[3].velocity, 1.0);
  near(path1.states[3].duration, 4.0);
  near(path1.states[3].type, f2c.PathSectionType_SWATH);
  near(path1.states[4].point.getX(), 1.0);
  near(path1.states[4].point.getY(), 0.0);
  near(path1.states[4].velocity, 1.0);
  near(path1.states[4].duration, 1.0);
  near(path1.states[4].type, f2c.PathSectionType_SWATH);
  near(path1.states[5].point.getX(), 0.0);
  near(path1.states[5].point.getY(), 0.0);
  near(path1.states[5].velocity, 1.0);
  near(path1.states[5].duration, 0.0);
  near(path1.states[5].type, f2c.PathSectionType_SWATH);
  near(path1.task_time, 7.0);
  near(path1.size(), 6);

def test_fields2cover_types_path_populateAndReduce():
  path1 = f2c.Path();
  path2 = f2c.Path();
  path1.populate(200);
  near(path1.size(), 0);
  near(path1.task_time, 0.0, 1e-6);

  swath1 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0.0, 0.0), f2c.Point(0.0, 1.0), f2c.Point(2.0, 1.0)])));
  swath2 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(3.0, 1.0), f2c.Point(3.0, 4.0), f2c.Point(1.0, 4.0)])));

  path1.appendSwath(swath1, 2.0);
  path2.appendSwath(swath2, 1.0);
  path1 += path2;

  near(path1.length(), 9);
  near(path1.task_time, 6.5, 1e-6);
  for s in path1.states:
    near(s.dir, f2c.PathDirection_FORWARD);
  path_c = path1.clone();
  path1.populate(200);
  assert (path1.length() > 9.0);
  assert (path1.length() < 2.0 * 9.0);
  near(path1.size(), 200);
  near(path1.task_time, 6.5, 1e-6);
  for s in path1.states:
    near(s.dir, f2c.PathDirection_FORWARD);
    near(s.type, f2c.PathSectionType_SWATH);
  path1.populate(10);
  assert (path1.length() > 9.0);
  assert (path1.length() < 2.0 * 9.0);
  near(path1.size(), 10);
  near(path1.states.size(), 10);
  near(path1.task_time, 6.5, 1e-6);
  for s in path1.states:
    near(s.dir, f2c.PathDirection_FORWARD);
    near(s.type, f2c.PathSectionType_SWATH);
  path1.populate(500);
  assert (path1.length() > 9.0);
  assert (path1.length() < 2.0 * 9.0);
  path1.reduce(0.1);
  assert (path1.length() > 9.0);
  assert (path1.length() < 2.0 * 9.0);
  path1.reduce(0.1);
  assert (path1.length() > 9.0);
  assert (path1.length() < 2.0 * 9.0);
  assert (path1.size() < 100);

  for s in path1.states:
    near(s.dir, f2c.PathDirection_FORWARD);
    near(s.type, f2c.PathSectionType_SWATH);

def test_fields2cover_types_path_length():
  line1 = f2c.LineString();
  line2 = f2c.LineString();
  line1.addPoint( 0.0, 1.0);
  line1.addPoint( 1.0, 1.0);
  line1.addPoint( 1.0, 4.0);
  line2.addPoint( 1.0, 4.0);
  line2.addPoint( 1.0, 0.0);
  line2.addPoint( 0.0, 0.0);
  swath1 = f2c.Swath(line1);
  swath2 = f2c.Swath(line2);

  path1 = f2c.Path();
  path2 = f2c.Path();
  path1.appendSwath(swath1, 2.0);
  near(path1.length(), 4.0);
  path2.appendSwath(swath2, 1.0);
  path1 += path2;
  near(path1.length(), 9.0);
  path1 = path2;
  near(path1.length(), 5.0);

def test_fields2cover_types_path_saveLoad():
  path = f2c.Path();

  state = f2c.PathState();
  state.point = f2c.Point(3.124, -4.5, 3);
  state.angle = -0.5;
  state.velocity = -2.5;
  state.duration = 2;
  state.dir = f2c.PathDirection_FORWARD;
  state.type = f2c.PathSectionType_SWATH;
  path.states.push_back(state);

  state = f2c.PathState();
  state.point = f2c.Point(-2.3, 5);
  state.angle = 0.1;
  state.velocity = 4.5;
  state.duration = 6;
  state.dir = f2c.PathDirection_BACKWARD;
  state.type = f2c.PathSectionType_SWATH;
  path.states.push_back(state);

  assert (path.serializePath() == "3.124 -4.5 3 -0.5 -2.5 2 1 1\n-2.3 5 0 0.1 4.5 6 -1 1\n");
  path.saveToFile("/tmp/test_path");
  path_read = f2c.Path();
  path_read.loadFile("/tmp/test_path");
  assert (path.serializePath() == path_read.serializePath());

def test_fields2cover_types_path_list_points():
  line1 = f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0.0, 1.0), f2c.Point(1.0, 1.0), f2c.Point(1.0, 4.0)]));
  swath1 = f2c.Swath(line1);
  path = f2c.Path();
  path.appendSwath(swath1, 2.0);

  n = path.states.size()
  points = [path.states[i].point for i in range(n)]

  near(n, 3)
  near(path.states[0].point.getX(), 0.0);
  near(path.states[0].point.getY(), 1.0);
  near(path.states[0].velocity, 2.0);
  near(path.states[0].duration, 0.5);
  near(path.states[0].type, f2c.PathSectionType_SWATH);
  near(path.states[1].point.getX(), 1.0);
  near(path.states[1].point.getY(), 1.0);
  near(path.states[1].velocity, 2.0);
  near(path.states[1].duration, 1.5);
  near(path.states[1].type, f2c.PathSectionType_SWATH);
  near(path.states[2].point.getX(), 1.0);
  near(path.states[2].point.getY(), 4.0);
  near(path.states[2].velocity, 2.0);
  near(path.states[2].duration, 0.0);
  near(path.states[2].type, f2c.PathSectionType_SWATH);





  states = list(path.states)
  points = [item.point for item in states]

  near(path.states[0].point.getX(), 0.0);
  near(path.states[0].point.getY(), 1.0);
  near(path.states[0].velocity, 2.0);
  near(path.states[0].duration, 0.5);
  near(path.states[0].type, f2c.PathSectionType_SWATH);
  near(path.states[1].point.getX(), 1.0);
  near(path.states[1].point.getY(), 1.0);
  near(path.states[1].velocity, 2.0);
  near(path.states[1].duration, 1.5);
  near(path.states[1].type, f2c.PathSectionType_SWATH);
  near(path.states[2].point.getX(), 1.0);
  near(path.states[2].point.getY(), 4.0);
  near(path.states[2].velocity, 2.0);
  near(path.states[2].duration, 0.0);
  near(path.states[2].type, f2c.PathSectionType_SWATH);


