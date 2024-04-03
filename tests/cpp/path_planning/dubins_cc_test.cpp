//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <numeric>
#include <fstream>
#include "fields2cover.h"
#include "../test_helpers/path_planning_checker.hpp"


TEST(fields2cover_pp_dubins_cc, turn_dist) {
  f2c::types::Robot robot(2.5);
  robot.setCruiseVel(2.0);
  robot.setMaxCurv(1.0);
  robot.setMaxDiffCurv(2.0);

  f2c::pp::DubinsCurvesCC turn;
  F2CPoint start(0.0, 0.0), end(-3.0, 0.0);
  const double pi_2 { boost::math::constants::half_pi<double>()};
  auto path = turn.createTurn(robot, start, pi_2, end, 3.0 * pi_2);

  EXPECT_TRUE(IsPathCorrect(path, start, pi_2, end, 3.0 * pi_2));
}

TEST(fields2cover_pp_dubins_cc, random_points) {
  f2c::types::Robot robot(2.5);
  robot.setCruiseVel(2.0);
  robot.setMaxCurv(1.0);
  robot.setMaxDiffCurv(1.0);
  f2c::pp::DubinsCurvesCC turn;
  EXPECT_TRUE(turn.getUsingCache());
  turn.setUsingCache(false);
  EXPECT_FALSE(turn.getUsingCache());

  for (double ang = 0.25; ang < boost::math::constants::pi<double>(); ang += 0.25) {
    F2CPoint start(0.0, 0.0), end(4.0, 0.0);
    auto path = turn.createTurn(robot, start, ang,
      end, ang + boost::math::constants::pi<double>());
    EXPECT_TRUE(IsPathCorrect(path, start, ang,
      end, ang + boost::math::constants::pi<double>()));
  }
}

