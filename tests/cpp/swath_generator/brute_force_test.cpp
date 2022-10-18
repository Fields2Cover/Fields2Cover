//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "fields2cover/utils/random.h"
#include "fields2cover/types.h"
#include "fields2cover/objectives/n_swath.h"
#include "fields2cover/swath_generator/brute_force.h"
#include "swath_generator_checker.hpp"


TEST(fields2cover_swath_brute_gen, best_angle) {
  f2c::Random rand(4);
  f2c::obj::NSwath n_swath;
  f2c::sg::BruteForce sw_gen;

  sw_gen.step_angle = 1;
  auto poly = rand.genConvexField(1e3).field.getCell(0);
  IsSwathGenerationCorrect(sw_gen, n_swath, poly);
  IsSwathGenerationCorrect(sw_gen, n_swath, F2CCells(poly));
  IsSwathGenerationCorrect(sw_gen, n_swath, F2CCell());
  IsSwathGenerationCorrect(sw_gen, n_swath, F2CCells());
}
