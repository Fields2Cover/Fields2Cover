#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import math
import fields2cover as f2c

def near(a, b, error = 1e-7):
  assert abs(a - b) < error


def test_fields2cover_types_swath_hasSameDir():
  swath1 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0, 0), f2c.Point(1, 1)])), 1.0);
  swath2 = swath1.clone();
  swath2.reverse();
  assert (swath1.hasSameDir(swath1));
  assert not (swath1.hasSameDir(swath2));
  assert not (swath2.hasSameDir(swath1));

def test_fields2cover_types_swath_comparison():
  swath1 = f2c.Swath(f2c.LineString(f2c.VectorPoint([
      f2c.Point(0, 0), f2c.Point(1, 1)])), 1.0, 1);
  swath2 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0, 0), f2c.Point(2, 2)])), 1.0, 2);
  swath3 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(1, 1), f2c.Point(0, 0)])), 1.0, 3);
  swath4 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0, 0), f2c.Point(1, 1)])), 2.0, 4);
  swath5 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(1, 0), f2c.Point(2, 2)])), 1.0, 5);
  swath6 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0, 1), f2c.Point(2, 2)])), 1.0, 6);

  assert not (swath1 != swath1);
  assert (swath1 == swath1);
  assert (swath1 >= swath1);
  assert (swath1 <= swath1);
  assert not (swath1 < swath1);
  assert not (swath1 > swath1);
  assert (swath1 != swath3);
  assert not (swath1 == swath3);
  assert (swath1 != swath4);
  assert not (swath1 == swath4);
  assert not (swath1 > swath4);
  assert (swath1 < swath4);
  assert not (swath1 >= swath4);
  assert (swath1 <= swath4);
  assert (swath1 < swath5);
  assert not (swath1 > swath5);
  assert (swath1 <= swath5);
  assert not (swath1 >= swath5);
  assert (swath5 > swath1);
  assert not (swath5 < swath1);
  assert (swath5 >= swath1);
  assert not (swath5 <= swath1);
  assert (swath1 < swath6);
  assert not (swath1 > swath6);
  assert (swath1 <= swath6);
  assert not (swath1 >= swath6);
  assert (swath6 > swath1);
  assert not (swath6 < swath1);
  assert (swath6 >= swath1);
  assert not (swath6 <= swath1);



def test_fields2cover_types_swath_length():
  path1 = f2c.LineString();
  path2 = f2c.LineString();
  path3 = f2c.LineString();
  path1.addPoint( 0.0, 1.0);
  path1.addPoint( 4.0, 1.0);
  path2.addPoint( 0.0, 3.0);
  path2.addPoint( 4.0, 3.0);
  path3.addPoint( 0.0, 0.0);
  path3.addPoint( 1.0, 1.0);
  swath1 = f2c.Swath(path1);
  swath2 = f2c.Swath(path2);
  swath3 = f2c.Swath(path3);

  near(swath1.length(), 4);
  near(swath2.length(), 4);
  near(pow(swath3.length(),2), 2, 1e-7);

def test_fields2cover_types_swath_width():
  swath = f2c.Swath(5.0);
  near(swath.getWidth(), 5.0);

  swath2 = f2c.Swath(0.2);
  near(swath2.getWidth(), 0.2);

  path = f2c.LineString();
  path.addPoint(0.0, 1.0);
  path.addPoint(4.0, 1.0);
  """
  EXPECT_THROW(f2c.Swath(path, -10), std::invalid_argument);
  EXPECT_THROW(f2c.Swath(0), std::invalid_argument);
  EXPECT_THROW(f2c.Swath(-10), std::invalid_argument);
  """


def test_fields2cover_types_swath_area():
  swath_empty = f2c.Swath(f2c.LineString(), 1.0);
  assert (swath_empty.areaCovered().isEmpty());

  line = f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0, 1), f2c.Point(4, 1)]));
  swath = f2c.Swath(line, 2.0);
  cell = swath.areaCovered();
  assert (swath.getWidth() * swath.length() == cell.getGeometry(0).area());
  assert (swath.getWidth() * swath.length() >= 0.5 * swath.area());
  assert (swath.getWidth() * swath.length() <= swath.area());



def test_fields2cover_types_swath_angles():
  path1 = f2c.LineString();
  path2 = f2c.LineString();
  path3 = f2c.LineString();
  path1.addPoint( 0.0, 1.0);
  path1.addPoint( 4.0, 1.0);
  path2.addPoint( 3.0, 1.0);
  path2.addPoint( 3.0, 4.0);
  path3.addPoint( 0.0, 0.0);
  path3.addPoint( 1.0, 1.0);
  swath_empty = f2c.Swath();
  swath1 = f2c.Swath(path1);
  swath2 = f2c.Swath(path2);
  swath3 = f2c.Swath(path3);

  near(swath_empty.getInAngle(), -1.0);
  near(swath_empty.getOutAngle(), -1.0);
  near(math.fmod(swath1.getInAngle(), 2*math.pi), 0.0, 1e-5);
  near(math.fmod(swath2.getInAngle(),2*math.pi), math.pi/2, 1e-5);
  near(math.fmod(swath3.getInAngle(),2*math.pi), math.pi / 4.0, 1e-5);
  near(math.fmod(swath1.getInAngle(),2*math.pi), math.fmod(swath1.getOutAngle(),2*math.pi), 1e-5);
  near(math.fmod(swath2.getInAngle(),2*math.pi), math.fmod(swath2.getOutAngle(),2*math.pi), 1e-5);
  near(math.fmod(swath3.getInAngle(),2*math.pi), math.fmod(swath3.getOutAngle(),2*math.pi), 1e-5);

def test_fields2cover_types_swath_startAndEndPoints():
  path1 = f2c.LineString();
  path1.addPoint( -100.0, 30.0);
  path1.addPoint( 4.0, 24.2);
  path1.addPoint( 50.0, -34.2);
  swath1 = f2c.Swath(path1);

  near(swath1.startPoint().getX(), -100.0, 1e-5);
  near(swath1.startPoint().getY(), 30.0, 1e-5);
  near(swath1.endPoint().getX(), 50.0, 1e-5);
  near(swath1.endPoint().getY(), -34.2, 1e-5);

