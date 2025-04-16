//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover.h"
#include "../test_helpers/robot_data.hpp"
#include "../test_helpers/path_planning_checker.hpp"

TEST(fields2cover_types_robotwheel, getWidth_setWidth) {
  f2c::types::RobotWheel wheel;
  wheel.setWidth(0.3);
  EXPECT_EQ(wheel.getWidth(), 0.3);
}

TEST(fields2cover_types_robotwheel, pressurePoints) {
  f2c::types::RobotWheel wheel;
  EXPECT_EQ(wheel.size(), 1);
  EXPECT_EQ(wheel.getPressurePoints().size(), 1);
  EXPECT_EQ(wheel.getPressurePoints().back(), F2CPoint(0, 0));
  EXPECT_EQ(wheel.getPressurePoint(100), F2CPoint(0, 0));

  F2CLineString line {F2CPoint(0.1, 0.2), F2CPoint(0.3, 0.4), F2CPoint(0.5, 0.6)};
  wheel.setPressurePoints(line);
  EXPECT_EQ(wheel.size(), 3);
  EXPECT_EQ(wheel.getPressurePoint(1), F2CPoint(0.3, 0.4));
  wheel.addPressurePoint(F2CPoint(0.7, 0.8));
  EXPECT_EQ(wheel.size(), 4);
  EXPECT_EQ(wheel.getPressurePoint(3), F2CPoint(0.7, 0.8));
  EXPECT_EQ(wheel.getPressurePoints().back(), F2CPoint(0.7, 0.8));
}


