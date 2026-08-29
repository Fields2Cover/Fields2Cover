//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/headland_generator/constant_headland.h"
#include "fields2cover/types.h"

TEST(fields2cover_hg_base, maxHLWidthRequired) {
  f2c::hg::ConstHL hl_gen;
  F2CRobot robot(2.0, 3.0);
  robot.setMinTurningRadius(5.0);

  // Two turning radii to come back on track, plus half the robot on the border.
  // The radius does not survive a round trip untouched: Robot guards the
  // division with a 1e-7 term, so 5.0 comes back as 4.9999976.
  EXPECT_NEAR(hl_gen.maxHLWidthRequired(robot), 11.0, 1e-4);
}

TEST(fields2cover_hg_base, generate_headlands_with_robot) {
  F2CCell cell {F2CLinearRing {
    F2CPoint(0,0), F2CPoint(100,0), F2CPoint(100,100), F2CPoint(0,100),
    F2CPoint(0,0)}};
  F2CCells cells {cell};
  f2c::hg::ConstHL hl_gen;
  F2CRobot robot(2.0, 3.0);
  robot.setMinTurningRadius(5.0);
  double width = hl_gen.maxHLWidthRequired(robot);

  // Without a generator of its own, the widest headland is left on every border.
  EXPECT_NEAR(hl_gen.generateHeadlands(cell, robot, 0.0).area(),
              hl_gen.generateHeadlands(cells, width).area(), 1e-7);
  EXPECT_NEAR(hl_gen.generateHeadlands(cells, robot, {0.0}).area(),
              hl_gen.generateHeadlands(cells, width).area(), 1e-7);
  EXPECT_NEAR(hl_gen.generateHeadlands(cell, robot, 0.0).area(),
              (100 - 2 * width) * (100 - 2 * width), 1e-7);
}
