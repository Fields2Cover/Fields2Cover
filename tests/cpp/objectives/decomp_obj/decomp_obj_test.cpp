//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/objectives/decomp_obj/decomp_objective.h"

TEST(fields2cover_obj_decomp_obj, computeCost) {
  f2c::obj::DecompObjective obj;
  F2CCells cells;
  EXPECT_EQ(obj.computeCost(cells), 0.0);
  EXPECT_TRUE(obj.isMinimizing());
  EXPECT_FALSE(obj.isMaximizing());
}

