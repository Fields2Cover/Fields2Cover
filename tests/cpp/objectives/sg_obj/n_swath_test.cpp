//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/objectives/sg_obj/n_swath.h"
#include "fields2cover/types.h"

TEST(fields2cover_obj_n_swath, get_best_value) {
  F2CSwaths swaths3 (2);
  f2c::obj::NSwath obj;
  EXPECT_EQ(obj.computeCost(swaths3), 2);

  for (int i = 0; i < 10; ++i) {
    swaths3.emplace_back(F2CSwath());
    EXPECT_EQ(obj.computeCost(swaths3), i+3);
  }
}

TEST(fields2cover_obj_n_swath, computeCost_cost) {
  F2CSwath swaths1 (1);
  F2CSwaths swaths2 (2);
  F2CSwaths swaths3 (2);
  F2CSwathsByCells swaths_cells;
  swaths_cells.emplace_back(swaths2);
  swaths_cells.emplace_back(swaths3);

  f2c::obj::NSwath obj;

  EXPECT_EQ(obj.computeCost(swaths1), 1.0);
  EXPECT_EQ(obj.computeCostWithMinimizingSign(swaths1), 1.0);

  EXPECT_EQ(obj.computeCost(swaths2), 2.0);
  EXPECT_EQ(obj.computeCostWithMinimizingSign(swaths2), 2.0);

  EXPECT_EQ(obj.computeCost(swaths3), 2.0);
  EXPECT_EQ(obj.computeCostWithMinimizingSign(swaths3), 2.0);

  EXPECT_EQ(obj.computeCost(swaths_cells), 4.0);
  EXPECT_EQ(obj.computeCostWithMinimizingSign(swaths_cells), 4.0);

  swaths3.emplace_back(F2CSwath());
  EXPECT_EQ(obj.computeCost(swaths3), 3.0);

  swaths3.emplace_back(F2CSwath());
  EXPECT_EQ(obj.computeCost(swaths3), 4.0);
}

TEST(fields2cover_obj_n_swath, params_check) {
  f2c::obj::NSwath objective;

  EXPECT_FALSE(objective.isMaximizing());
  EXPECT_TRUE(objective.isMinimizing());
  EXPECT_FALSE(objective.isFastCompAvailable());
}

