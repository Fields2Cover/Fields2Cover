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
#include "../test_helpers/robot_data.hpp"

TEST(fields2cover_pp_pp, planPathForConnection) {
  F2CRobot robot = getSimpleRobot();
  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurves dubins;
  F2CMultiPoint mp;
  auto path1 = path_planner.planPathForConnection(robot,
      F2CPoint(0, 0), M_PI * 0.5, mp, F2CPoint(2, 0), M_PI * 1.5,  dubins);
  EXPECT_NEAR(path1.length(), M_PI, 1e-2);

  mp.addPoint(10, 0.33*40);
  mp.addPoint(20, 0.66*40);
  mp.addPoint(30, 40);

  auto path2 = path_planner.planPathForConnection(robot,
      F2CPoint(0, 0), M_PI * 0.5, mp, F2CPoint(60, 0), M_PI * 1.5,  dubins);
  EXPECT_NEAR(path2.length(), 100, 10.0);

  mp.addPoint(33, 40);
  auto path3 = path_planner.planPathForConnection(robot,
      F2CPoint(30, 37), 0.0, mp, F2CPoint(35, 37), 0.0,  dubins);
  EXPECT_NEAR(path3.length(), 5, 1);

  EXPECT_TRUE(isPathCorrect(path1));
  EXPECT_TRUE(isPathCorrect(path2));
  EXPECT_TRUE(isPathCorrect(path3));
}

