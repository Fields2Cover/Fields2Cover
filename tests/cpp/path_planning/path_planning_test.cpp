//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <numeric>
#include <fstream>
#include "fields2cover/types.h"
#include "fields2cover/path_planning/dubins_curves.h"
#include "fields2cover/path_planning/path_planning.h"
#include "fields2cover/path_planning/dubins_curves_cc.h"
#include "fields2cover/route_planning/boustrophedon_order.h"
#include "fields2cover/swath_generator/brute_force.h"

TEST(fields2cover_pp_pp, turn_dist) {
  F2CLineString path1, path2, path3;
  path1.addPoint( 0.0, 0.0);
  path1.addPoint( 0.0, 1.0);
  path2.addPoint( 3.0, 1.0);
  path2.addPoint( 3.0, 0.0);
  F2CSwath swath1(path1);
  F2CSwath swath2(path2);
  F2CSwaths swaths {swath1, swath2};

  F2CRobot robot;
  robot.max_icc = 1.0 / 1.5;
  f2c::pp::PathPlanning path_planner;
  path_planner.turn_point_dist = 0.1;
  f2c::pp::DubinsCurves dubins;

  auto path = path_planner.searchBestPath(robot, swaths, dubins);
  // Precision is quite low so this is just checking a prediction is done
  EXPECT_NEAR(path.length(), 2 + 1.5 * M_PI, 0.1);
}


