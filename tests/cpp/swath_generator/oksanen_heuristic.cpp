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
#include "fields2cover/swath_generator/oksanen_heuristic.h"
#include "../test_helpers/swath_generator_checker.hpp"


TEST(fields2cover_sg_oksanen_heuristic, generate_swaths) {
  f2c::Random rand(4);
  f2c::sg::OksanenHeuristic sw_gen;
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

TEST(fields2cover_sg_oksanen_heuristic, best_angle) {
  f2c::Random rand(4);
  f2c::obj::SwathLength n_swath;
  f2c::sg::OksanenHeuristic sw_gen;

  sw_gen.setStepAngle(20. * M_PI / 180.);
  EXPECT_NEAR(sw_gen.getStepAngle(), 20. * M_PI / 180., 1e-3);
  sw_gen.setMaxIt(8);
  EXPECT_EQ(sw_gen.getMaxIt(), 8);
  auto poly = rand.genConvexCell(1e3);
  isSwathGenerationCorrect(sw_gen, n_swath, poly);
  isSwathGenerationCorrect(sw_gen, n_swath, F2CCells(poly));
  isSwathGenerationCorrect(sw_gen, n_swath, F2CCell());
  isSwathGenerationCorrect(sw_gen, n_swath, F2CCells());

  sw_gen.setStepAngle(20. * M_PI / 180.);
  sw_gen.setMaxIt(8);
  const double gr_inv = 0.5 * (sqrt(5.0) - 1.0);
  sw_gen.setRatioStepAngle(gr_inv);
  isSwathGenerationCorrect(sw_gen, n_swath, poly);
}

TEST(fields2cover_sg_oksanen_heuristic, step_ang_seq) {
  f2c::sg::OksanenHeuristic sw_gen;
  sw_gen.setStepAngle(20. * M_PI / 180.);
  sw_gen.setMaxIt(8);
  auto seq05 = sw_gen.getStepAngleSeq();
  double seq05_s = sw_gen.getFinalStepAngle();
  EXPECT_EQ(seq05.size(), sw_gen.getMaxIt());
  EXPECT_NEAR(seq05[0], sw_gen.getStepAngle(), 1e-10);
  EXPECT_NEAR(seq05.back(), seq05_s, 1e-10);
  EXPECT_EQ(sw_gen.getTimesComp(), 9 + 2 * 3 * 7);

  int n1 = sw_gen.getTimesComp();

  const double gr_inv = 0.5 * (sqrt(5.0) - 1.0);
  sw_gen.setRatioStepAngle(gr_inv);
  EXPECT_NEAR(sw_gen.getRatioStepAngle(), gr_inv, 1e-10);
  auto seqgr = sw_gen.getStepAngleSeq();
  double seqgr_s = sw_gen.getFinalStepAngle();
  EXPECT_EQ(seqgr.size(), sw_gen.getMaxIt());
  EXPECT_NEAR(seqgr[0], sw_gen.getStepAngle(), 1e-10);
  EXPECT_NEAR(seqgr.back(), seqgr_s, 1e-10);

  int n2 = sw_gen.getTimesComp();

  const double gr2_inv = 0.5 * (3.0 - sqrt(5.0));
  sw_gen.setRatioStepAngle(gr2_inv);

  sw_gen.setStepAngle(10. * M_PI / 180.);
  sw_gen.setMaxIt(4);
  sw_gen.setNumBestSelected(4);

  auto seqgr2 = sw_gen.getStepAngleSeq();
  double seqgr2_s = sw_gen.getFinalStepAngle();
  EXPECT_NEAR(seqgr2[0], sw_gen.getStepAngle(), 1e-10);
  EXPECT_NEAR(seqgr2.back(), seqgr2_s, 1e-10);
}



