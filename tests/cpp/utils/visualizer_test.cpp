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
  F2CFields fields {field};
  F2CRobot robot;
  F2COptim optim;

  robot.start_point = F2CPoint(-55, -22);
  robot.end_point = F2CPoint(35, 43);
  robot.op_width = 3.0;
  F2CRobots robots {robot};
  optim.headland_width = 2.0 * robot.op_width;

  f2c::hg::ConstHL hl_gen;
  auto no_headlands = hl_gen.generateHeadlands(
      field.field, optim.headland_width);
  
  f2c::sg::BruteForce swath_gen;
  auto swaths = swath_gen.generateSwaths(0.1, robot.op_width, no_headlands);

  f2c::Visualizer::show(false);
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(fields);
  f2c::Visualizer::plot(no_headlands);
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::plot(robots);
  f2c::Visualizer::axis("equal");
  f2c::Visualizer::save("test_cell.png");
  struct stat buffer;
  EXPECT_TRUE((stat ("test_cell.png", &buffer) == 0));
}



TEST(fields2cover_utils_visualizer, plot_cell) {
  F2CPoint p (2.1, 2.2);
  F2CMultiPoint ps {F2CPoint(0.1, 0.2), F2CPoint(0.3, 0.2)};
  F2CLineString line1;
  line1.addPoint( 0.0, 1.0);
  line1.addPoint( 1.0, 1.0);
  line1.addPoint( 1.0, 4.0);
  F2CLineString line2 {F2CPoint(1.7, 1.8), F2CPoint(1.8, 1.8)};
  F2CLineString line3 {F2CPoint(1.8, 1.9), F2CPoint(1.9, 1.9)};
  F2CLineString line4 {F2CPoint(2.0, 2.0), F2CPoint(2.9, 2.9)};
  F2CLinearRing ring {F2CPoint(3.0, 3.0), F2CPoint(3.0, 2.0), F2CPoint(2.0, 2.0), F2CPoint(3.0, 3.0)};
  F2CMultiLineString lines;
  lines.addGeometry(line3);
  lines.addGeometry(line4);
  F2CSwath swath1(line1);
  F2CPath path1;
  path1.appendSwath(swath1, 2.0);

  f2c::Visualizer::figure(300);
  f2c::Visualizer::title("TEST");
  f2c::Visualizer::plot(std::vector<F2CPath>({path1}));
  f2c::Visualizer::plot(p);
  f2c::Visualizer::plot(std::vector<F2CPoint>({p}));
  f2c::Visualizer::plot(std::vector<F2CMultiPoint>({ps}));
  f2c::Visualizer::plot(std::vector<F2CLineString>({line2}));
  f2c::Visualizer::plot(std::vector<F2CLinearRing>({ring}));
  f2c::Visualizer::plot(std::vector<F2CMultiLineString>({lines}));
  f2c::Visualizer::plot(std::vector<F2CCell>({create_polygon()}));
  f2c::Visualizer::plot(std::vector<F2CCells>({F2CCells(create_polygon())}));
  f2c::Visualizer::plot(std::vector<double>({1.5, 1.4, 1.6}), "rx");
  f2c::Visualizer::plot(std::vector<double>({0.5, 0.6, 0.7}), std::vector<double>({1.5, 1.4, 1.6}), "bx");
  f2c::Visualizer::xlim(-1, 5);
  f2c::Visualizer::ylim(-1, 5);
  f2c::Visualizer::show(false);
}




