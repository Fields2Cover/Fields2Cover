//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/utils/random.h"
#include "fields2cover/types.h"

std::vector<F2CLinearRing> create_polygons_test(void) {
  std::vector<F2CLinearRing> v_line;
  v_line.emplace_back(F2CLinearRing({F2CPoint(-10,0), F2CPoint(-10,20),
      F2CPoint(10,20), F2CPoint(10,0), F2CPoint(-10,0)}));
  return v_line;
}

TEST(fields2cover_utils_GeometryOp, get_dims) {
  auto polys = create_polygons_test();
  auto poly = polys[0];
  EXPECT_EQ(polys[0].getDimMinX(), -10);
  EXPECT_EQ(polys[0].getDimMinY(), 0);
  EXPECT_EQ(polys[0].getDimMaxX(), 10);
  EXPECT_EQ(polys[0].getDimMaxY(), 20);
  EXPECT_EQ(polys[0].getHeight(), 20);
  EXPECT_EQ(polys[0].getWidth(), 20);
  EXPECT_EQ(polys[0].getMinSafeLength(), 40);
}


TEST(fields2cover_utils_GeometryOp, get_angle_random) {
  f2c::Random rand;
  auto rand_val {0.0};
  auto two_pi = boost::math::constants::two_pi<double>();
  for (int i = 0; i < 10000; ++i) {
    rand_val = rand.getAngleRandom();
    EXPECT_LT( rand_val, two_pi);
    EXPECT_GE( rand_val, 0.0);
  }
}

TEST(fields2cover_utils_GeometryOp, get_random_double) {
  f2c::Random rand;
  const int N = 10000;
  double sum_val {0.0};
  for (int i = 0; i < N; ++i) {
    auto rand_val = rand.getRandomDouble();
    EXPECT_LT( rand_val, 1.0);
    EXPECT_GT( rand_val, 0.0);
    sum_val += rand_val; 
  }
  EXPECT_NEAR(sum_val, N/2, N/10);

}

TEST(fields2cover_utils_GeometryOp, get_linear_random) {
  f2c::Random rand;
  auto rand_val {0.0};
  for (int i = 0; i < 10000; ++i) {
    double max_rand = rand.getRandomDouble();
    double min_rand = rand.getRandomDouble() - 1.0;
    rand_val = rand.getRandomLinear(min_rand, max_rand);
    EXPECT_LT( rand_val, max_rand);
    EXPECT_GE( rand_val, min_rand);
  }
}

TEST(fields2cover_utils_GeometryOp, get_exp_random) {
  f2c::Random rand;
  auto rand_val {0.0};
  for (int i = 0; i < 100; ++i) {
    double max_rand = rand.getRandomDouble() + 1.5;
    double min_rand = rand.getRandomDouble() + 0.5;

    rand_val = rand.getRandomExp(min_rand, max_rand);
    EXPECT_LT( rand_val, max_rand);
    EXPECT_GE( rand_val, min_rand);
  }
}

TEST(fields2cover_utils_GeometryOp, get_exp_distribution_random) {
  f2c::Random rand;
  auto mean {0.0};
  auto const steps{1000};
  auto const lambda{1.5};
  auto const epsilon{0.05};

  for (int i = 0; i < steps; ++i) {
    mean += rand.getRandomExpDist(lambda);
  }
  mean /= steps;
  EXPECT_LT( mean, (1/lambda) + epsilon);
  EXPECT_GE( mean, (1/lambda) - epsilon);
}


TEST(fields2cover_utils_GeometryOp, get_angle_diff_abs) {
  f2c::Random rand;
  const double pi = boost::math::constants::pi<double>();
  for (int i = 0; i < 1000; ++i) {
    double a = rand.getAngleRandom();
    double b = a + 2.0 * pi * static_cast<int>(1000.0 * rand.getRandomDouble() - 500.0);
    double c = b + pi/7;
    double d = b - pi/9;
    EXPECT_NEAR( F2CPoint::getAngleDiffAbs(a, b), 0, 1e-7);
    EXPECT_NEAR( F2CPoint::getAngleDiffAbs(b, a), F2CPoint::getAngleDiffAbs(b, a), 1e-7);
    EXPECT_NEAR( F2CPoint::getAngleDiffAbs(a, c), pi/7, 1e-7);
    EXPECT_NEAR( F2CPoint::getAngleDiffAbs(c, a), F2CPoint::getAngleDiffAbs(c, a), 1e-7);
    EXPECT_NEAR( F2CPoint::getAngleDiffAbs(a, d), pi/9, 1e-7);
    EXPECT_NEAR( F2CPoint::getAngleDiffAbs(d, a), F2CPoint::getAngleDiffAbs(d, a), 1e-7);

    auto ang1 = rand.getAngleRandom();
    auto ang2 = rand.getAngleRandom();
    EXPECT_NEAR( F2CPoint::getAngleDiffAbs(ang1, ang2), F2CPoint::getAngleDiffAbs(ang2, ang1), 1e-7);
  }
}


TEST(fields2cover_utils_GeometryOp, get_angle_from_points) {
  auto degree = boost::math::constants::degree<double>();
  auto pi = boost::math::constants::pi<double>();
  F2CPoint p1(1.0, 0.0);
  F2CPoint p2(0.0, 1.0);
  F2CPoint p3(1.0, 1.0);
  F2CPoint p4(-1.0, 0.0);
  F2CPoint p5(cos(20 * degree), sin(20 * degree));
  EXPECT_NEAR(p1.getAngleFromPoints(p1), 0, 0.01);
  EXPECT_NEAR(p1.getAngleFromPoints(p4), pi, 0.01);
  EXPECT_NEAR(p1.getAngleFromPoints(p3), pi / 4.0, 0.01);
  EXPECT_NEAR(p1.getAngleFromPoints(p2), pi / 2.0, 0.01);
  EXPECT_NEAR(p2.getAngleFromPoints(p1), 3 * pi / 2.0, 0.01);
  EXPECT_NEAR(p1.getAngleFromPoints(p5), 20.0 * degree, 0.01);
}

TEST(fields2cover_utils_GeometryOp, get_point_from_angle) {
  auto pi = boost::math::constants::pi<double>();
  F2CPoint p(0.0, 1.0);
  F2CPoint ref(1.0, 1.0);
  F2CPoint res = p.getPointFromAngle(pi/4.0, sqrt(2.0));
  EXPECT_NEAR(res.getX(), ref.getX(), 0.01);
}

TEST(fields2cover_utils_GeometryOp, getLinesInPolygons) {
  auto rings = create_polygons_test();
  F2CCells polys;
  F2CCell polygon;
  polygon.addRing(rings[0]);
  polys.addGeometry(polygon);
  F2CLineString line;
  line->addPoint(-20, -10); 
  line->addPoint(-5, 5); 

  F2CMultiLineString lines;
  lines.addGeometry(line);

  auto reduced_line = polygon.getLinesInside(line);
  EXPECT_TRUE(polys.Crosses(line));
  EXPECT_NEAR(reduced_line.getLength(), std::sqrt(2.0)*5, 0.01);

  reduced_line = polygon.getLinesInside(lines);
  EXPECT_TRUE(polygon.Crosses(lines));
  EXPECT_NEAR(reduced_line.getLength(), std::sqrt(2.0)*5, 0.01);
  
  auto path = polys.getLinesInside(line);
  EXPECT_NEAR(path.getLength(), std::sqrt(2.0)*5, 0.01);
  line.addPoint(20, 30); 
  reduced_line = polys.getLinesInside(line);
  EXPECT_NEAR(reduced_line.getLength(), std::sqrt(2.0)*20, 0.01);

  F2CMultiLineString lines2;
  lines2.addGeometry(line);
  reduced_line = polys.getLinesInside(lines2);
  EXPECT_NEAR(reduced_line.getLength(), std::sqrt(2.0)*20, 0.01);
}

TEST(fields2cover_utils_GeometryOp, mod_2pi) {
  for (double i = 0.01; i < 100.0; i += 0.1) {
    auto ang = i;
    EXPECT_NEAR(fmod(ang, boost::math::constants::two_pi<double>()), F2CPoint::mod_2pi(ang), 1e-3);
    EXPECT_EQ(i, ang);
    EXPECT_NEAR(fmod(1e5 * boost::math::constants::two_pi<double>() - i, boost::math::constants::two_pi<double>()),
        F2CPoint::mod_2pi(-i), 1e-3);
  }
}

TEST(fields2cover_utils_GeometryOp, genRandField) {
  f2c::Random rand;
  auto field = rand.generateRandField( 4, 1e2);
  EXPECT_NEAR(field.field.getArea(), 1e2 , 1e-6);
  EXPECT_THROW(rand.generateRandField( -2, 1e2), std::invalid_argument);
  EXPECT_THROW(rand.generateRandField( 0, 1e2), std::invalid_argument);
  EXPECT_THROW(rand.generateRandField( 2, 1e2), std::invalid_argument);
  EXPECT_THROW(rand.generateRandField( 6, -1e2), std::invalid_argument);
}

TEST(fields2cover_utils_GeometryOp, isConvex) {
  f2c::Random rand(4);
  int n {100};
  auto field = rand.generateRandField( 3, 1e2);
  auto poly = field.field.getCell(0);
  int convex_fields {0}, non_convex_fields {0};
  for (int i = 0; i < n; ++i) {
    field = rand.genConvexField(1e2);
    if (field.field.isConvex()) {
      ++convex_fields;
    }
  }
  EXPECT_GT(static_cast<double>(convex_fields) / n, 0.9999);

  for (int i = 0; i < n; ++i) {
    field = rand.genNonConvexField(1e2);
    if (!field.field.isConvex()) {
      ++non_convex_fields;
    }
  }
  EXPECT_GT(static_cast<double>(non_convex_fields) / n, 0.9999);

  EXPECT_FALSE(F2CCell().isConvex());
}

TEST(fields2cover_utils_GeometryOp, rotateFromPoint) {
  F2CPoint p_in(1, 0), p_out(1, 0);
  double pi = boost::math::constants::pi<double>();
  p_out = F2CPoint(0,0).rotateFromPoint(pi/2.0, p_out);
  EXPECT_NEAR(p_out.getX(), 0, 1e-7);
  EXPECT_NEAR(p_out.getY(), 1, 1e-7);
  p_out = F2CPoint(0,0).rotateFromPoint(pi/2.0, p_out);
  EXPECT_NEAR(p_out.getX(), -1, 1e-7);
  EXPECT_NEAR(p_out.getY(), 0, 1e-7);
  p_out = F2CPoint(0,0).rotateFromPoint(pi/2.0, p_out);
  EXPECT_NEAR(p_out.getX(), 0, 1e-7);
  EXPECT_NEAR(p_out.getY(), -1, 1e-7);
  p_out = F2CPoint(0,0).rotateFromPoint(pi/2.0, p_out);
  EXPECT_NEAR(p_out.getX(), 1, 1e-7);
  EXPECT_NEAR(p_out.getY(), 0, 1e-7);
  p_out = p_in.rotateFromPoint(pi/2.0, p_out);
  EXPECT_NEAR(p_out.getX(), 1, 1e-7);
  EXPECT_NEAR(p_out.getY(), 0, 1e-7);
  p_out = F2CPoint(0.5, 0).rotateFromPoint(pi/2.0, p_out);
  EXPECT_NEAR(p_out.getX(), 0.5, 1e-7);
  EXPECT_NEAR(p_out.getY(), 0.5, 1e-7);

  F2CLineString line;
  line->addPoint(1, 2); 
  line->addPoint(3, 4); 
  line = F2CPoint(5, 6).rotateFromPoint(pi, line);
  p_out = line.StartPoint();
  EXPECT_NEAR(p_out.getX(), 9, 1e-7);
  EXPECT_NEAR(p_out.getY(), 10, 1e-7);
  p_out = line.EndPoint();
  EXPECT_NEAR(p_out.getX(), 7, 1e-7);
  EXPECT_NEAR(p_out.getY(), 8, 1e-7);
}


