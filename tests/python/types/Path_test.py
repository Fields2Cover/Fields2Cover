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
  near(path1.points[0].getX(), 0.0);
  near(path1.points[0].getY(), 1.0);
  near(path1.velocities[0], 2.0);
  near(path1.durations[0], 0.5);
  near(path1.type[0], f2c.PathSectionType_SWATH);
  near(path1.points[1].getX(), 1.0);
  near(path1.points[1].getY(), 1.0);
  near(path1.velocities[1], 2.0);
  near(path1.durations[1], 1.5);
  near(path1.type[1], f2c.PathSectionType_SWATH);
  near(path1.points[2].getX(), 1.0);
  near(path1.points[2].getY(), 4.0);
  near(path1.velocities[2], 2.0);
  near(path1.durations[2], 0.0);
  near(path1.type[2], f2c.PathSectionType_SWATH);
  near(path1.task_time, 2.0);

def test_fields2cover_types_path_opPlusEqual():
  swath1 = f2c.Swath(f2c.LineString(f2c.VectorPoint([f2c.Point(0.0, 1.0), f2c.Point(1.0, 1.0), f2c.Point(1.0, 4.0)])));
  swath2 = f2c.Swath(f2c.LineString(f2c.VectorPoint([f2c.Point(1.0, 4.0), f2c.Point(1.0, 0.0), f2c.Point(0.0, 0.0)])));
  	
  path1 = f2c.Path();
  path2 = f2c.Path();
  path1.appendSwath(swath1, 2.0);
  path2.appendSwath(swath2, 1.0);
  path1 += path2;
  near(path1.points[0].getX(), 0.0);
  near(path1.points[0].getY(), 1.0);
  near(path1.velocities[0], 2.0);
  near(path1.durations[0], 0.5);
  near(path1.type[0], f2c.PathSectionType_SWATH);
  near(path1.points[1].getX(), 1.0);
  near(path1.points[1].getY(), 1.0);
  near(path1.velocities[1], 2.0);
  near(path1.durations[1], 1.5);
  near(path1.type[1], f2c.PathSectionType_SWATH);
  near(path1.points[2].getX(), 1.0);
  near(path1.points[2].getY(), 4.0);
  near(path1.velocities[2], 2.0);
  near(path1.durations[2], 0.0);
  near(path1.type[2], f2c.PathSectionType_SWATH);
  near(path1.points[3].getX(), 1.0);
  near(path1.points[3].getY(), 4.0);
  near(path1.velocities[3], 1.0);
  near(path1.durations[3], 4.0);
  near(path1.type[3], f2c.PathSectionType_SWATH);
  near(path1.points[4].getX(), 1.0);
  near(path1.points[4].getY(), 0.0);
  near(path1.velocities[4], 1.0);
  near(path1.durations[4], 1.0);
  near(path1.type[4], f2c.PathSectionType_SWATH);
  near(path1.points[5].getX(), 0.0);
  near(path1.points[5].getY(), 0.0);
  near(path1.velocities[5], 1.0);
  near(path1.durations[5], 0.0);
  near(path1.type[5], f2c.PathSectionType_SWATH);
  near(path1.task_time, 7.0);
  near(path1.points.size(), 6);
  near(path1.durations.size(), 6);
  near(path1.directions.size(), 6);
  near(path1.velocities.size(), 6);
  assert (path1.isValid());

def test_fields2cover_types_path_populateAndReduce():
  path1 = f2c.Path();
  path2 = f2c.Path();
  path1.populate(200);
  assert (path1.isValid());
  near(path1.velocities.size(), 0);
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
  for d in path1.directions:
    near(d, f2c.PathDirection_FORWARD);
  path_c = path1.clone();
  path1.populate(200);
  assert (path1.length() > 9.0);
  assert (path1.length() < 2.0 * 9.0);
  assert (path1.isValid());
  near(path1.points.size(), 200);
  near(path1.angles.size(), 200);
  near(path1.velocities.size(), 200);
  near(path1.durations.size(), 200);
  near(path1.directions.size(), 200);
  near(path1.type.size(), 200);
  near(path1.task_time, 6.5, 1e-6);
  for d in path1.directions:
    near(d, f2c.PathDirection_FORWARD);
  for t in path1.type:
    near(t, f2c.PathSectionType_SWATH);
  path1.populate(10);
  assert (path1.isValid());
  assert (path1.length() > 9.0);
  assert (path1.length() < 2.0 * 9.0);
  near(path1.points.size(), 10);
  near(path1.angles.size(), 10);
  near(path1.velocities.size(), 10);
  near(path1.durations.size(), 10);
  near(path1.directions.size(), 10);
  near(path1.type.size(), 10);
  near(path1.task_time, 6.5, 1e-6);
  for d in path1.directions:
    near(d, f2c.PathDirection_FORWARD);
  for t in path1.type:
    near(t, f2c.PathSectionType_SWATH);
  path1.populate(500);
  assert (path1.length() > 9.0);
  assert (path1.length() < 2.0 * 9.0);
  path1.reduce(0.1);
  assert (path1.length() > 9.0);
  assert (path1.length() < 2.0 * 9.0);
  path1.reduce(0.1);
  assert (path1.length() > 9.0);
  assert (path1.length() < 2.0 * 9.0);
  assert (path1.isValid());
  assert (path1.points.size() < 100);
  assert (path1.angles.size() < 100);
  assert (path1.velocities.size() < 100);
  assert (path1.durations.size() < 100);
  assert (path1.directions.size() < 100);
  assert (path1.type.size() < 100);

  for d in path1.directions:
    near(d, f2c.PathDirection_FORWARD);
  for t in path1.type:
    near(t, f2c.PathSectionType_SWATH);

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

def test_fields2cover_types_path_serialize():
  path = f2c.Path();
  assert (path.serializePath() == "");
  path.points.push_back(f2c.Point(2.3, -5));
  path.angles.push_back(0.1);
  path.velocities.push_back(3);
  path.durations.push_back(3);
  path.directions.push_back(f2c.PathDirection_FORWARD);
  path.type.push_back(f2c.PathSectionType_TURN);

  assert (path.serializePath() == "2.3 -5 0.1 3 3 1 2\n");


def test_fields2cover_types_path_saveLoad():
  path = f2c.Path();
  path_read = f2c.Path();
  path.points.push_back(f2c.Point(-2.3, 5));
  path.angles.push_back(0.1);
  path.velocities.push_back(4.5);
  path.durations.push_back(6);
  path.directions.push_back(f2c.PathDirection_BACKWARD);
  path.type.push_back(f2c.PathSectionType_SWATH);

  assert (path.serializePath() == "-2.3 5 0.1 4.5 6 -1 1\n");
  path.saveToFile("/tmp/test_path");
  path_read.loadFile("/tmp/test_path");
  assert (path.serializePath() == path_read.serializePath());


