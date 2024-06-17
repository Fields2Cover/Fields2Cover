//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/utils/spline.h"

TEST(fields2cover_utils_spline, init) {
  std::vector<double> x {1, 2, 4, 5};
  std::vector<double> y1 {1, 2, 4, 5};
  std::vector<double> y2 {-10, -20, -40, -50};
  std::vector<double> y3 {0, 1, 1, 0};

  f2c::CubicSpline cs1(x, y1);
  EXPECT_NEAR(cs1(3), 3.0, 1e-3);

  f2c::CubicSpline cs2(x, y2);
  EXPECT_NEAR(cs2(3), -30.0, 1e-3);

  f2c::CubicSpline cs3(x, y3);
  EXPECT_GT(cs3(3), 1.0);
  EXPECT_LT(cs3(3), 2.0);
  EXPECT_NEAR(cs3(1.5), cs3(1.5), 1e-3);
}


