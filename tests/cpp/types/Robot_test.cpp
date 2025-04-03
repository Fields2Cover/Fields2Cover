//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover.h"
#include "../test_helpers/robot_data.hpp"
#include "../test_helpers/path_planning_checker.hpp"



TEST(fields2cover_types_robot, init) {
  F2CRobot robot_def_const(1., 2.);
  EXPECT_NEAR(robot_def_const.getWidth(),    1., 1e-5);
  EXPECT_NEAR(robot_def_const.getCovWidth(), 2., 1e-5);

  F2CRobot robot;
  robot.setName("robot test");
  EXPECT_EQ(robot.getName(), "robot test");
  robot.setWidth(5.0);
  EXPECT_NEAR(robot.getWidth(),              5.0, 1e-5);
  robot.setCovWidth(10.0);
  EXPECT_NEAR(robot.getCovWidth(),           10., 1e-5);

  robot.setMinTurningRadius(10.0);
  EXPECT_NEAR(robot.getMaxCurv(),            0.1, 1e-5);
  EXPECT_NEAR(robot.getMinTurningRadius(),   10., 1e-5);

  robot.setMaxCurv(0.4);
  EXPECT_NEAR(robot.getMaxCurv(),            0.4, 1e-5);
  EXPECT_NEAR(robot.getMinTurningRadius(),   2.5, 1e-5);
  robot.setMaxDiffCurv(0.3);
  EXPECT_NEAR(robot.getMaxDiffCurv(),        0.3, 1e-5);

  robot.setCruiseVel(2.0);
  EXPECT_NEAR(robot.getCruiseVel(),          2.0, 1e-5);
  EXPECT_NEAR(robot.getTurnVel(),            2.0, 1e-5);
  robot.setTurnVel(5.0);
  EXPECT_NEAR(robot.getTurnVel(),            5.0, 1e-5);


  EXPECT_THROW(F2CRobot(0), std::out_of_range);
  EXPECT_THROW(F2CRobot(-3), std::out_of_range);

  F2CRobot r1 = robot;
  EXPECT_NEAR(r1.getWidth(), 5.0, 1e-5);

  const F2CRobot r2 {robot};
  EXPECT_EQ(r2.getWidth(), 5.0);
}


