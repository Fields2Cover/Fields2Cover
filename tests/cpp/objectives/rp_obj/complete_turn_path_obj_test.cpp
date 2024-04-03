//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/objectives/rp_obj/complete_turn_path_obj.h"
#include "fields2cover/objectives/pp_obj/path_length.h"
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

  F2CRobot robot(2.1);
  robot.setMinTurningRadius(0.5);

  f2c::pp::DubinsCurves dubins;
  f2c::obj::CompleteTurnPathObj length1(robot, dubins);
  f2c::pp::DubinsCurvesCC dubinscc;
  f2c::obj::CompleteTurnPathObj length2(robot, dubinscc);
  f2c::pp::ReedsSheppCurves reeds_shepp;
  f2c::obj::CompleteTurnPathObj length3(robot, reeds_shepp);
  f2c::pp::ReedsSheppCurvesHC reeds_shepphc;
  f2c::obj::CompleteTurnPathObj length4(robot, reeds_shepphc);

  // Precision is quite low so this is just checking a prediction is done
  EXPECT_NEAR(length1.computeCost(swath1, swath2), M_PI/2.0, 0.1);
  EXPECT_NEAR(length2.computeCost(swath1, swath2),
      dubinscc.createSimpleTurn(robot, 1, 0.5*M_PI, 1.5*M_PI).length(), 0.1);
  EXPECT_NEAR(length3.computeCost(swath1, swath2), M_PI/2.0, 0.1);
  EXPECT_NEAR(length4.computeCost(swath1, swath2),
      reeds_shepphc.createSimpleTurn(robot, 1, 0.5*M_PI, 1.5*M_PI).length(), 0.1);
}


TEST(fields2cover_obj_complete_turn_path_obj, computeCost_cost_2) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(1.0, 1.0), F2CPoint(1.0, 0.0)}));
  F2CSwaths swaths1 {swath1};
  F2CSwaths swaths2 {swath2};

  F2CRobot robot(2.1);
  robot.setMinTurningRadius(0.5);

  f2c::pp::DubinsCurves dubins;
  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves, f2c::obj::PathLength> length(robot);
  length.setTurnPlanner(dubins);
  EXPECT_NEAR(length.computeCost(
        F2CPoint(0.0, 1.0), M_PI/2.0, F2CPoint(1.0, 1.0), -M_PI/2.0), M_PI/2.0, 0.1);
}

TEST(fields2cover_obj_complete_turn_path_obj, computeCost_cost_long_swath) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(100.0, 1.0), F2CPoint(100.0, 0.0)}));
  F2CSwaths swaths {swath1, swath2};

  F2CRobot robot(2.1);
  robot.setMinTurningRadius(0.5);

  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves> length(robot);
  length.setTurnPlanner(f2c::pp::DubinsCurves());

  // Precision is quite low so this is just checking a prediction is done
  EXPECT_NEAR(length.computeCost(swath1, swath2), 99 + 0.5 * M_PI, 0.01);
  EXPECT_NEAR(length.computeCost(swaths), 101 + 0.5 * M_PI, 0.01);
}

TEST(fields2cover_obj_complete_turn_path_obj, computeCost_cost_dubinsCC) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(100.0, 1.0), F2CPoint(100.0, 0.0)}));
  F2CSwaths swaths {swath1, swath2};

  F2CRobot robot(2.1);
  robot.setMinTurningRadius(0.5);

  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurvesCC> length(robot);
  length.setTurnPlanner(f2c::pp::DubinsCurvesCC());

  EXPECT_NEAR(length.computeCost(swath1, swath2), 103 + 0.5 * M_PI, 0.4);
  EXPECT_NEAR(length.computeCost(swaths), 105 + 0.5 * M_PI, 0.4);
}

TEST(fields2cover_obj_complete_turn_path_obj, computeCost_cost_reedsshepp) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(100.0, 1.0), F2CPoint(100.0, 0.0)}));
  F2CSwaths swaths {swath1, swath2};
  F2CRobot robot(2.1);
  robot.setMinTurningRadius(0.5);

  f2c::obj::CompleteTurnPathObj<f2c::pp::ReedsSheppCurves> length(robot);
  length.setTurnPlanner(f2c::pp::ReedsSheppCurves());

  // Precision is quite low so this is just checking a prediction is done
  EXPECT_NEAR(length.computeCost(swath1, swath2), 99 + 0.5 * M_PI, 0.01);
  EXPECT_NEAR(length.computeCost(swaths), 101 + 0.5 * M_PI, 0.01);
}

TEST(fields2cover_obj_complete_turn_path_obj, computeCost_cost_reedssheppHC) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(100.0, 1.0), F2CPoint(100.0, 0.0)}));
  F2CSwaths swaths {swath1, swath2};
  F2CRobot robot(2.1);
  robot.setMinTurningRadius(0.5);

  f2c::obj::CompleteTurnPathObj<f2c::pp::ReedsSheppCurvesHC> length(robot);
  length.setTurnPlanner(f2c::pp::ReedsSheppCurvesHC());

  EXPECT_NEAR(length.computeCost(swath1, swath2), 103 + 0.5 * M_PI, 0.3);
  EXPECT_NEAR(length.computeCost(swaths), 105 + 0.5 * M_PI, 0.3);
}

TEST(fields2cover_complete_pred_turn_path_obj, params_check) {
  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves> objective;

  EXPECT_FALSE(objective.isMaximizing());
  EXPECT_TRUE(objective.isMinimizing());
}

