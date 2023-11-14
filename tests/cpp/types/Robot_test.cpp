//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_robot, init) {
  F2CRobot robot(3, 6);
  EXPECT_EQ(robot.robot_width, 3);
  EXPECT_EQ(robot.op_width, 6);
  EXPECT_EQ(robot.getRobotWidth(), 3);

  EXPECT_THROW( F2CRobot(0,0), std::out_of_range);
  EXPECT_THROW( F2CRobot(-3,3), std::out_of_range);
  EXPECT_THROW( F2CRobot(3,-3), std::out_of_range);
  EXPECT_THROW( F2CRobot(6,3), std::out_of_range);

  auto r1 = robot;
  EXPECT_EQ(r1.robot_width, 3);
  EXPECT_EQ(r1.op_width, 6);
  EXPECT_EQ(r1.getRobotWidth(), 3);

  auto r2 {robot};
  EXPECT_EQ(r2.robot_width, 3);
  EXPECT_EQ(r2.op_width, 6);
  EXPECT_EQ(r2.getRobotWidth(), 3);

  auto r3 = F2CRobot(1, 2);
  EXPECT_EQ(r3.robot_width, 1);
  EXPECT_EQ(r3.op_width, 2);

  F2CRobot r4;
  r4.setMinRadius(20.0);
  EXPECT_NEAR(r4.op_width, 0, 1e-7);
  EXPECT_NEAR(r4.robot_width, 0, 1e-7);
  EXPECT_NEAR(r4.max_icc, 1/20.0, 1e-7);

  F2CRobot r5 = std::move(r4);
  EXPECT_NEAR(r5.op_width, 0, 1e-7);
  EXPECT_NEAR(r5.robot_width, 0, 1e-7);
  EXPECT_NEAR(r5.max_icc, 1/20.0, 1e-7);
}


