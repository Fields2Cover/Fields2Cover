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
#include "fields2cover.h"


TEST(fields2cover_sg_brute_gen, generate_swaths) {
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
    std::vector<double> widths {1, 2, 4, 8};
    auto v_swaths = sw_gen.generateSwaths(ang, widths, poly);
    for (auto&& i_swaths : v_swaths) {
      for (auto&& s : i_swaths) {
        EXPECT_NEAR(s.getInAngle(), ang, 1e-3);
        EXPECT_NEAR(s.getOutAngle(), ang, 1e-3);
      }
    }
    for (int j = 0; j < 3; ++j) {
      for (int k = 0; k < 1; ++k) {
        EXPECT_NEAR(v_swaths[j][k].getPath().distance(v_swaths[j+1][k].getPath()),
            0.5*(widths[j] + widths[j+1]), 1e-3);
      }
    }
  }
}

TEST(fields2cover_sg_brute_gen, best_angle) {
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

TEST(fields2cover_sg_brute_gen, gen_swaths) {
  f2c::Random rand(4);
  f2c::sg::BruteForce sw_gen;
  F2CCell cell {F2CLinearRing({{0, 0}, {37, 0}, {37, 30}, {0, 30}, {0, 0}})};

  double ang = .5*M_PI;

  sw_gen.setOverlapType(f2c::sg::SwathOverlapType::NO_OVERLAP);
  auto swaths1 = sw_gen.generateSwaths(ang, 10, cell);
  for (auto&& s : swaths1) {
    EXPECT_NEAR(s.getInAngle(), ang, 1e-3);
    EXPECT_NEAR(s.getOutAngle(), ang, 1e-3);
  }
  EXPECT_TRUE(areSwathsCorrectlyGen(swaths1, {5, 15, 25}));

  sw_gen.setOverlapType(f2c::sg::SwathOverlapType::END_OVERLAP);
  auto swaths2 = sw_gen.generateSwaths(ang, 10, cell);
  EXPECT_TRUE(areSwathsCorrectlyGen(swaths2, {5, 15, 25, 32}));


  sw_gen.setOverlapType(f2c::sg::SwathOverlapType::MIDDLE_OVERLAP);
  auto swaths3 = sw_gen.generateSwaths(ang, 10, cell);
  EXPECT_TRUE(areSwathsCorrectlyGen(swaths3, {5, 15, 25, 35}));

  sw_gen.setOverlapType(f2c::sg::SwathOverlapType::EVENLY_DISTRIBUTED_OVERLAP);
  auto swaths4 = sw_gen.generateSwaths(ang, 10, cell);
  EXPECT_TRUE(areSwathsCorrectlyGen(swaths4, {5, 14, 23, 32}));
}

TEST(fields2cover_sg_brute_gen, get_swaths_distribution) {
  auto check = [](const std::vector<double>& v1, const std::vector<double>& v2) {
    if (v1.size() != v2.size()) {
      EXPECT_TRUE(false);
      return;
    }
    for (int i = 0; i < v1.size(); ++i) {
      EXPECT_NEAR(v1[i], v2[i], 1e-2);
    }
  };

  auto d0 = f2c::sg::SwathGeneratorBase::getSwathsDistribution(
      f2c::sg::SwathOverlapType::NO_OVERLAP, 37, 10);
  auto d1 = f2c::sg::SwathGeneratorBase::getSwathsDistribution(
      f2c::sg::SwathOverlapType::END_OVERLAP, 37, 10);
  auto d2 = f2c::sg::SwathGeneratorBase::getSwathsDistribution(
      f2c::sg::SwathOverlapType::MIDDLE_OVERLAP, 37, 10);
  auto d3 = f2c::sg::SwathGeneratorBase::getSwathsDistribution(
      f2c::sg::SwathOverlapType::EVENLY_DISTRIBUTED_OVERLAP, 37, 10);

  check(d0, {5, 15, 25});
  check(d1, {5, 15, 25, 32});
  check(d2, {5, 15, 25, 35});
  check(d3, {5, 14, 23, 32});
}
