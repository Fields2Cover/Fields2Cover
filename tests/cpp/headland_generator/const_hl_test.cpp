//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <fstream>
#include "fields2cover/headland_generator/constant_headland.h"
#include "fields2cover/utils/random.h"
#include "fields2cover/types.h"

TEST(fields2cover_hl_const_gen, empty_area) {
  f2c::Random rand;
  f2c::hg::ConstHL hl_gen;

  auto field = rand.generateRandField(1e3, 5);
  EXPECT_NEAR(field.area(), 1e3, 1e-2);
  auto no_hl = hl_gen.generateHeadlands(field.getField(), 1e2);
  EXPECT_NEAR(no_hl.area(), 0, 1e-7);
}

TEST(fields2cover_hl_const_gen, border_area) {
  f2c::Random rand;
  f2c::hg::ConstHL hl_gen;

  auto field = rand.generateRandField(1e5, 5);
  auto no_hl = hl_gen.generateHeadlands(field.getField(), 1.0);
  auto extra_hl = hl_gen.generateHeadlands(field.getField(), -1.0);

  EXPECT_GT(field.area(), 0);
  EXPECT_NEAR(field.area(), 1e5, 1);
  EXPECT_NEAR(no_hl.area(), 1e5 * 0.9, 1e4);
  EXPECT_GT(no_hl.area() / field.area(), 0.9);
  EXPECT_LT(no_hl.area() / field.area(), 0.999);
  EXPECT_NEAR(extra_hl.area(), 1e5 * 1.1, 1e4);
  EXPECT_LT(extra_hl.area() / field.area(), 1.1);
  EXPECT_GT(extra_hl.area() / field.area(), 1.001);
}

TEST(fields2cover_hl_const_gen, border_swaths) {
  f2c::Random rand (4242);
  f2c::hg::ConstHL hl_gen;

  F2CField field = rand.generateRandField(1e4, 5);
  std::vector<F2CCells> hl_swaths = hl_gen.generateHeadlandSwaths(field.getField(), 3.0, 10);
  F2CCells no_hl = hl_gen.generateHeadlandArea(field.getField(), 3.0, 10);

  EXPECT_NEAR(field.area(), 1e4, 1e-3);
  EXPECT_EQ(hl_swaths.size(), 10);
  for (size_t i = 0; i < hl_swaths.size()-1; ++i) {
    if (i > 0) {
      EXPECT_LT(hl_swaths[i + 1].area(), hl_swaths[i].area());
    } else {
      EXPECT_LT(hl_swaths[i].area(), field.area());
    }
    EXPECT_EQ(hl_swaths[i].size(), 1);
  }
  EXPECT_GT(hl_swaths.back().area(), no_hl.area());
}


