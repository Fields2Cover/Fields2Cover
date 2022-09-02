//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include <iostream>
#include <gtest/gtest.h>
#include "fields2cover/utils/visualizer.h"
#include "fields2cover.h"

F2CCell create_polygon(void) {
  F2CCell poly;
  F2CLinearRing line;
  line.addPoint(-10, 0);
  line.addPoint(-10, 20);
  line.addPoint(10, 20);
  line.addPoint(10, 0);
  line.addPoint(-10, 0);
  poly.addRing(line);
  return poly; 
}

TEST(fields2cover_utils_visualizer, save_field) {
  f2c::Random rand(1);
  F2CField field = rand.generateRandField(5, 1e4);
  F2CRobot robot;
  F2COptim optim;

  robot.start_point = F2CPoint(-55, -22);
  robot.end_point = F2CPoint(35, 43);
  robot.op_width = 3.0;
  optim.headland_width = 2.0 * robot.op_width;

  f2c::hg::ConstHL<f2c::obj::NSwath> hl_gen;
  auto no_headlands = hl_gen.generateHeadlands(
      field.field, optim.headland_width);
  
  f2c::sg::BruteForce<f2c::obj::NSwath> swath_gen;
  auto swaths = swath_gen.generateSwaths(0.1, robot.op_width, no_headlands);

  f2c::Visualizer::show(false);
  f2c::Visualizer::figure(1000);
  f2c::Visualizer::plot(field.field);
  f2c::Visualizer::plot(no_headlands);
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::plot(robot);
  f2c::Visualizer::save("test_cell.png");
  struct stat buffer;
  EXPECT_TRUE((stat ("test_cell.png", &buffer) == 0));
}



TEST(fields2cover_utils_visualizer, plot_cell) {
  F2CLineString line1;
  line1.addPoint( 0.0, 1.0);
  line1.addPoint( 1.0, 1.0);
  line1.addPoint( 1.0, 4.0);
  F2CSwath swath1(line1);
  F2CPath path1;
  path1.appendSwath(swath1, 2.0);

  f2c::Visualizer::figure(300);
  f2c::Visualizer::plot(path1);
  f2c::Visualizer::plot(create_polygon());
  f2c::Visualizer::show(false);
}

