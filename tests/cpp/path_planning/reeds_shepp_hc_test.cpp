//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <numeric>
#include <fstream>
#include "fields2cover/path_planning/reeds_shepp_curves_hc.h"
#include "fields2cover/types.h"
#include "../test_helpers/path_planning_checker.hpp"

TEST(fields2cover_pp_reeds_shepp_hc, turn_dist) {
  f2c::types::Robot robot(2.1);
  robot.setCruiseVel(2.0);
  robot.setMaxCurv(1.0);
  robot.setMaxDiffCurv(1.0);

  f2c::pp::ReedsSheppCurvesHC turn;
  F2CPoint start(0.0, 0.0), end(-3.0, 0.0);
  auto path = turn.createTurn(robot,
      start, boost::math::constants::half_pi<double>(),
      end, 3.0 * boost::math::constants::half_pi<double>());
  EXPECT_TRUE(IsPathCorrect(path, start, boost::math::constants::half_pi<double>(),
        end, 3.0 * boost::math::constants::half_pi<double>(), false));
}

TEST(fields2cover_pp_reeds_shepp_hc, random_points) {
  f2c::types::Robot robot(2.1);
  robot.setCruiseVel(2.0);
  robot.setMaxCurv(1.0);
  robot.setMaxDiffCurv(1.0);
  f2c::pp::ReedsSheppCurvesHC turn;
  EXPECT_TRUE(turn.getUsingCache());
  turn.setUsingCache(false);
  EXPECT_FALSE(turn.getUsingCache());
  for (double ang = 0.25; ang < boost::math::constants::pi<double>(); ang += 0.25) {
    F2CPoint start(0.0, 0.0), end(4.0, 0.0);
    auto path = turn.createTurn(robot, start, ang,
        end, ang + boost::math::constants::pi<double>());
    EXPECT_TRUE(IsPathCorrect(path, start, ang,
        end, ang + boost::math::constants::pi<double>(), false));
  }
}

