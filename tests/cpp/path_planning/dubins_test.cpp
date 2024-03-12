//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <numeric>
#include <fstream>
#include "fields2cover/path_planning/dubins_curves.h"
#include "fields2cover/types.h"
#include "../test_helpers/path_planning_checker.hpp"

TEST(fields2cover_pp_dubins, turn_dist) {
  f2c::types::Robot robot(2.1);
  robot.setCruiseVel(2.0);
  robot.setMaxCurv(1.0);
  robot.setMaxDiffCurv(1.0);

  f2c::pp::DubinsCurves turn;
  F2CPoint start(0.0, 0.0), end(-3.0, 0.0);
  auto path = turn.createTurn(robot,
      start, boost::math::constants::half_pi<double>(),
      end, 3.0 * boost::math::constants::half_pi<double>());
  EXPECT_TRUE(IsPathCorrect(path, start, boost::math::constants::half_pi<double>(),
      end, 3.0 * boost::math::constants::half_pi<double>()));
}

TEST(fields2cover_pp_dubins, random_points) {
  f2c::types::Robot robot(2.1);
  robot.setCruiseVel(2.0);
  robot.setMaxCurv(1.0);
  robot.setMaxDiffCurv(1.0);
  f2c::pp::DubinsCurves turn;
  EXPECT_TRUE(turn.getUsingCache());
  turn.setUsingCache(false);
  EXPECT_FALSE(turn.getUsingCache());
  const double step = 0.1;
  for (double ang = step; ang < boost::math::constants::pi<double>(); ang += step) {
    F2CPoint start(0.0, 0.0), end(4.0, 0.0);
    auto path = turn.createTurn(robot, start, ang,
        end, ang + boost::math::constants::pi<double>());
    EXPECT_TRUE(IsPathCorrect(path, start, ang,
      end, ang + boost::math::constants::pi<double>()));
  }
}


TEST(fields2cover_pp_dubins, cached_turn) {
  f2c::types::Robot robot(2.1);
  robot.setCruiseVel(2.0);
  robot.setMaxCurv(0.3);
  robot.setMaxDiffCurv(1.0);
  f2c::pp::DubinsCurves turn;
  turn.setUsingCache(true);
  EXPECT_TRUE(turn.getUsingCache());

  auto path1 = turn.createTurn(robot,
      F2CPoint(-331.021, 418.463), 4.71239,
      F2CPoint(-328.021, 404.284), 1.5708);
  //auto path4 = turn.createTurnIfNotCached(14.4929, 0.208505, 3.35);
  auto path2 = turn.createTurn(robot,
      F2CPoint(-331.021, 418.463), 4.71239,
      F2CPoint(-328.021, 404.284), 1.5708);
  EXPECT_EQ(path1.size(), path2.size());
  for (int i = 0; i < path1.size(); ++i) {
    EXPECT_TRUE(path1[i].point == path2[i].point);
  }
}

TEST(fields2cover_pp_dubins, turn_valid) {
  f2c::types::Robot robot(2.1);
  robot.setCruiseVel(2.0);
  robot.setMaxCurv(0.3);
  f2c::pp::DubinsCurves turn;
  auto path3 = turn.createSimpleTurn(robot, 14.4929, 0.208505, 3.35);
  path3.populate(100);
  EXPECT_TRUE(turn.isTurnValid(path3, 14.4929, 3.35, 0.1, 0.1));

  path3[50].point.setY(-3);
  EXPECT_FALSE(turn.isTurnValid(path3, 14.4929, 3.35, 0.1, 0.1));
  path3.back().angle = 0;
  EXPECT_FALSE(turn.isTurnValid(path3, 14.4929, 3.35, 0.1, 0.1));
  path3.back().point.setY(3);
  EXPECT_FALSE(turn.isTurnValid(path3, 14.4929, 3.35, 0.1, 0.1));
  path3.back().point.setX(15);
  EXPECT_FALSE(turn.isTurnValid(path3, 14.4929, 3.35, 0.1, 0.1));
}

