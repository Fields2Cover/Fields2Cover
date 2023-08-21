//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
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

  auto field = rand.generateRandField(5, 1e3);
  EXPECT_NEAR(field.getArea(), 1e3, 1e-2);
  auto no_hl = hl_gen.generateHeadlands(field.field, 1e2);
  EXPECT_NEAR(no_hl.getArea(), 0, 1e-7);
}

TEST(fields2cover_hl_const_gen, border_area) {
  f2c::Random rand;
  f2c::hg::ConstHL hl_gen;

  auto field = rand.generateRandField(5, 1e5);
  auto no_hl = hl_gen.generateHeadlands(field.field, 1.0);

  EXPECT_GT(field.getArea(), 0);
  EXPECT_NEAR(field.getArea(), 1e5, 1);
  EXPECT_NEAR(no_hl.getArea(), 1e5 * 0.9, 1e4);
  EXPECT_GT(no_hl.getArea() / field.getArea(), 0.9);
  EXPECT_LT(no_hl.getArea() / field.getArea(), 1.0);
}

