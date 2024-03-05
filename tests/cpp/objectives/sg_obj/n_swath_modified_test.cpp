//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/objectives/sg_obj/n_swath_modified.h"
#include "fields2cover/types.h"

TEST(fields2cover_obj_n_swath_mod, computeCost_cost) {
  f2c::obj::NSwathModified objective;
  F2CCell cell;

  F2CLinearRing ext_ring;
  ext_ring.addPoint(0, 0);
  ext_ring.addPoint(4, 0);
  ext_ring.addPoint(4, 5);
  ext_ring.addPoint(0, 4);
  ext_ring.addPoint(0, 0);

  F2CLinearRing int_ring1;
  int_ring1.addPoint(0.1, 0.1);
  int_ring1.addPoint(0.1, 3.0);
  int_ring1.addPoint(0.2, 3.6);
  int_ring1.addPoint(0.2, 0.1);
  int_ring1.addPoint(0.1, 0.1);

  F2CLinearRing int_ring2;
  int_ring2.addPoint(2.1, 0.1);
  int_ring2.addPoint(2.1, 3.0);
  int_ring2.addPoint(2.2, 3.6);
  int_ring2.addPoint(2.2, 0.1);
  int_ring2.addPoint(2.1, 0.1);

  EXPECT_EQ(objective.computeCost(cell), 0.0);
  cell.addRing(ext_ring);
  EXPECT_NEAR(objective.computeCost(cell), 0.0, 1e-7);
  EXPECT_NEAR(objective.computeCost(0.0, 0.1, cell), 50.0, 1e-7);
  EXPECT_NEAR(objective.computeCost(M_PI * 0.5, 0.1, cell), 40.0, 1e-7);
  EXPECT_NEAR(objective.computeCost(M_PI, 0.1, cell), 50.0, 1e-7);
  cell.addRing(int_ring1);
  EXPECT_NEAR(objective.computeCost(0.0, 0.1, cell), 85.0, 1e-7);
  EXPECT_NEAR(objective.computeCost(M_PI * 0.5, 0.1, cell), 41.0, 1e-7);
  EXPECT_NEAR(objective.computeCost(M_PI, 0.1, cell), 85.0, 1e-7);
  cell.addRing(int_ring2);
  EXPECT_NEAR(objective.computeCost(0.0, 0.1, cell), 120.0, 1e-7);
  EXPECT_NEAR(objective.computeCost(M_PI * 0.5, 0.1, cell), 42.0, 1e-7);
  EXPECT_NEAR(objective.computeCostWithMinimizingSign(M_PI, 0.1, cell), 120.0, 1e-7);
}

TEST(fields2cover_obj_n_swath_mod, params_check) {
  f2c::obj::NSwathModified objective;

  EXPECT_FALSE(objective.isMaximizing());
  EXPECT_TRUE(objective.isMinimizing());
  EXPECT_TRUE(objective.isFastCompAvailable());
}

