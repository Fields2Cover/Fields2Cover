//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/objectives/complete_turn_path_obj.h"
#include "fields2cover/path_planning/dubins_curves.h"

TEST(fields2cover_obj_complete_turn_path_obj, computeCost_cost) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(3.0, 1.0), F2CPoint(3.0, 0.0)}));
  F2CSwaths swaths1 {swath1};
  F2CSwaths swaths2 {swath2};

  F2CRobot robot(2.1, 2.5);
  robot.max_icc = 2.0;
 
  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves> length(robot);

  // Precision is quite low so this is just checking a prediction is done
  EXPECT_NEAR(length.computeCost(swath1, swath2), M_PI, 0.1);
}

TEST(fields2cover_complete_pred_turn_path_obj, params_check) {
  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves> objective;

  EXPECT_FALSE(objective.isMaximizing());
  EXPECT_TRUE(objective.isMinimizing());
}

