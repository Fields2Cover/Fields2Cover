//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <utility>
#include "fields2cover/objectives/hg_obj/rem_area.h"
#include "fields2cover/types.h"

TEST(fields2cover_obj_remArea, computeCost) {
  f2c::obj::RemArea rem_area;

  F2CCell total_field(F2CLinearRing(
    {F2CPoint(-2,-2), F2CPoint(6,-2), F2CPoint(6,6), F2CPoint(-2,6), F2CPoint(-2,-2)}));
  F2CCell field(F2CLinearRing(
    {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,4), F2CPoint(0,4), F2CPoint(0,0)}));

  EXPECT_EQ(rem_area.computeCost(total_field, field), 0.25);
  EXPECT_EQ(rem_area.computeCostWithMinimizingSign(total_field, field), -0.25);
  EXPECT_EQ(rem_area.computeCost(F2CCells(total_field), field), 0.25);
  EXPECT_EQ(rem_area.computeCostWithMinimizingSign(F2CCells(total_field), field), -0.25);
  EXPECT_EQ(rem_area.computeCost(total_field, F2CCells(field)), 0.25);
  EXPECT_EQ(rem_area.computeCostWithMinimizingSign(total_field, F2CCells(field)), -0.25);
  EXPECT_EQ(rem_area.computeCost(F2CCells(total_field), F2CCells(field)), 0.25);
  EXPECT_EQ(rem_area.computeCostWithMinimizingSign(F2CCells(total_field), F2CCells(field)), -0.25);
}

TEST(fields2cover_obj_remArea, params_check) {
  f2c::obj::RemArea objective;
  EXPECT_TRUE(objective.isMaximizing());
  EXPECT_FALSE(objective.isMinimizing());
}

