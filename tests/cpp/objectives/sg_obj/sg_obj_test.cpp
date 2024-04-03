//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/objectives/sg_obj/sg_objective.h"

TEST(fields2cover_obj_sg_obj, get_best_value) {
  F2CSwaths swaths3 (2);
  f2c::obj::SGObjective obj;
  EXPECT_EQ(obj.computeCost(swaths3), 0);

  for (int i = 0; i < 10; ++i) {
    swaths3.emplace_back(F2CSwath());
    EXPECT_EQ(obj.computeCost(swaths3), 0);
  }
}

TEST(fields2cover_obj_sg_obj, computeCost_cost) {
  F2CSwath swath1 (1);
  F2CSwaths swaths2 (2);
  F2CSwaths swaths3 (2);
  F2CSwathsByCells swaths_cells;
  swaths_cells.emplace_back(swaths2);
  swaths_cells.emplace_back(swaths3);

  f2c::obj::SGObjective obj;

  EXPECT_EQ(obj.computeCost(swath1), 0.0);
  EXPECT_EQ(obj.computeCostWithMinimizingSign(swath1), 0.0);

  EXPECT_EQ(obj.computeCost(swaths2), 0.0);
  EXPECT_EQ(obj.computeCostWithMinimizingSign(swaths2), 0.0);

  EXPECT_EQ(obj.computeCost(swaths3), 0.0);
  EXPECT_EQ(obj.computeCostWithMinimizingSign(swaths3), 0.0);

  EXPECT_EQ(obj.computeCost(swaths_cells), 0.0);
  EXPECT_EQ(obj.computeCost(F2CCell(), swaths_cells), 0.0);
  EXPECT_EQ(obj.computeCost(F2CCells(), swaths_cells), 0.0);
  EXPECT_EQ(obj.computeCostWithMinimizingSign(swaths_cells), 0.0);

  swaths3.emplace_back(F2CSwath());
  EXPECT_EQ(obj.computeCost(swaths3), 0.0);

  swaths3.emplace_back(F2CSwath());
  EXPECT_EQ(obj.computeCost(swaths3), 0.0);
  EXPECT_EQ(obj.computeCost(F2CCell(), swaths3), 0.0);
  EXPECT_EQ(obj.computeCost(F2CCells(), swaths3), 0.0);

  EXPECT_EQ(obj.computeCostWithMinimizingSign(F2CCell(), swath1), 0.0);
  EXPECT_EQ(obj.computeCostWithMinimizingSign(F2CCells(), swath1), 0.0);
  EXPECT_EQ(obj.computeCostWithMinimizingSign(F2CCell(), swaths3), 0.0);
  EXPECT_EQ(obj.computeCostWithMinimizingSign(F2CCells(), swaths3), 0.0);
  EXPECT_EQ(obj.computeCostWithMinimizingSign(F2CCell(), swaths_cells), 0.0);
  EXPECT_EQ(obj.computeCostWithMinimizingSign(F2CCells(), swaths_cells), 0.0);
}

TEST(fields2cover_obj_sg_obj, params_check) {
  f2c::obj::SGObjective objective;

  EXPECT_FALSE(objective.isMaximizing());
  EXPECT_TRUE(objective.isMinimizing());
  EXPECT_FALSE(objective.isFastCompAvailable());
}

