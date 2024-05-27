//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include <iostream>
#include <chrono>
#include <thread>
#include <gtest/gtest.h>
#include "fields2cover/utils/visualizer.h"
#include "fields2cover.h"
#include "../test_helpers/robot_data.hpp"

bool checkIfFileExists(const std::string& file, double max_time_waiting = 10.0) {
  struct stat buffer;
  const clock_t begin_time = std::clock();
  double t {0.0};
  while (t < max_time_waiting) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    t = double(std::clock() - begin_time) /  CLOCKS_PER_SEC;
    if (stat(file.c_str(), &buffer) == 0) {
      return true;
    }
  }
  return false;
}

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

TEST(fields2cover_utils_visualizer, save_Route_and_Path) {
  F2CCells cells {
    F2CCell(F2CLinearRing({
          F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)
    }))
  };
  cells.addRing(0, F2CLinearRing({
          F2CPoint(.4,.4), F2CPoint(.4,.6),F2CPoint(.6,.6),F2CPoint(.6,.4), F2CPoint(.4,.4)
  }));
  cells.addRing(0, F2CLinearRing({
          F2CPoint(1.2,1.2), F2CPoint(1.2,1.6),F2CPoint(1.6,1.6),F2CPoint(1.6,1.2), F2CPoint(1.2,1.2)
  }));
  cells *= 3e1;

  f2c::hg::ConstHL const_hl;
  F2CCells no_hl = const_hl.generateHeadlandArea(cells, 3, 3);
  auto hl_swaths = const_hl.generateHeadlandSwaths(cells, 3, 3, false);

  f2c::sg::BruteForce bf;
  F2CSwathsByCells swaths = bf.generateSwaths(M_PI/2.0, 3, no_hl);

  f2c::rp::RoutePlannerBase route_planner;
  F2CRoute route = route_planner.genRoute(hl_swaths[1], swaths);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(hl_swaths[1]);
  f2c::Visualizer::plot(route.asLineString());
  for (auto&& vs : route.getVectorSwaths()) {
    f2c::Visualizer::plot(vs);
  }
  for (auto&& c : route.getConnections()) {
    f2c::Visualizer::plot(c);
  }
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::save("test_fig/route_by_pieces.png");
  EXPECT_TRUE(checkIfFileExists("test_fig/route_by_pieces.png"));


  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(hl_swaths[1]);
  f2c::Visualizer::plot(route);
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::save("test_fig/route.png");
  EXPECT_TRUE(checkIfFileExists("test_fig/route.png"));

  F2CRobot robot;
  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurves dubins;
  F2CPath path = path_planner.planPath(robot, route, dubins);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(F2CPath());
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(hl_swaths[1]);
  f2c::Visualizer::plot(path);
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::save("test_fig/path.png");
  EXPECT_TRUE(checkIfFileExists("test_fig/path.png"));
}

TEST(fields2cover_utils_visualizer, save_Cell) {
  F2CLinearRing ring1(std::vector<F2CPoint>(
        {F2CPoint(-3,-3), F2CPoint(4,-2), F2CPoint(4,8), F2CPoint(-3,-3)}));
  F2CLinearRing ring2(std::vector<F2CPoint>(
        {F2CPoint(1,1), F2CPoint(3,2), F2CPoint(3,6), F2CPoint(1,1)}));
  F2CCell cell1;
  cell1.addRing(ring1);
  cell1.addRing(ring2);
  cell1 = cell1 + F2CPoint(15,0);
  F2CCell cell2 = create_polygon();
  F2CCells cells;
  cells.addGeometry(cell1);
  cells.addGeometry(cell2);
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(
      F2CLineString(F2CPoint(12,4), F2CPoint(20, 4)));
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(cell1);
  f2c::Visualizer::save("test_fig/cells_def.png");
  EXPECT_TRUE(checkIfFileExists("test_fig/cells_def.png"));

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells, {0.0, 0.9, 0.0});
  f2c::Visualizer::plot(cell1, {0.9, 0.0, 0.0});
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::save("test_fig/cells.png");
  EXPECT_TRUE(checkIfFileExists("test_fig/cells.png"));

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(
      F2CLineString(F2CPoint(12,4), F2CPoint(20, 4)), {1.0, 0, 0});
  f2c::Visualizer::plotFilled(cells, {0.0, 0.9, 0.0});
  f2c::Visualizer::plotFilled(cell1, {0.9, 0.0, 0.0});
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::save("test_fig/cells_filled.png");
  EXPECT_TRUE(checkIfFileExists("test_fig/cells_filled.png"));

  f2c::Visualizer::figure();
  f2c::Visualizer::plotFilled(cells, {0.0, 0.9, 0.0}, {1.0, 0.0, 1.0});
  f2c::Visualizer::plotFilled(F2CCell(), {0.5,0,0}, {0.5,0.5,0.5});
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::save("test_fig/cells_holes.png");
  EXPECT_TRUE(checkIfFileExists("test_fig/cells_holes.png"));
}

TEST(fields2cover_utils_visualizer, save_LinearRing) {
  F2CLinearRing ring(std::vector<F2CPoint>(
        {F2CPoint(1,1), F2CPoint(3,2), F2CPoint(3,6), F2CPoint(1,1)}));
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(ring + F2CPoint(-5,0));
  f2c::Visualizer::plot(ring, {0.9, 0.0, 0.0});
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::save("test_fig/linearring.png");
  EXPECT_TRUE(checkIfFileExists("test_fig/linearring.png"));
}

TEST(fields2cover_utils_visualizer, save_Point) {
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(F2CPoint(1,5));
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::save("test_fig/point.png");
  EXPECT_TRUE(checkIfFileExists("test_fig/point.png"));
}

TEST(fields2cover_utils_visualizer, save_field) {
  f2c::Random rand(1);
  F2CField field = rand.generateRandField(1e4, 5);
  F2CRobot robot (3.0);

  f2c::hg::ConstHL hl_gen;
  auto no_headlands = hl_gen.generateHeadlands(field.getField(), robot.getCovWidth());

  f2c::sg::BruteForce swath_gen;
  auto swaths = swath_gen.generateSwaths(0.1, robot.getCovWidth(), no_headlands);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(F2CPoint(3,4));
  f2c::Visualizer::plot(field);
  f2c::Visualizer::plot(no_headlands);
  f2c::Visualizer::plot(swaths[0], {});
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::save("test_fig/test_cell.png");
  EXPECT_TRUE(checkIfFileExists("test_fig/test_cell.png"));
}



TEST(fields2cover_utils_visualizer, plot_vectors) {
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

  f2c::Visualizer::figure();
  f2c::Visualizer::title("TEST");
  f2c::Visualizer::plot(lines);
  f2c::Visualizer::plot(line1, {0,0,0.95});
  f2c::Visualizer::plot(line2);
  f2c::Visualizer::xlim(-1, 5);
  f2c::Visualizer::ylim(-1, 5);
  f2c::Visualizer::save("test_fig/test_everything2.png");
  EXPECT_TRUE(checkIfFileExists("test_fig/test_everything2.png"));
}

TEST(fields2cover_utils_visualizer, save_Point_size) {
  f2c::Visualizer::figure();
  f2c::Visualizer::figure_size(100, 100);
  f2c::Visualizer::plot(F2CPoint(1,5));
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::save("test_fig/point_100x100.png");
  EXPECT_TRUE(checkIfFileExists("test_fig/point_100x100.png"));
}

