//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <filesystem>
#include <chrono>
#include "fields2cover/headland_generator/constant_headland.h"
#include "fields2cover/utils/random.h"
#include "fields2cover/types.h"
#include "fields2cover/utils/visualizer.h"
#include "../test_helpers/path_planning_checker.hpp"
#include "../test_helpers/robot_data.hpp"

TEST(fields2cover_hl_hl_gen_base, find_center_corner_turn) {
  double ang_diff = M_PI * 0.5;
  f2c::types::PathSimp path;
  path.appendSmoothCircularSegment(ang_diff, 2.0, 0.1);

  F2CPoint p = f2c::hg::HeadlandGeneratorBase::findCenterCornerTurn(
      path, path.atEnd().theta);
  EXPECT_NEAR(p.getX(), path.atEnd().p.getX(), 1e-2);
  EXPECT_NEAR(p.getY(), 0.0, 1e-2);
}


TEST(fields2cover_hl_hl_gen_base, contConvexTurn) {
  f2c::types::PathSimp path = f2c::hg::HeadlandGeneratorBase::contConvexTurn(
      M_PI * 0.25, 1.0, 10.0);
  EXPECT_TRUE(isPathCorrect(path.toPath()));
  EXPECT_EQ(path.size(), 4);
  EXPECT_EQ(path[0].type, f2c::types::SectionSimp::CLOTHOID);
  EXPECT_EQ(path[1].type, f2c::types::SectionSimp::CIRCLE);
  EXPECT_EQ(path[2].type, f2c::types::SectionSimp::CIRCLE);
  EXPECT_EQ(path[3].type, f2c::types::SectionSimp::CLOTHOID);

  auto path_s = path.toPath();
  for (auto&& s : path_s.getStates()) {
    EXPECT_FALSE(cos(s.angle) < -0.90);
  }

  f2c::types::PathSimp path_neg = f2c::hg::HeadlandGeneratorBase::contConvexTurn(
      M_PI * 0.25, 1.0, -10.0);
  EXPECT_TRUE(isPathCorrect(path_neg.toPath()));
  EXPECT_EQ(path_neg.size(), 4);
  EXPECT_EQ(path_neg[0].type, f2c::types::SectionSimp::CLOTHOID);
  EXPECT_EQ(path_neg[1].type, f2c::types::SectionSimp::CIRCLE);
  EXPECT_EQ(path_neg[2].type, f2c::types::SectionSimp::CIRCLE);
  EXPECT_EQ(path_neg[3].type, f2c::types::SectionSimp::CLOTHOID);

  f2c::types::PathSimp path_slow = f2c::hg::HeadlandGeneratorBase::contConvexTurn(
      M_PI * 0.25, 10.0, 0.001);
  EXPECT_TRUE(isPathCorrect(path_slow.toPath()));
  EXPECT_EQ(path_slow.size(), 2);
  EXPECT_EQ(path_slow[0].type, f2c::types::SectionSimp::CLOTHOID);
  EXPECT_EQ(path_slow[1].type, f2c::types::SectionSimp::CLOTHOID);
}

TEST(fields2cover_hl_hl_gen_base, generateHeadlandExtBorder) {
  F2CLinearRing ring{F2CPoint(0, 0), F2CPoint(0, 10), F2CPoint(5, 10),
    F2CPoint(5, 5), F2CPoint(10, 5), F2CPoint(10, 10), F2CPoint(15, 15),
    F2CPoint(15, 0), F2CPoint(0, 0)};
  std::vector<double> offsets {0, 1, 1, 1, 1, 1, 0, 1};

  f2c::hg::ConstHL hl_gen;

  EXPECT_THROW(hl_gen.generateHeadlandExtBorder(ring, {1, 2}), std::invalid_argument);

  F2CLinearRing ext_ring = hl_gen.generateHeadlandExtBorder(ring, offsets);

  EXPECT_EQ(ring.size(), ext_ring.size());
  EXPECT_NEAR(137.5, ring.area(), 1e-7);
  EXPECT_GT(ring.area(), ext_ring.area());
  EXPECT_LT(ring.area() / 2.0, ext_ring.area());
}

TEST(fields2cover_hl_hl_gen_base, generateHeadlandBorders) {
  F2CLinearRing ring{F2CPoint(0, 0), F2CPoint(0, 10), F2CPoint(5, 10),
    F2CPoint(5, 5), F2CPoint(10, 5), F2CPoint(10, 10), F2CPoint(15, 15),
    F2CPoint(15, 0), F2CPoint(0, 0)};
  std::vector<double> offsets {0, 1, 1, 1, 1, 1, 0, 1};

  f2c::hg::ConstHL hl_gen;
  auto ext_rings = hl_gen.generateHeadlandBorders(ring, offsets, 3, 0.5);

  EXPECT_NEAR(137.5, ring.area(), 1e-7);
  EXPECT_GT(ring.area(), ext_rings[0].area());
  EXPECT_GT(ext_rings[0].area(), ext_rings[1].area());
  EXPECT_GT(ext_rings[1].area(), ext_rings[2].area());
  EXPECT_LT(ring.area() / 4.0, ext_rings[2].area());
}


TEST(fields2cover_hl_hl_gen_base, getHeadlandBorderRoute) {
  F2CLinearRing ring{F2CPoint(0, 0), F2CPoint(0, 10), F2CPoint(5, 10),
    F2CPoint(5, 5), F2CPoint(10, 5), F2CPoint(10, 10), F2CPoint(15, 15),
    F2CPoint(15, 0), F2CPoint(0, 0)};
  std::vector<double> offsets(ring.size() - 1);

  f2c::hg::ConstHL hl_gen;
  auto ext_rings = hl_gen.generateHeadlandBorders(ring, offsets, 5, 0.5);
  auto line = hl_gen.generateHeadlandBorderRoute(ext_rings, false);
  EXPECT_EQ(ext_rings[0].getGeometry(0), line.getGeometry(0));
  EXPECT_EQ(offsets.size() * 5 + 1, line.size());

  auto line2 = hl_gen.generateHeadlandBorderRoute(ext_rings, true);
  EXPECT_EQ(ext_rings[4].getGeometry(0), line2.getGeometry(0));
  EXPECT_EQ(offsets.size() * 5 + 1, line2.size());
}

TEST(fields2cover_hl_hl_gen_base, connectHeadlandBorderRoute) {
  F2CLinearRing ring {F2CPoint(0, 0), F2CPoint(0, 80), F2CPoint(40, 80),
    F2CPoint(40, 40), F2CPoint(80, 40), F2CPoint(80, 80), F2CPoint(120, 120),
    F2CPoint(120, 0), F2CPoint(0, 0)};
  F2CRobot robot = getSquare1Robot();
  std::vector<double> offsets(ring.size() - 1);
  std::fill(offsets.begin(), offsets.end(), robot.getImplCovWidth() * 0.75);

  f2c::hg::ConstHL hl_gen;
  auto ext_rings = hl_gen.generateHeadlandBorders(ring, offsets, 3, robot.getImplCovWidth());
  auto line = hl_gen.generateHeadlandBorderRoute(ext_rings, true);

  auto planner = f2c::hg::CornerPlanners::FORWARD_CONT_IMPL_OFF;
  auto path1 = hl_gen.connectHeadlandBorderRoute(robot, planner, line, offsets, false, true);
  EXPECT_TRUE(isPathCorrect(path1.toPath()));

  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plotFilled(
      robot.computeAreaTravelled(path1.toPath()), {1, 0., 0.}, {0.8, 0., 0.});
  f2c::Visualizer::plotFilled(
     robot.computeAreaCovered(path1.toPath()), {1, 0.85, 0.4}, {0.8, 0.95, 0.8});
  f2c::Visualizer::plot(ring);
  f2c::Visualizer::plot(line);
  f2c::Visualizer::plot(path1);
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::show();
  */

  planner = f2c::hg::CornerPlanners::FORWARD_CONT_BUFF_IMPL_ON;
  auto path2 = hl_gen.connectHeadlandBorderRoute(robot, planner, line, offsets, false, true);
  EXPECT_TRUE(isPathCorrect(path2.toPath()));

  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plotFilled(
      robot.computeAreaTravelled(path2.toPath()), {1, 0., 0.}, {0.8, 0., 0.});
  f2c::Visualizer::plotFilled(
      robot.computeAreaCovered(path2.toPath()), {1, 0.85, 0.4}, {0.8, 0.95, 0.8});
  f2c::Visualizer::plot(ring);
  f2c::Visualizer::plot(line);
  f2c::Visualizer::plot(path2);
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::show();
  */

  planner = f2c::hg::CornerPlanners::FISHTAIL;
  auto path3 = hl_gen.connectHeadlandBorderRoute(robot, planner, line, offsets, false, true);
  EXPECT_TRUE(isPathCorrect(path3.toPath()));

  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plotFilled(
      robot.computeAreaTravelled(path3.toPath()), {1, 0., 0.}, {0.8, 0., 0.});
  f2c::Visualizer::plotFilled(
      robot.computeAreaCovered(path3.toPath()), {1, 0.85, 0.4}, {0.8, 0.95, 0.8});
  f2c::Visualizer::plot(ring);
  f2c::Visualizer::plot(ext_rings);
  f2c::Visualizer::plot(line);
  f2c::Visualizer::plot(path3);
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::show();
  */
}

TEST(fields2cover_hl_hl_gen_base, genCornerTurn_ForwardOn) {
  F2CRobot robot = getSquare1Robot();
  f2c::hg::ConstHL hl_gen;
  F2CPoint p1 {1, 1};
  F2CPoint p2 {5, 1};
  F2CPoint p3 {9, 6};
  auto planner = f2c::hg::CornerPlanners::FORWARD_CONT_IMPL_ON;
  auto path1 = hl_gen.genCornerTurn(robot, p1, p2, p3, true,  planner, 0, 0, 0.5, 0.5);
  auto path2 = hl_gen.genCornerTurn(robot, p1, p2, p3, false, planner, 0, 0, 0.5, 0.5);

  EXPECT_TRUE(isPathCorrect(path1.toPath()));
  EXPECT_TRUE(isPathCorrect(path2.toPath()));

  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(F2CLineString({p1, p2, p3}));
  f2c::Visualizer::plot(path1);
  f2c::Visualizer::plot(path2);
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::show();
  */
}

TEST(fields2cover_hl_hl_gen_base, genCornerTurn_ForwardOff) {
  F2CRobot robot = getSquare1Robot();
  f2c::hg::ConstHL hl_gen;
  F2CPoint p1 {1, 1};
  F2CPoint p2 {5, 1};
  F2CPoint p3 {9, 6};
  auto planner = f2c::hg::CornerPlanners::FORWARD_CONT_IMPL_OFF;
  auto path1 = hl_gen.genCornerTurn(robot, p1, p2, p3, true,  planner, 0, 0, 0.5, 0.5);
  auto path2 = hl_gen.genCornerTurn(robot, p1, p2, p3, false, planner, 0, 0, 0.5, 0.5);

  EXPECT_TRUE(isPathCorrect(path1.toPath()));
  EXPECT_TRUE(isPathCorrect(path2.toPath()));

  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(F2CLineString({p1, p2, p3}));
  f2c::Visualizer::plot(path1);
  f2c::Visualizer::plot(path2);
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::show();
  */
}

TEST(fields2cover_hl_hl_gen_base, genCornerTurn_Fishtail) {
  F2CRobot robot = getSquare1Robot();
  f2c::hg::ConstHL hl_gen;
  F2CPoint p1 {1, 1};
  F2CPoint p2 {5, 1};
  F2CPoint p3 {9, 6};
  auto planner = f2c::hg::CornerPlanners::FISHTAIL;
  auto path1 = hl_gen.genCornerTurn(robot, p1, p2, p3, true,  planner, 0, 0, 0.5, 0.5);
  auto path2 = hl_gen.genCornerTurn(robot, p1, p2, p3, false, planner, 0, 0, 0.5, 0.5);


  EXPECT_TRUE(isPathCorrect(path1.toPath()));
  EXPECT_TRUE(isPathCorrect(path2.toPath()));

  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(F2CLineString({p1, p2, p3}));
  f2c::Visualizer::plot(path1);
  f2c::Visualizer::plot(path2);
  //f2c::Visualizer::axis_equal();
  f2c::Visualizer::show();
  */
}

TEST(fields2cover_hl_hl_gen_base, genCornerTurn_FordwardBuff) {
  F2CRobot robot = getSquare1Robot();
  f2c::hg::ConstHL hl_gen;
  constexpr double s2 = sqrt(2.0)/2.0;
  F2CPoint p11 {0, 0}; F2CPoint p12 {0, 1}; F2CPoint p13 {0, 2};
  F2CPoint p21 {5, 0}; F2CPoint p22 {4, 1}; F2CPoint p23 {3, 2};
  F2CPoint p31 {5, 5}; F2CPoint p32 {4, 4}; F2CPoint p33 {3, 3};

  auto planner = f2c::hg::CornerPlanners::FORWARD_CONT_BUFF_IMPL_ON;
  auto path1 = hl_gen.genCornerTurn(robot, p11, p21, p31, true, planner, 2, 2, 0.5, 0.5);
  auto path2 = hl_gen.genCornerTurn(robot, p12, p22, p32, true, planner, 1, 2, .5, 0.5);
  auto path3 = hl_gen.genCornerTurn(robot, p13, p23, p33, true, planner, 0, 2, .5, 0.5);

  EXPECT_GT(path1.toPath().size(), 2);
  EXPECT_TRUE(isPathCorrect(path1.toPath()));
  EXPECT_TRUE(isPathCorrect(path2.toPath()));
  EXPECT_TRUE(isPathCorrect(path3.toPath()));

  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(F2CLineString({p11, p21, p31}));
  f2c::Visualizer::plot(F2CLineString({p12, p22, p32}));
  f2c::Visualizer::plot(F2CLineString({p13, p23, p33}));
  f2c::Visualizer::plot(path1);
  f2c::Visualizer::plot(path2);
  f2c::Visualizer::plot(path3);
  f2c::Visualizer::show();
  */
}

TEST(fields2cover_hl_hl_gen_base, computeOffsetTurn) {
  F2CRobot robot = getSquare1Robot();
  f2c::hg::ConstHL hl_gen;
  F2CPoint p1 {0,   0};
  F2CPoint p2 {20,  0};
  F2CPoint p3 {20, 20};
  F2CLineString corner {p1, p2, p3};
  auto planner1 = f2c::hg::CornerPlanners::FORWARD_CONT_IMPL_OFF;
  auto planner2 = f2c::hg::CornerPlanners::FORWARD_CONT_IMPL_ON;
  auto planner3 = f2c::hg::CornerPlanners::FORWARD_CONT_BUFF_IMPL_ON;
  auto planner4 = f2c::hg::CornerPlanners::FISHTAIL;

  auto offsets1 = hl_gen.computeOffsetTurn(robot, M_PI/2.0, planner1, 2);
  double o11 = offsets1.first; double o12 = offsets1.second;
  auto offsets2 = hl_gen.computeOffsetTurn(robot, M_PI/2.0, planner2, 2);
  double o21 = offsets2.first; double o22 = offsets2.second;
  auto offsets3 = hl_gen.computeOffsetTurn(robot, M_PI/2.0, planner3, 0);
  double o311 = offsets3.first; double o321 = offsets3.second;
  auto offsets32 = hl_gen.computeOffsetTurn(robot, 3*M_PI/2.0, planner3, 2);
  double o312 = offsets32.first; double o322 = offsets32.second;
  auto offsets4 = hl_gen.computeOffsetTurn(robot, M_PI/2.0, planner4, 2);
  double o41 = offsets4.first; double o42 = offsets4.second;

  auto path1  = hl_gen.genCornerTurn(robot, p1, p2, p3, true,  planner1, 0, 2, o11, o12);
  auto path2  = hl_gen.genCornerTurn(robot, p1, p2, p3, true,  planner2, 0, 2, o21, o22);
  auto path3  = hl_gen.genCornerTurn(robot, p1, p2, p3, true,  planner3, 0, 0, o311, o312);
  auto path32 = hl_gen.genCornerTurn(robot, p1, p2, p3, true,  planner3, 0, 2, o321, o322);
  auto path4  = hl_gen.genCornerTurn(robot, p1, p2, p3, true,  planner4, 0, 2, o41, o42);


  //EXPECT_NEAR(offsets1.first, offsets1.second,1e-3);

  double s {1.05};
  F2CLineString corner1  {p1 - F2CPoint(0, o11)*s, p2 - F2CPoint(-o12, o11)*s, p3 - F2CPoint(-o12, 0)*s};
  F2CLineString corner2  {p1 - F2CPoint(0, o21)*s, p2 - F2CPoint(-o22, o21)*s, p3 - F2CPoint(-o22, 0)*s};
  F2CLineString corner3  {p1 - F2CPoint(0, o311)*s, p2 - F2CPoint(-o312, o311)*s, p3 - F2CPoint(-o312, 0)*s};
  F2CLineString corner32 {p1 - F2CPoint(0, o321)*s, p2 - F2CPoint(-o322, o321)*s, p3 - F2CPoint(-o322, 0)*s};
  F2CLineString corner4  {p1 - F2CPoint(0, o41)*s, p2 - F2CPoint(-o42, o41)*s, p3 - F2CPoint(-o42, 0)*s};

  EXPECT_TRUE(isPathCorrect(path1.toPath()));
  EXPECT_GT(path1.length(), 0);
  EXPECT_FALSE(corner1.intersects(robot.computeAreaTravelled(path1.toPath())));
  EXPECT_TRUE(isPathCorrect(path2.toPath()));
  EXPECT_GT(path2.length(), 0);
  EXPECT_FALSE(corner2.intersects(robot.computeAreaTravelled(path2.toPath())));
  EXPECT_TRUE(isPathCorrect(path3.toPath()));
  EXPECT_GT(path3.length(), 0);
  EXPECT_FALSE(corner3.intersects(robot.computeAreaTravelled(path3.toPath())));
  EXPECT_TRUE(isPathCorrect(path32.toPath()));
  EXPECT_GT(path32.length(), 0);
  EXPECT_FALSE(corner32.intersects(robot.computeAreaTravelled(path32.toPath())));
  EXPECT_TRUE(isPathCorrect(path4.toPath()));
  EXPECT_GT(path4.length(), 0);
  EXPECT_FALSE(corner4.intersects(robot.computeAreaTravelled(path4.toPath())));

  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plotFilled(
      robot.computeAreaTravelled(path1.toPath()), {1, 0., 0.}, {0.8, 0., 0.});
  f2c::Visualizer::plotFilled(
     robot.computeAreaCovered(path1.toPath()), {1, 0.85, 0.4}, {0.8, 0.95, 0.8});
  f2c::Visualizer::plot(corner);
  f2c::Visualizer::plot(path1);
  f2c::Visualizer::plot(corner1, {0.5,0.9,0.5});
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::show();
  */

}


std::vector<std::vector<double>> read_csv(std::string filename) {
  std::vector<std::vector<double>> result;
  std::ifstream myFile(filename);
  if(!myFile.is_open()) throw std::runtime_error("Could not open file");
  std::string line;
  double x, y, planner;
  std::string op;
  while(std::getline(myFile, line)) {
    std::replace(line.begin(), line.end(), ',', ' ');
    std::stringstream ss(line);
    ss >> x >> y >> planner;
    result.emplace_back(std::vector<double>({x, y, planner}));
  }
  myFile.close();
  return result;
}


/// REMOVE TEST
/*
TEST(fields2cover_hl_hl_gen_base, computeOffsetTurnRing) {
  std::vector<int> possible_planners {0,2,3};

  bool dir_inside_out = true;
  bool dir_right = false;
  bool using_const_offset = false;
  bool using_best_planner = false;
  int using_pick_planner = possible_planners[2];


  f2c::hg::ConstHL hl_gen;
  F2CRobot robot = getSquare1Robot();

  std::string path = std::string(DATA_PATH) + "data_fields/";

  std::ofstream res_file;
  res_file.open ("res_file.csv", std::ios_base::app);


  for (const auto& entry : std::filesystem::directory_iterator(path)) {
    std::cout << entry.path() << std::endl;
    std::string file_name = std::filesystem::path(entry).filename();

    auto field_csv = read_csv(entry.path());
    F2CLinearRing ring;
    std::vector<f2c::hg::CornerPlanners> planners;
    for (auto v : field_csv) {
      ring.addPoint(v[0], v[1]);
      if (using_best_planner) {
        planners.emplace_back(static_cast<f2c::hg::CornerPlanners>(v[2]));
      } else {
        planners.emplace_back(static_cast<f2c::hg::CornerPlanners>(using_pick_planner));
      }
    }
    ring.addPoint(ring.startPoint());

    //ring *= 1.0/sqrt(5.0);


    std::vector<double> offsets (planners.size());
    if (using_const_offset) {
      for (int i = 0; i < offsets.size(); ++i) {
        //offsets[i] = 0.5 *robot.getImplCovWidth() + robot.getImplWidth();
        offsets[i] = robot.getImplLength();
      }
    } else {
      offsets = hl_gen.pickOffsets(hl_gen.computeOffsetTurn(
          robot, ring, dir_right, planners, 3), robot.getImplWidth(), dir_inside_out);
    }

    auto ext_rings = hl_gen.generateHeadlandBorders(ring, offsets, 3, robot.getImplCovWidth());
    auto line = hl_gen.generateHeadlandBorderRoute(ext_rings, dir_inside_out);

    auto path1 = hl_gen.connectHeadlandBorderRoute(robot, planners, line, offsets, dir_right, dir_inside_out);
    EXPECT_TRUE(isPathCorrect(path1.toPath()));

    F2CCell hl(ring);
    hl.addRing(ext_rings[2]);
    auto area_cov = robot.computeAreaCovered(path1.toPath());
    auto area_uncov = F2CCells(hl).difference(area_cov);

    std::cout<<"TOTAL AREA  "<<F2CCell(ring).area()<<std::endl;
    std::cout<<"UNCOV AREA  "<<area_uncov.area()<<std::endl;
    std::cout<<"UNCOV AREA % "<<100.0 * area_uncov.area() / F2CCell(ring).area()<<std::endl;

    res_file << file_name << ", ";
    res_file << using_const_offset << ", ";
    res_file << using_best_planner << ", ";
    res_file << using_pick_planner << ", ";
    res_file << F2CCell(ring).area() << ", ";
    res_file << area_uncov.area() << ", ";
    res_file << path1.toPath().length() << "\n";


    f2c::Visualizer::figure();
    f2c::Visualizer::plotFilled(
      F2CCell(ring), {0.75, 0.8, 0.99}, {0.75, 0.8, 0.99});
    f2c::Visualizer::plotFilled(
      robot.computeAreaCovered(path1.toPath()), {0.65, 0.41, 0.0}, {0.65, 0.41, 0.0});
    //f2c::Visualizer::plotFilled(
    //  robot.computeAreaTravelled(path1.toPath()), {0.3, 0.4, 0.6, 0.05}, {0.3, 0.4, 0.6, 0.05});
    //f2c::Visualizer::plotFilled(
    //    area_cov, {1, 0.85, 0.4}, {0.8, 0.95, 0.8});
    f2c::Visualizer::plot(ring);
    //f2c::Visualizer::plot(ext_rings);
    f2c::Visualizer::plot(line);
    //f2c::Visualizer::plotFilled(
    //    area_uncov, {1, 0.85, 0.4}, {0.8, 0.95, 0.8});
    //f2c::Visualizer::plot(hl);
    f2c::Visualizer::plot(path1);
    f2c::Visualizer::axis_equal();
    f2c::Visualizer::show();
  }
  res_file.close();
}
*/

TEST(fields2cover_hl_hl_gen_base, getConcaveHalfTurn) {
  double ang_end = -.5*M_PI;
  f2c::hg::ConstHL hl;
  auto path1 = hl.getConcaveHalfTurn(M_PI/4.0, ang_end, 1,1,0,0);
  auto path2 = hl.getConcaveHalfTurn(M_PI/8.0, ang_end, 2,1,0,0);
  auto path3 = hl.getConcaveHalfTurn(M_PI/16.0, ang_end, 3,1,0,0);
  EXPECT_NEAR(path1.atEnd().theta, ang_end, 1e-5);
  EXPECT_NEAR(path2.atEnd().theta, ang_end, 1e-5);
  EXPECT_NEAR(path3.atEnd().theta, ang_end, 1e-5);
}

TEST(fields2cover_hl_hl_gen_base, findOffsetCornerOnConcaveTurn) {
  f2c::hg::ConstHL hl;
  EXPECT_EQ(F2CPoint(-1,-2), hl.findOffsetCornerOnConcaveTurn(-0.5*M_PI, 2.0 , 1.0));
  EXPECT_EQ(F2CPoint(-2,-1), hl.findOffsetCornerOnConcaveTurn(-0.5*M_PI, 1.0 , 2.0));
}













std::vector<F2CPoint> movePointsOutCorner(const std::vector<F2CPoint>& ps,
      double ang, double w1, double w2) {
    auto p1_d = ps[0].getPointFromAngle(M_PI/2.0, w1);
    auto p3_d = ps[2].getPointFromAngle(ang + M_PI/2.0, w2);
    auto p2_d = F2CPoint::intersectionOfLines(
        p1_d, ps[1].getPointFromAngle(M_PI/2.0, w1),
        p3_d, ps[1].getPointFromAngle(ang + M_PI/2.0, w2));
    return {p1_d, p2_d, p3_d};
}

std::vector<F2CPoint> movePointsOutCorner2(const std::vector<F2CPoint>& ps,
      double ang, double w1, double w2) {
    auto p1_d = ps[0].getPointFromAngle(M_PI/2.0, w1);
    auto p3_d = ps[2].getPointFromAngle(ang + M_PI/2.0, w2);
    auto p2_d = F2CPoint::intersectionOfLines(
        p1_d, ps[1].getPointFromAngle(M_PI/2.0, w1),
        p3_d, ps[1].getPointFromAngle(ang + M_PI/2.0, w2));
    p1_d = p2_d.getPointFromAngle(M_PI, ps[0].distance(ps[1]));
    p3_d = p2_d.getPointFromAngle(ang, ps[2].distance(ps[1]));
    return {p1_d, p2_d, p3_d};
}


// cost 1: path length of ext turn
double getPathLengthExtTurn(const std::vector<F2CPathSimp>& paths, const F2CRobot& robot) {
  double lr2 = fabs(robot.getFrontImplCov().getX()-robot.getBackImplCov().getX());
  return paths[0].length() - lr2;
}

// cost 2: path length of all turns
double getPathLengthTurns(const std::vector<F2CPathSimp>& paths, const F2CRobot& robot) {
  double len {0};
  double lr2 = + fabs(robot.getFrontImplCov().getX()-robot.getBackImplCov().getX());
  for (auto&& path : paths) {
    len += path.length() - lr2;
  }
  return len;
}

// cost 3: covered area from ext swath to border
double getCovAreaExtTurn(const F2CCells& area_field,
    const std::vector<F2CCells>& cov_area) {
  return cov_area[0].intersection(area_field).buffer(0).area();
}

// cost 4: uncovered area from ext swath to border
double getUncovAreaExtTurn(const F2CCells& area_field,
    const std::vector<F2CCells>& cov_area) {
  return area_field.difference(cov_area[0]).buffer(0).area();
}

// cost 5: traversed area from center to border of ext turn
double getTravAreaExtTurn(const F2CCells& area_field,
    const std::vector<F2CCells>& trav_area) {
  return trav_area[0].intersection(area_field).buffer(0).area();
}

// cost 6: covered area by all the swaths
double getCovAreaTurns(const F2CCells& area_field,
    const std::vector<F2CCells>& cov_area) {
  F2CCells cov {cov_area[0]};
  for (auto c : cov_area) {
    cov = cov.unionOp(c);
  }
  return cov.intersection(area_field).area();
}

// cost 7: uncovered area by all the swaths
double getUncovAreaTurns(const F2CCells& area_field,
    const std::vector<F2CCells>& cov_area) {
  F2CCells cov {cov_area[0]};
  for (auto c : cov_area) {
    cov = cov.unionOp(c);
  }
  return area_field.difference(cov.unionCascaded()).area();
}

// cost 8: travelled area by all the swaths
double getTravAreaTurns(const F2CCells& area_field,
    const std::vector<F2CCells>& trav_area) {
  F2CCells trav {trav_area[0]};
  for (auto t : trav_area) {
    trav = trav.unionOp(t);
  }
  return trav.intersection(area_field).area();
}

// cost 9: Cov area overlap
double getOverlapAreaTurns(const F2CCells& area_field,
    const std::vector<F2CCells>& cov_area) {
  double overlap {0};
  for (int i = 0; i < cov_area.size(); ++i) {
    for (int j = i + 1; j < cov_area.size(); ++j) {
      overlap += (cov_area[i].intersection(cov_area[j])).area();
    }
  }
  return overlap;
}



