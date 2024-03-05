#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c
import math
import numpy as np

def near(a, b, error = 1e-7):
  assert abs(a - b) < error


def create_polygons_test():
  ring = f2c.LinearRing(f2c.VectorPoint([f2c.Point(-10,0), f2c.Point(-10,20), \
      f2c.Point(10,20), f2c.Point(10,0), f2c.Point(-10,0)]));
  return ring;

def test_fields2cover_utils_GeometryOp_getDims():
  polys = [create_polygons_test()];
  near(polys[0].getDimMinX(), -10);
  near(polys[0].getDimMinY(), 0);
  near(polys[0].getDimMaxX(), 10);
  near(polys[0].getDimMaxY(), 20);
  near(polys[0].getHeight(), 20);
  near(polys[0].getWidth(), 20);
  near(polys[0].getMinSafeLength(), 40);


def test_fields2cover_utils_random_getAngleRandom():
  rand = f2c.Random();
  rand_val = 0.0;
  two_pi = 2.0 * math.pi;
  for i in range(10000):
    rand_val = rand.getAngleRandom();
    assert (rand_val < two_pi);
    assert (rand_val >= 0.0);

def test_fields2cover_utils_random_getRandomDouble():
  rand = f2c.Random();
  N = 10000;
  sum_val = 0.0;
  for i in range(N):
    rand_val = rand.getRandomDouble();
    assert (rand_val < 1.0);
    assert (rand_val > 0.0);
    sum_val += rand_val;
  near(sum_val, N/2, N/10);

def test_fields2cover_utils_random_getLinearRandom():
  rand = f2c.Random();
  rand_val = 0.0;
  for i in range(10000):
    max_rand = rand.getRandomDouble();
    min_rand = rand.getRandomDouble() - 1.0;
    rand_val = rand.getRandomLinear(min_rand, max_rand);
    assert (rand_val < max_rand);
    assert (rand_val >= min_rand);

def test_fields2cover_utils_random_getExpRandom():
  rand = f2c.Random();
  rand_val = 0.0;
  for i in range(100):
    max_rand = rand.getRandomDouble() + 1.5;
    min_rand = rand.getRandomDouble() + 0.5;
    rand_val = rand.getRandomExp(min_rand, max_rand);
    assert (rand_val < max_rand);
    assert (rand_val >= min_rand);

def test_fields2cover_utils_random_getExpDistRandom():
  rand = f2c.Random();
  mean = 0.0;
  lambda_param = 1.5;
  epsilon = 0.1;
  steps = 3000

  for i in range(steps):
    mean += rand.getRandomExpDist(lambda_param);

  mean /= steps;
  assert (mean < (1/lambda_param) + epsilon);
  assert (mean >= (1/lambda_param) - epsilon);

def test_fields2cover_utils_random_getAngleDiffAbs():
  rand = f2c.Random();
  pi = math.pi;
  for i in range(1000):
    a = rand.getAngleRandom();
    b = a + 2.0 * pi * math.floor(1000.0 * rand.getRandomDouble() - 500.0);
    c = b + pi/7;
    d = b - pi/9;
    near(f2c.Point.getAngleDiffAbs(a, b), 0, 1e-7);
    near(f2c.Point.getAngleDiffAbs(b, a), f2c.Point.getAngleDiffAbs(b, a), 1e-7);
    near(f2c.Point.getAngleDiffAbs(a, c), pi/7, 1e-7);
    near(f2c.Point.getAngleDiffAbs(c, a), f2c.Point.getAngleDiffAbs(c, a), 1e-7);
    near(f2c.Point.getAngleDiffAbs(a, d), pi/9, 1e-7);
    near(f2c.Point.getAngleDiffAbs(d, a), f2c.Point.getAngleDiffAbs(d, a), 1e-7);

    ang1 = rand.getAngleRandom();
    ang2 = rand.getAngleRandom();
    near(f2c.Point.getAngleDiffAbs(ang1, ang2), f2c.Point.getAngleDiffAbs(ang2, ang1), 1e-7);


def test_fields2cover_utils_GeometryOp_getAngleFromPoints():
  pi = math.pi;
  degree = pi / 180.0;
  p1 = f2c.Point(1.0, 0.0);
  p2 = f2c.Point(0.0, 1.0);
  p3 = f2c.Point(1.0, 1.0);
  p4 = f2c.Point(-1.0, 0.0);
  p5 = f2c.Point(math.cos(20 * degree), math.sin(20 * degree));
  near(p1.getAngleFromPoints(p1, p1), 0, 0.01);
  near(p1.getAngleFromPoints(p1, p4), pi, 0.01);
  near(p1.getAngleFromPoints(p1, p3), pi / 4.0, 0.01);
  near(p1.getAngleFromPoints(p1, p2), pi / 2.0, 0.01);
  near(p2.getAngleFromPoints(p2, p1), 3 * pi / 2.0, 0.01);
  near(p1.getAngleFromPoints(p1, p5), 20.0 * degree, 0.01);

def test_fields2cover_utils_GeometryOp_getPointFromAngle():
  pi = math.pi;
  p = f2c.Point(0.0, 1.0);
  ref = f2c.Point(1.0, 1.0);
  res = p.getPointFromAngle(pi/4.0, math.sqrt(2.0));
  near(res.getX(), ref.getX(), 0.01);

def test_fields2cover_utils_GeometryOp_getLinesInPolygons():
  ring = create_polygons_test();
  polys = f2c.Cells();
  polygon = f2c.Cell();
  polygon.addRing(ring);
  polys.addGeometry(polygon);
  line = f2c.LineString();
  line.addPoint(-20, -10);
  line.addPoint(-5, 5);

  lines = f2c.MultiLineString();
  lines.addGeometry(line);

  reduced_line = polygon.getLinesInside(line);
  assert (polys.crosses(line));
  near(reduced_line.length(), math.sqrt(2.0)*5, 0.01);

  reduced_line = polygon.getLinesInside(lines);
  assert (polygon.crosses(lines));
  near(reduced_line.length(), math.sqrt(2.0)*5, 0.01);

  path = polys.getLinesInside(line);
  near(path.length(), math.sqrt(2.0)*5, 0.01);
  line.addPoint(20, 30);
  reduced_line = polys.getLinesInside(line);
  near(reduced_line.length(), math.sqrt(2.0)*20, 0.01);

  lines2 = f2c.MultiLineString();
  lines2.addGeometry(line);
  reduced_line = polys.getLinesInside(lines2);
  near(reduced_line.length(), math.sqrt(2.0)*20, 0.01);

def test_fields2cover_utils_GeometryOp_mod2pi():
  for i in np.arange(0.01, 100.0, 0.1):
    ang = i;
    near((ang % (2.0 * math.pi)), f2c.Point.mod_2pi(ang), 1e-3);
    near(i, ang);
    near(((1e5 * 2.0 * math.pi - i) % (2.0 * math.pi)), f2c.Point.mod_2pi(-i), 1e-3);


def test_fields2cover_utils_GeometryOp_isConvex():
  rand = f2c.Random(4);
  n = 100;
  field = rand.generateRandField(1e2, 3);
  poly = field.getField().getCell(0);
  convex_fields = 0;
  non_convex_fields = 0;
  for i in range(n):
    field = rand.genConvexField(1e2);
    if (field.getField().isConvex()):
      convex_fields += 1;
  assert (convex_fields/n > 0.9999);

  for i in range(n):
    field = rand.genNonConvexField(1e2);
    if (not field.getField().isConvex()):
      non_convex_fields += 1;
  assert (non_convex_fields/n > 0.9999);

  assert not (f2c.Cell().isConvex());

def test_fields2cover_utils_GeometryOp_rotateFromPoint():
  p_in = f2c.Point(1, 0);
  p_out = f2c.Point(1, 0);
  pi = math.pi;
  p_out = f2c.Point(0,0).rotateFromPoint(pi/2.0, p_out);
  near(p_out.getX(), 0, 1e-7);
  near(p_out.getY(), 1, 1e-7);
  p_out = f2c.Point(0,0).rotateFromPoint(pi/2.0, p_out);
  near(p_out.getX(), -1, 1e-7);
  near(p_out.getY(), 0, 1e-7);
  p_out = f2c.Point(0,0).rotateFromPoint(pi/2.0, p_out);
  near(p_out.getX(), 0, 1e-7);
  near(p_out.getY(), -1, 1e-7);
  p_out = f2c.Point(0,0).rotateFromPoint(pi/2.0, p_out);
  near(p_out.getX(), 1, 1e-7);
  near(p_out.getY(), 0, 1e-7);
  p_out = p_in.rotateFromPoint(pi/2.0, p_out);
  near(p_out.getX(), 1, 1e-7);
  near(p_out.getY(), 0, 1e-7);
  p_out = f2c.Point(0.5, 0).rotateFromPoint(pi/2.0, p_out);
  near(p_out.getX(), 0.5, 1e-7);
  near(p_out.getY(), 0.5, 1e-7);

  line = f2c.LineString();
  line.addPoint(1, 2);
  line.addPoint(3, 4);
  p_center = f2c.Point(5, 6)
  line = p_center.rotateFromPoint(pi, line);
  p_out = line.startPoint();
  near(p_out.getX(), 9, 1e-7);
  near(p_out.getY(), 10, 1e-7);
  p_out = line.endPoint();
  near(p_out.getX(), 7, 1e-7);
  near(p_out.getY(), 8, 1e-7);


