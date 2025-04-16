//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <fstream>
#include "fields2cover/headland_generator/required_headland.h"
#include "fields2cover/swath_generator/curved_swaths.h"
#include "fields2cover/objectives/sg_obj/n_swath.h"
#include "fields2cover/utils/random.h"
#include "fields2cover/utils/visualizer.h"
#include "fields2cover/types.h"

TEST(fields2cover_hg_req_gen, generate_headlands_req_cell_robot) {
  F2CLinearRing line;
  for (double i = 0.0; i < 2.0 * M_PI; i += 0.1) {
    line.addPoint(50 + 50 * cos(i), 100 + 100 * sin(i));
  }
  line.addPoint(line[0]);

  //F2CLinearRing line{
    //F2CPoint(0,0), F2CPoint(100,0),F2CPoint(100,100),F2CPoint(0,100), F2CPoint(0,0)};
  F2CLinearRing line2{
    F2CPoint(48,48), F2CPoint(48,52),F2CPoint(52,52),F2CPoint(52,48), F2CPoint(48,48)};
  F2CCell cell {line};
  cell.addRing(line2);

  F2CRobot robot(3.0, 3.0);
  robot.setMinTurningRadius(5.0);

  double track_ang = M_PI*0.5;

  f2c::hg::ReqHL req_hl;
  auto hl = req_hl.generateHeadlands(cell, robot, track_ang);
  auto hl2 = req_hl.generateHeadlands(cell, robot, 0);

  f2c::sg::CurvedSwathsGen bf;
  f2c::obj::NSwath ns;
  auto swaths = bf.generateBestSwaths(ns, robot.getCovWidth(), hl);


  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cell);
  f2c::Visualizer::plot(hl);
  //f2c::Visualizer::plot(hl2);
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::show();
  */
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

  EXPECT_GT(field.area(), 0);
  EXPECT_NEAR(field.area(), 1e5, 1);
  EXPECT_NEAR(no_hl.area(), 1e5 * 0.9, 1e4);
  EXPECT_GT(no_hl.area() / field.area(), 0.9);
  EXPECT_LT(no_hl.area() / field.area(), 0.999);
  EXPECT_NEAR(extra_hl.area(), 1e5 * 1.1, 1e4);
  EXPECT_LT(extra_hl.area() / field.area(), 1.1);
  EXPECT_GT(extra_hl.area() / field.area(), 1.001);
}

TEST(fields2cover_hg_req_gen, border_swaths) {
  f2c::Random rand (4242);
  f2c::hg::ReqHL hl_gen;

  F2CField field = rand.generateRandField(1e4, 5);
  auto hl_swaths = hl_gen.generateHeadlandSwaths(field.getField(), 3.0, 10);
  F2CCells no_hl = hl_gen.generateHeadlandArea(field.getField(), 3.0, 10);

  EXPECT_NEAR(field.area(), 1e4, 1e-3);
  EXPECT_EQ(hl_swaths.size(), 10);
  for (size_t i = 0; i < hl_swaths.size()-1; ++i) {
    EXPECT_EQ(hl_swaths[i].size(), 5);
  }
}


