//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <numeric>
#include <fstream>
#include "fields2cover/path_planning/dubins_curves_cc.h"
#include "fields2cover/types.h"
#include "path_planning_checker.hpp"

TEST(fields2cover_pp_dubins_cc, turn_dist) {
  f2c::types::Robot robot(2.1, 2.5);
  robot.cruise_speed = 2.0;
  robot.max_icc = 1.0;
  robot.linear_curv_change = 2.0;

  f2c::pp::DubinsCurvesCC turn;
  F2CPoint start(0.0, 0.0), end(-3.0, 0.0);
  auto path = turn.createTurn(robot,
      start, boost::math::constants::half_pi<double>(),
      end, 3.0 * boost::math::constants::half_pi<double>(), 10);

  EXPECT_TRUE(IsPathCorrect(path, start, boost::math::constants::half_pi<double>(),
      end, 3.0 * boost::math::constants::half_pi<double>()));
}

TEST(fields2cover_pp_dubins_cc, random_points) {
  f2c::types::Robot robot(2.1, 2.5);
  robot.cruise_speed = 2.0;
  robot.max_icc = 1.0;
  robot.linear_curv_change = 1.0;
  f2c::pp::DubinsCurvesCC turn;
  turn.using_cache = false;
  for (double ang = 0.25; ang < boost::math::constants::pi<double>(); ang += 0.25) {
    F2CPoint start(0.0, 0.0), end(4.0, 0.0);
    auto path = turn.createTurn(robot, start, ang,
      end, ang + boost::math::constants::pi<double>(), 10);
    EXPECT_TRUE(IsPathCorrect(path, start, ang,
      end, ang + boost::math::constants::pi<double>()));
  }
}

