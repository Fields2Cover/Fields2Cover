//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <utility>
#include "fields2cover/objectives/decomp_obj/min_sum_altitude.h"
#include "fields2cover/types.h"

TEST(fields2cover_obj_minSumAltitude, computeCost) {
  f2c::obj::MinSumAltitude obj;

  F2CCell square_field(F2CLinearRing(
    {F2CPoint(0,0), F2CPoint(0,20), F2CPoint(10,20), F2CPoint(10,0), F2CPoint(0,0)}));
  EXPECT_EQ(obj.computeCost(square_field), 10);
  auto rot_field = F2CPoint(-5, 5).rotateFromPoint(M_PI/7.0, square_field);

  EXPECT_NEAR(obj.computeCostWithMinimizingSign(square_field), 10, 1e-3);
  EXPECT_NEAR(obj.computeCostWithMinimizingSign(rot_field), 10, 1e-3);
}

TEST(fields2cover_obj_minSumAltitude, params_check) {
  f2c::obj::MinSumAltitude objective;
  EXPECT_FALSE(objective.isMaximizing());
  EXPECT_TRUE(objective.isMinimizing());
}

