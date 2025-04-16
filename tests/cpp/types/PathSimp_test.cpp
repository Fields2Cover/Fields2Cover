//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "../test_helpers/path_planning_checker.hpp"


TEST(fields2cover_types_pathSimp, toPath) {
  f2c::types::PathSimp path_simp1;
  path_simp1.appendClothoid(5, 0.1);
  path_simp1.states[0].start.p = F2CPoint(1, 2);

  F2CPath path1 = path_simp1.toPath(0.1, 2.0);
  path1.getStates().pop_back();
  for (auto&& s : path1) {
    EXPECT_NEAR(s.velocity, 2.0, 1e-7);
    EXPECT_NEAR(s.len, 0.1, 1e-4);
    EXPECT_EQ(s.dir, f2c::types::PathDirection::FORWARD);
  }
  EXPECT_TRUE(isPathCorrect(path1));

  f2c::types::PathSimp path_simp2;
  path_simp2.appendClothoid(-5, 0.1);
  path_simp2.states[0].start.p = F2CPoint(1, 2);

  F2CPath path2 = path_simp2.toPath(0.2, 4.0);
  path2.getStates().pop_back();
  for (auto&& s : path2) {
    EXPECT_NEAR(s.velocity, 4.0, 1e-7);
    EXPECT_NEAR(s.len, 0.2, 1e-4);
    EXPECT_EQ(s.dir, f2c::types::PathDirection::BACKWARD);
  }
  EXPECT_TRUE(isPathCorrect(path2));
}

TEST(fields2cover_types_pathSimp, append) {
  f2c::types::PathSimp path_simp;
  path_simp.appendClothoid(5, 0.01);
  path_simp.appendCircle(10);
  path_simp.appendClothoid(5, -0.1);
  auto path = path_simp.toPath();
  EXPECT_TRUE(isPathCorrect(path));
  EXPECT_EQ(path_simp.size(), 3);
  EXPECT_NEAR(path_simp.atEnd().p.getX(), 18.706, 1e-2);
}

TEST(fields2cover_types_pathSimp, prependStraightLine) {
  f2c::types::PathSimp path_simp;
  path_simp.appendClothoid(5, 0.01);
  auto x = path_simp.atEnd().p.getX();
  auto y = path_simp.atEnd().p.getY();
  EXPECT_EQ(path_simp.size(), 1);

  path_simp.prependStraightLine(100, true);

  EXPECT_EQ(path_simp.size(), 2);
  EXPECT_EQ(path_simp.states[0].type, f2c::types::SectionSimp::STRAIGHT);
  EXPECT_EQ(path_simp.states[0].len, 100);
  EXPECT_EQ(path_simp.states[1].type, f2c::types::SectionSimp::CLOTHOID);
  EXPECT_EQ(path_simp.atStart().p.getX(), -100);
  EXPECT_EQ(path_simp.atStart().p.getY(), 0);
  EXPECT_EQ(path_simp.atEnd().p.getX(), x);
  EXPECT_EQ(path_simp.atEnd().p.getY(), y);
}


TEST(fields2cover_types_pathSimp, appendStraightLine) {
  f2c::types::PathSimp path_simp;
  path_simp.appendStraightLine(10, true);
  EXPECT_EQ(path_simp.atEnd().p.getX(), 10);
  EXPECT_EQ(path_simp.atEnd().p.getY(), 0);
  EXPECT_EQ(path_simp.atEnd().theta, 0);
  EXPECT_EQ(path_simp.atEnd().kurv, 0);
  EXPECT_EQ(path_simp.states[0].using_impl, true);
  auto path = path_simp.toPath();
  EXPECT_TRUE(isPathCorrect(path));
  EXPECT_EQ(path.length(), 10.);
  EXPECT_NEAR(path.length(), path_simp.length(), 1e-2);
}

TEST(fields2cover_types_pathSimp, appendCircle) {
  f2c::types::PathSimp path_simp;
  path_simp.appendCircleKurv(.5 * M_PI * 3., 1. / 3.);
  EXPECT_NEAR(path_simp.atEnd().p.getX(), 3, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().p.getY(), 3, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().theta, .5 * M_PI, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().kurv, 1./3., 1e-7);
  auto path = path_simp.toPath();
  EXPECT_TRUE(isPathCorrect(path));

  path_simp.appendCircle(M_PI * 3.);
  EXPECT_NEAR(path_simp.atEnd().p.getX(), -3, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().p.getY(), 3, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().theta, -.5 * M_PI, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().kurv, 1./3., 1e-7);
  path = path_simp.toPath();
  EXPECT_TRUE(isPathCorrect(path));
  EXPECT_NEAR(path.length(), 1.5 * M_PI * 3., 5e-2);
  EXPECT_NEAR(path.length(), path_simp.length(), 3e-2);
}

TEST(fields2cover_types_pathSimp, appendClothoid) {
  f2c::types::PathSimp path_simp;
  double L = 3.;
  double sigma = 0.1;
  double k = L * sigma;
  double expected_ang = k * k / (2. * sigma);

  path_simp.appendClothoid(L, sigma);
  EXPECT_NEAR(path_simp.atEnd().p.getX(), 2.93981687, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().p.getY(), 0.44353286, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().theta, expected_ang, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().kurv, k, 1e-7);
  path_simp.appendClothoid(L, -0.1);
  EXPECT_NEAR(path_simp.atEnd().kurv, 0.0, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().theta, 2. * expected_ang, 1e-7);
  auto path = path_simp.toPath();
  EXPECT_TRUE(isPathCorrect(path));
  EXPECT_NEAR(path.length(), 2. * L, 1e-4);
  EXPECT_NEAR(path.length(), path_simp.length(), 6e-2);
}

TEST(fields2cover_types_pathSimp, appendBuffCloth) {
  f2c::types::PathSimp path_simp;
  double D = 8.;
  double L = 3.;
  double sigma = 0.1;
  double k = L * sigma;
  double expected_ang = k * k / (2. * sigma);
  double expected_k = k / (k * D + 1.);

  path_simp.appendBuffCloth(L, sigma, D);
  EXPECT_NEAR(path_simp.atEnd().p.getX(), 6.41954115, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().p.getY(), 1.23995604, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().theta, expected_ang, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().kurv, expected_k, 1e-7);
  auto path = path_simp.toPath();
  EXPECT_TRUE(isPathCorrect(path));
  EXPECT_NEAR(path_simp.length(), L + expected_ang * D, 6e-2);
  EXPECT_NEAR(path.length(), path_simp.length(), 6e-2);

  path_simp.appendBuffCloth(L, -sigma, D);
  auto path2 = path_simp.toPath();
  EXPECT_TRUE(isPathCorrect(path2));
  EXPECT_NEAR(path2.length(), 2.0 * path.length(), 6e-2);
  EXPECT_NEAR(path_simp.atEnd().theta, 2. * expected_ang, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().kurv, 0, 1e-7);
  EXPECT_NEAR(path2.length(), path_simp.length(), 6e-2);

  path_simp.appendBuffCloth(L, -sigma, D);
  auto path3 = path_simp.toPath();
  EXPECT_TRUE(isPathCorrect(path3));
  EXPECT_NEAR(path3.length(), 3.0 * path.length(), 6e-2);
  EXPECT_NEAR(path_simp.atEnd().theta, expected_ang, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().kurv, -expected_k, 1e-7);
  EXPECT_NEAR(path3.length(), path_simp.length(), 2e-1);
}



TEST(fields2cover_types_pathSimp, appendSmoothCircularSegment) {
  double D = 8.;
  double ang = M_PI;
  double k = 1.;
  double sigma = 0.1;
  double expected_ang = k * k / (2. * sigma);
  double expected_k = k / (k * D + 1.);

  f2c::types::PathSimp path_simp;
  path_simp.appendSmoothCircularSegment(ang, k, sigma);
  EXPECT_NEAR(path_simp.atEnd().p.getX(), 0, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().p.getY(), 4.912877343, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().theta, ang, 1e-7);
  EXPECT_NEAR(path_simp.atEnd().kurv, 0, 1e-7);
  auto path = path_simp.toPath();
  EXPECT_TRUE(isPathCorrect(path));
  EXPECT_GT(path.length(), ang / k);
  EXPECT_NEAR(path.length(), path_simp.length(), 1e-2);

  f2c::types::PathSimp path_simp_D;
  path_simp_D.appendSmoothCircularSegment(ang, k, sigma, D);
  EXPECT_NEAR(path_simp_D.atEnd().p.getX(), 0, 1e-7);
  EXPECT_NEAR(path_simp_D.atEnd().p.getY(), 2. * D + 4.912877343, 1e-7);
  EXPECT_NEAR(path_simp_D.atEnd().theta, ang, 1e-7);
  EXPECT_NEAR(path_simp_D.atEnd().kurv, 0, 1e-7);
  auto path_D = path_simp_D.toPath();
  EXPECT_TRUE(isPathCorrect(path_D));
  EXPECT_NEAR(path_D.length(), path.length() + ang * D, 6e-2);

  f2c::types::PathSimp path_simp2;
  path_simp2.appendSmoothCircularSegment(M_PI * 0.25, 1.0, 10.0);
  f2c::types::Path test_path = path_simp2.toPath();
  EXPECT_TRUE(isPathCorrect(test_path));
}


TEST(fields2cover_types_pathSimp, setImplOnOff) {
  f2c::types::PathSimp path_simp;
  path_simp.appendSmoothCircularSegment(30, 1, 0.1);

  EXPECT_EQ(path_simp.size(), 4);
  path_simp.setImplOn();
  EXPECT_EQ(path_simp.size(), 4);
  for (auto&& s : path_simp.states) {
    EXPECT_TRUE(s.using_impl);
  }
  path_simp.setImplOff();
  EXPECT_EQ(path_simp.size(), 4);
  for (auto&& s : path_simp.states) {
    EXPECT_FALSE(s.using_impl);
  }
  path_simp.setImplOn();
  EXPECT_EQ(path_simp.size(), 4);
  for (auto&& s : path_simp.states) {
    EXPECT_TRUE(s.using_impl);
  }
}

TEST(fields2cover_types_pathSimp, at) {
  f2c::types::PathSimp path_simp;
  path_simp.appendStraightLine(2);
  EXPECT_EQ(path_simp.atEnd().p.getX(), 2);
  path_simp.appendStraightLine(3);
  EXPECT_EQ(path_simp.atEnd().p.getX(), 5);
  path_simp.appendStraightLine(4);
  EXPECT_EQ(path_simp.atEnd().p.getX(), 9);

  EXPECT_EQ(path_simp.atStart().p.getX(), 0);
  EXPECT_EQ(path_simp.atStart().p.getY(), 0);

  for (double d = 0; d < path_simp.length(); d+=0.05) {
    EXPECT_NEAR(path_simp.at(d).p.getX(), d, 1e-5);
    EXPECT_NEAR(path_simp.at(d).p.getY(), 0, 1e-5);
  }
  path_simp.appendCircleKurv(0.5*M_PI, 1.0);
  path_simp.appendStraightLine(2);
  EXPECT_NEAR(path_simp.at(9+0.5*M_PI).p.getX(), 10, 1e-5);
  EXPECT_NEAR(path_simp.at(9+0.5*M_PI).p.getY(), 1, 1e-5);

  EXPECT_EQ(path_simp.at(0).p.getX(), 0);
  EXPECT_EQ(path_simp.at(0).p.getY(), 0);
}
