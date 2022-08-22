//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <numeric>
#include <fstream>
#include "fields2cover/path_planning/dubins_curves.h"
#include "fields2cover/types.h"
#include "path_planning_checker.hpp"

TEST(fields2cover_pp_dubins, turn_dist) {
  f2c::types::Robot robot(2.1, 2.5);
  robot.cruise_speed = 2.0;
  robot.max_icc = 1.0;
  robot.linear_curv_change = 1.0;

  f2c::pp::DubinsCurves turn(robot);
  F2CPoint start(0.0, 0.0), end(-3.0, 0.0);
  auto path = turn.createTurn(
      start, boost::math::constants::half_pi<double>(),
      end, 3.0 * boost::math::constants::half_pi<double>());
  EXPECT_TRUE(IsPathCorrect(path, start, boost::math::constants::half_pi<double>(),
      end, 3.0 * boost::math::constants::half_pi<double>()));
}

TEST(fields2cover_pp_dubins, random_points) {
  f2c::types::Robot robot(2.1, 2.5);
  robot.cruise_speed = 2.0;
  robot.max_icc = 1.0;
  robot.linear_curv_change = 1.0;
  f2c::pp::DubinsCurves turn(robot);
  turn.using_cache = false;
  const double step = 0.01;
  for (double ang = step; ang < boost::math::constants::pi<double>(); ang += step) {
    F2CPoint start(0.0, 0.0), end(4.0, 0.0);
    auto path = turn.createTurn(start, ang,
        end, ang + boost::math::constants::pi<double>());
    EXPECT_TRUE(IsPathCorrect(path, start, ang,
      end, ang + boost::math::constants::pi<double>()));
  }
}


TEST(fields2cover_pp_dubins, cached_turn) {
  f2c::types::Robot robot(2.1, 2.5);
  robot.cruise_speed = 2.0;
  robot.max_icc = 0.3;
  robot.linear_curv_change = 1.0;
  f2c::pp::DubinsCurves turn(robot);
  turn.using_cache = true;
  auto path1 = turn.createTurn(
      F2CPoint(-331.021, 418.463), 4.71239,
      F2CPoint(-328.021, 404.284), 1.5708);
  //auto path4 = turn.createTurnIfNotCached(14.4929, 0.208505, 3.35);
  auto path2 = turn.createTurn(
      F2CPoint(-331.021, 418.463), 4.71239,
      F2CPoint(-328.021, 404.284), 1.5708);
  //auto path3 = turn.createSimpleTurn(14.4929, 0.208505, 3.35);
  EXPECT_TRUE(path1.points==path2.points);
}

