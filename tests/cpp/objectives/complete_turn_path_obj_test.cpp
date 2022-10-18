//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/objectives/complete_turn_path_obj.h"
#include "fields2cover/objectives/path_length.h"
#include "fields2cover/path_planning/dubins_curves.h"
#include "fields2cover/path_planning/dubins_curves_cc.h"
#include "fields2cover/path_planning/reeds_shepp_curves.h"
#include "fields2cover/path_planning/reeds_shepp_curves_hc.h"
#include "fields2cover/utils/visualizer.h"

TEST(fields2cover_obj_complete_turn_path_obj, computeCost_cost) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(1.0, 1.0), F2CPoint(1.0, 0.0)}));
  F2CSwaths swaths1 {swath1};
  F2CSwaths swaths2 {swath2};

  F2CRobot robot(2.1, 2.5);
  robot.setMinRadius(0.5);
 
  f2c::pp::DubinsCurves dubins;
  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves> length(robot, dubins);

  // Precision is quite low so this is just checking a prediction is done
  EXPECT_NEAR(length.computeCost(swath1, swath2), M_PI/2.0, 0.1);
}


TEST(fields2cover_obj_complete_turn_path_obj, computeCost_cost_2) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(1.0, 1.0), F2CPoint(1.0, 0.0)}));
  F2CSwaths swaths1 {swath1};
  F2CSwaths swaths2 {swath2};

  F2CRobot robot(2.1, 2.5);
  robot.setMinRadius(0.5);

  f2c::pp::DubinsCurves dubins;
  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves, f2c::obj::PathLength> length(robot);
  length.setTurnPlanner(dubins);
  EXPECT_NEAR(length.computeCost(
        F2CPoint(0.0, 1.0), M_PI/2.0, F2CPoint(1.0, 1.0), -M_PI/2.0), M_PI/2.0, 0.1);
}

TEST(fields2cover_obj_complete_turn_path_obj, computeCost_cost_long_swath) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(100.0, 1.0), F2CPoint(101.0, 0.0)}));
  F2CSwaths swaths {swath1, swath2};

  F2CRobot robot(2.1, 2.5);
  robot.setMinRadius(0.5);
 
  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves> length(robot);

  // Precision is quite low so this is just checking a prediction is done
  EXPECT_GT(length.computeCost(swath1, swath2), 96 + M_PI);
  EXPECT_LT(length.computeCost(swath1, swath2), 98 + M_PI);
  EXPECT_GT(length.computeCost(swaths), 98 + M_PI);
  EXPECT_LT(length.computeCost(swaths), 100 + M_PI);
}

TEST(fields2cover_obj_complete_turn_path_obj, computeCost_cost_dubinsCC) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(100.0, 1.0), F2CPoint(101.0, 0.0)}));
  F2CSwaths swaths {swath1, swath2};

  F2CRobot robot(2.1, 2.5);
  robot.setMinRadius(0.5);
 
  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurvesCC> length(robot);
  length.setTurnPlanner(f2c::pp::DubinsCurvesCC());

  // Precision is quite low so this is just checking a prediction is done
  EXPECT_GT(length.computeCost(swath1, swath2), 96 + M_PI);
  EXPECT_LT(length.computeCost(swath1, swath2), 98 + M_PI);
  EXPECT_GT(length.computeCost(swaths), 98 + M_PI);
  EXPECT_LT(length.computeCost(swaths), 102 + M_PI);
}

TEST(fields2cover_obj_complete_turn_path_obj, computeCost_cost_reedsshepp) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(100.0, 1.0), F2CPoint(101.0, 0.0)}));
  F2CSwaths swaths {swath1, swath2};
  F2CRobot robot(2.1, 2.5);
  robot.setMinRadius(0.5);
 
  f2c::obj::CompleteTurnPathObj<f2c::pp::ReedsSheppCurves> length(robot);
  length.setTurnPlanner(f2c::pp::ReedsSheppCurves());

  // Precision is quite low so this is just checking a prediction is done
  EXPECT_GT(length.computeCost(swath1, swath2), 96 + M_PI);
  EXPECT_LT(length.computeCost(swath1, swath2), 98 + M_PI);
  EXPECT_GT(length.computeCost(swaths), 98 + M_PI);
  EXPECT_LT(length.computeCost(swaths), 100 + M_PI);
}

TEST(fields2cover_obj_complete_turn_path_obj, computeCost_cost_reedssheppHC) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(100.0, 1.0), F2CPoint(101.0, 0.0)}));
  F2CSwaths swaths {swath1, swath2};
  F2CRobot robot(2.1, 2.5);
  robot.setMinRadius(0.5);
 
  f2c::obj::CompleteTurnPathObj<f2c::pp::ReedsSheppCurvesHC> length(robot);
  length.setTurnPlanner(f2c::pp::ReedsSheppCurvesHC());

  // Precision is quite low so this is just checking a prediction is done
  EXPECT_GT(length.computeCost(swath1, swath2), 96 + M_PI);
  EXPECT_LT(length.computeCost(swath1, swath2), 98 + M_PI);
  EXPECT_GT(length.computeCost(swaths), 98 + M_PI);
  EXPECT_LT(length.computeCost(swaths), 102 + M_PI);
}

TEST(fields2cover_complete_pred_turn_path_obj, params_check) {
  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves> objective;

  EXPECT_FALSE(objective.isMaximizing());
  EXPECT_TRUE(objective.isMinimizing());
}

