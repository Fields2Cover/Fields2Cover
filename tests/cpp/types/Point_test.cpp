//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

namespace f2c {

TEST(fields2cover_types_point, minus) {
  F2CPoint p1(1.1, 2.2, 3.3), p2(-0.5, 0.6, 1);
  auto p3 = p2 - p1;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(p1.getZ(), 3.3, 1e-7);
  EXPECT_NEAR(p2.getX(), -0.5, 1e-7);
  EXPECT_NEAR(p2.getY(), 0.6, 1e-7);
  EXPECT_NEAR(p2.getZ(), 1, 1e-7);
  EXPECT_NEAR(p3.getX(), -1.6, 1e-7);
  EXPECT_NEAR(p3.getY(), -1.6, 1e-7);
  EXPECT_NEAR(p3.getZ(), -2.3, 1e-7);

  auto p4 = p2 - p1;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(p1.getZ(), 3.3, 1e-7);
  EXPECT_NEAR(p2.getX(), -0.5, 1e-7);
  EXPECT_NEAR(p2.getY(), 0.6, 1e-7);
  EXPECT_NEAR(p2.getZ(), 1, 1e-7);
  EXPECT_NEAR(p4.getX(), -1.6, 1e-7);
  EXPECT_NEAR(p4.getY(), -1.6, 1e-7);
  EXPECT_NEAR(p4.getZ(), -2.3, 1e-7);
}

TEST(fields2cover_types_point, plus) {
  F2CPoint p1(1.1, 2.2, 3.3), p2(-0.5, 0.6, 1);
  auto p3 = p2 + p1;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(p1.getZ(), 3.3, 1e-7);
  EXPECT_NEAR(p2.getX(), -0.5, 1e-7);
  EXPECT_NEAR(p2.getY(), 0.6, 1e-7);
  EXPECT_NEAR(p2.getZ(), 1, 1e-7);
  EXPECT_NEAR(p3.getX(), 0.6, 1e-7);
  EXPECT_NEAR(p3.getY(), 2.8, 1e-7);
  EXPECT_NEAR(p3.getZ(), 4.3, 1e-7);
}

TEST(fields2cover_types_point, mult_equal) {
  F2CPoint p1(1.1, 2.2, 3);
  p1 *= 1.5;
  EXPECT_NEAR(p1.getX(), 1.65, 1e-7);
  EXPECT_NEAR(p1.getY(), 3.3, 1e-7);
  EXPECT_NEAR(p1.getZ(), 4.5, 1e-7);
}

TEST(fields2cover_types_point, multiply) {
  F2CPoint p1(1.1, 2.2, 3);
  auto p2 = p1 * 1.5;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(p1.getZ(), 3, 1e-7);
  EXPECT_NEAR(p2.getX(), 1.65, 1e-7);
  EXPECT_NEAR(p2.getY(), 3.3, 1e-7);
  EXPECT_NEAR(p2.getZ(), 4.5, 1e-7);
}

TEST(fields2cover_types_point, plus_v) {
  F2CPoint p1(1.1, 2.2);
  std::vector<F2CPoint> v {F2CPoint(0.0, 0.0), F2CPoint(1.0, 2.0)};
  auto p2 = v + p1;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(v[0].getX(), 0, 1e-7);
  EXPECT_NEAR(v[0].getY(), 0, 1e-7);
  EXPECT_NEAR(v[1].getX(), 1, 1e-7);
  EXPECT_NEAR(v[1].getY(), 2, 1e-7);
  EXPECT_NEAR(p2[0].getX(), 1.1, 1e-7);
  EXPECT_NEAR(p2[0].getY(), 2.2, 1e-7);
  EXPECT_NEAR(p2[1].getX(), 2.1, 1e-7);
  EXPECT_NEAR(p2[1].getY(), 4.2, 1e-7);
}

TEST(fields2cover_types_point, minus_v) {
  F2CPoint p1(1.1, 2.2);
  std::vector<F2CPoint> v {F2CPoint(0.0, 0.0), F2CPoint(1.0, 2.0)};
  auto p2 = v - p1;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(v[0].getX(), 0, 1e-7);
  EXPECT_NEAR(v[0].getY(), 0, 1e-7);
  EXPECT_NEAR(v[1].getX(), 1, 1e-7);
  EXPECT_NEAR(v[1].getY(), 2, 1e-7);
  EXPECT_NEAR(p2[0].getX(), -1.1, 1e-7);
  EXPECT_NEAR(p2[0].getY(), -2.2, 1e-7);
  EXPECT_NEAR(p2[1].getX(), -0.1, 1e-7);
  EXPECT_NEAR(p2[1].getY(), -0.2, 1e-7);

  auto p3 = p1 - v;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(v[0].getX(), 0, 1e-7);
  EXPECT_NEAR(v[0].getY(), 0, 1e-7);
  EXPECT_NEAR(v[1].getX(), 1, 1e-7);
  EXPECT_NEAR(v[1].getY(), 2, 1e-7);
  EXPECT_NEAR(p3[0].getX(), 1.1, 1e-7);
  EXPECT_NEAR(p3[0].getY(), 2.2, 1e-7);
  EXPECT_NEAR(p3[1].getX(), 0.1, 1e-7);
  EXPECT_NEAR(p3[1].getY(), 0.2, 1e-7);
}

TEST(fields2cover_types_point, clone) {
  F2CPoint a(2, 3);
  F2CPoint b = a.clone();
  a.setX(10);
	EXPECT_EQ(b.getX(),2);
}

}  // namespace f2c

