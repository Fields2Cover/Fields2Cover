//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <utility>
#include "fields2cover/objectives/path_length.h"
#include "fields2cover/path_planning/dubins_curves.h"
#include "fields2cover/types.h"

TEST(fields2cover_obj_pathLength, computeCost) { 
  f2c::types::Robot robot(2.1, 2.5);
  robot.max_icc = 1.0/1.5;
  f2c::pp::DubinsCurves turn;
  F2CPoint start(0.0, 0.0), end(-3.0, 0.0);
  f2c::obj::PathLength obj;

  auto path = turn.createTurn(robot,
      start, boost::math::constants::half_pi<double>(),
      end, 3.0 * boost::math::constants::half_pi<double>(), 10);
  
  EXPECT_NEAR(obj.computeCost(path), 3 * M_PI / 2.0, 1e-2);
  EXPECT_NEAR(obj.computeCostWithMinimizingSign(path), 3 * M_PI / 2.0, 1e-2);
}

TEST(fields2cover_obj_pathLength, params_check) {
  f2c::obj::PathLength objective;
  EXPECT_TRUE(objective.isMinimizing());
  EXPECT_FALSE(objective.isMaximizing());
}

