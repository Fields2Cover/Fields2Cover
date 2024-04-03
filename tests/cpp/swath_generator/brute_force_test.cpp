//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "fields2cover/utils/random.h"
#include "fields2cover/types.h"
#include "fields2cover/objectives/sg_obj/n_swath.h"
#include "fields2cover/swath_generator/brute_force.h"
#include "../test_helpers/swath_generator_checker.hpp"


TEST(fields2cover_swath_brute_gen, generate_swaths) {
  f2c::Random rand(4);
  f2c::sg::BruteForce sw_gen;
  auto poly = rand.genConvexCell(1e3);

  for (int i = 0; i < 10; ++i) {
    double ang = rand.getAngleRandom();
    auto swaths = sw_gen.generateSwaths(ang, 1, poly);
    for (auto&& s : swaths) {
      EXPECT_NEAR(s.getInAngle(), ang, 1e-3);
      EXPECT_NEAR(s.getOutAngle(), ang, 1e-3);
    }
  }
}

TEST(fields2cover_swath_brute_gen, best_angle) {
  f2c::Random rand(4);
  f2c::obj::NSwath n_swath;
  f2c::sg::BruteForce sw_gen;

  sw_gen.setStepAngle(1);
  EXPECT_EQ(sw_gen.getStepAngle(), 1.0);
  auto poly = rand.genConvexCell(1e3);
  isSwathGenerationCorrect(sw_gen, n_swath, poly);
  isSwathGenerationCorrect(sw_gen, n_swath, F2CCells(poly));
  isSwathGenerationCorrect(sw_gen, n_swath, F2CCell());
  isSwathGenerationCorrect(sw_gen, n_swath, F2CCells());
}
