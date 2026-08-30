//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/headland_generator/required_headland.h"
#include "fields2cover/headland_generator/constant_headland.h"
#include "fields2cover/utils/random.h"
#include "fields2cover/types.h"

namespace {

F2CRobot createRobot() {
  F2CRobot robot(3.0, 3.0);
  robot.setMinTurningRadius(5.0);
  return robot;
}

F2CCell createSquare(double side) {
  return F2CCell{F2CLinearRing{
    F2CPoint(0, 0), F2CPoint(side, 0), F2CPoint(side, side),
    F2CPoint(0, side), F2CPoint(0, 0)}};
}

}  // namespace

TEST(fields2cover_hg_req_gen, required_headland_dist) {
  f2c::hg::ReqHL hl_gen;
  F2CRobot robot = createRobot();

  // A border the swaths run along is only entered, never turned on.
  EXPECT_NEAR(hl_gen.requiredHeadlandDist(robot, 0.0, 0.0), 6.5, 1e-4);
  EXPECT_NEAR(hl_gen.requiredHeadlandDist(robot, 0.0, M_PI), 6.5, 1e-4);
  // A border the swaths end on takes a full turn.
  EXPECT_NEAR(hl_gen.requiredHeadlandDist(robot, 0.0, 0.5 * M_PI), 11.5, 1e-4);
  EXPECT_NEAR(hl_gen.requiredHeadlandDist(robot, 0.5 * M_PI, 0.0), 11.5, 1e-4);
  // In between it grows with the angle, and never past the widest headland.
  EXPECT_NEAR(hl_gen.requiredHeadlandDist(robot, 0.0, 0.25 * M_PI),
              6.5 + 5.0 * sqrt(0.5), 1e-4);
  EXPECT_LE(hl_gen.requiredHeadlandDist(robot, 1.0, 2.0),
            hl_gen.maxHLWidthRequired(robot));
}

TEST(fields2cover_hg_req_gen, required_headland_dist_on_ring) {
  f2c::hg::ReqHL hl_gen;
  F2CRobot robot = createRobot();
  F2CLinearRing ring = createSquare(100.0).getExteriorRing();

  auto dists = hl_gen.requiredHeadlandDist(robot, 0.0, ring);

  // One distance per segment, not per point.
  EXPECT_EQ(dists.size(), ring.size() - 1);
  EXPECT_NEAR(dists[0], 6.5, 1e-4);
  EXPECT_NEAR(dists[1], 11.5, 1e-4);
  EXPECT_NEAR(dists[2], 6.5, 1e-4);
  EXPECT_NEAR(dists[3], 11.5, 1e-4);
}

TEST(fields2cover_hg_req_gen, headland_width_follows_the_border_angle) {
  f2c::hg::ReqHL hl_gen;
  f2c::hg::ConstHL const_hl_gen;
  F2CRobot robot = createRobot();
  F2CCell cell = createSquare(100.0);

  // Swaths along x: the borders they end on keep 11.5, the other two 6.5.
  auto hl = hl_gen.generateHeadlands(cell, robot, 0.0);
  EXPECT_NEAR(hl.area(), (100 - 2 * 11.5) * (100 - 2 * 6.5), 1e-2);

  // Turning the swaths turns the wide borders with them.
  auto hl_y = hl_gen.generateHeadlands(cell, robot, 0.5 * M_PI);
  EXPECT_NEAR(hl_y.area(), hl.area(), 1e-2);
  EXPECT_NEAR(hl.getWidth(), hl_y.getHeight(), 1e-2);
  EXPECT_NEAR(hl.getHeight(), hl_y.getWidth(), 1e-2);
  // Swaths along x turn on the vertical borders, so x is what shrinks.
  EXPECT_LT(hl.getWidth(), hl.getHeight());

  // A constant headland gives up the difference on every border.
  auto const_hl = const_hl_gen.generateHeadlands(cell, robot, 0.0);
  EXPECT_NEAR(const_hl.area(), (100 - 2 * 11.5) * (100 - 2 * 11.5), 1e-2);
  EXPECT_GT(hl.area(), const_hl.area());

  // The mainland never reaches outside the field.
  EXPECT_TRUE(hl.within(F2CCells(cell)));
}

TEST(fields2cover_hg_req_gen, headland_around_an_obstacle) {
  f2c::hg::ReqHL hl_gen;
  F2CRobot robot = createRobot();
  F2CCell cell = createSquare(100.0);
  cell.addRing(F2CLinearRing{
    F2CPoint(40, 40), F2CPoint(40, 60), F2CPoint(60, 60), F2CPoint(60, 40),
    F2CPoint(40, 40)});

  auto hl = hl_gen.generateHeadlands(cell, robot, 0.0);

  // The obstacle grows by the same rule: 11.5 where a swath ends on it.
  EXPECT_NEAR(hl.area(),
      (100 - 2 * 11.5) * (100 - 2 * 6.5) - (20 + 2 * 11.5) * (20 + 2 * 6.5),
      1e-2);
}

TEST(fields2cover_hg_req_gen, one_track_angle_per_cell) {
  f2c::hg::ReqHL hl_gen;
  F2CRobot robot = createRobot();
  F2CCells cells {createSquare(100.0)};
  cells.addGeometry(createSquare(100.0) + F2CPoint(200, 0));

  auto hl = hl_gen.generateHeadlands(cells, robot, {0.0, 0.5 * M_PI});
  EXPECT_EQ(hl.size(), 2);
  EXPECT_NEAR(hl.area(), 2 * (100 - 2 * 11.5) * (100 - 2 * 6.5), 1e-2);

  // Without an angle per cell there is nothing to size the borders by,
  // so every border keeps the widest headland.
  auto hl_no_angs = hl_gen.generateHeadlands(cells, robot, {0.0});
  EXPECT_NEAR(hl_no_angs.area(),
      hl_gen.generateHeadlands(cells, hl_gen.maxHLWidthRequired(robot)).area(),
      1e-7);
}

TEST(fields2cover_hg_req_gen, empty_area) {
  f2c::Random rand;
  f2c::hg::ReqHL hl_gen;

  auto field = rand.generateRandField(1e3, 5);
  EXPECT_NEAR(field.area(), 1e3, 1e-2);
  auto no_hl = hl_gen.generateHeadlands(field.getField(), 1e2);
  EXPECT_NEAR(no_hl.area(), 0, 1e-7);
}

TEST(fields2cover_hg_req_gen, border_area) {
  f2c::Random rand;
  f2c::hg::ReqHL hl_gen;

  auto field = rand.generateRandField(1e5, 5);
  auto no_hl = hl_gen.generateHeadlands(field.getField(), 1.0);
  auto extra_hl = hl_gen.generateHeadlands(field.getField(), -1.0);

  EXPECT_NEAR(field.area(), 1e5, 1);
  EXPECT_GT(no_hl.area() / field.area(), 0.9);
  EXPECT_LT(no_hl.area() / field.area(), 0.999);
  EXPECT_LT(extra_hl.area() / field.area(), 1.1);
  EXPECT_GT(extra_hl.area() / field.area(), 1.001);
}

TEST(fields2cover_hg_req_gen, border_swaths) {
  f2c::Random rand(4242);
  f2c::hg::ReqHL hl_gen;

  F2CField field = rand.generateRandField(1e4, 5);
  auto hl_swaths = hl_gen.generateHeadlandSwaths(field.getField(), 3.0, 10);
  auto no_hl = hl_gen.generateHeadlandArea(field.getField(), 3.0, 10);

  EXPECT_EQ(hl_swaths.size(), 10);
  EXPECT_GT(hl_swaths[0].area(), hl_swaths[9].area());
  EXPECT_LT(no_hl.area(), field.area());
}
