//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include "fields2cover.h"
#include <iostream>

int main() {
  f2c::Random rand(42);
  F2CRobot robot (2.0, 6.0);
  f2c::hg::ConstHL const_hl;
  F2CCells cells = rand.generateRandField(5, 1e4).field;
  F2CCells no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.robot_width);
  f2c::sg::BruteForce<f2c::obj::NSwath> bf;
  F2CSwaths swaths = bf.generateSwaths(M_PI, robot.op_width, no_hl.getGeometry(0));
  f2c::rp::SnakeOrder snake_sorter(swaths);
  swaths = snake_sorter.genSortedSwaths();


  f2c::pp::PathPlanning path_planner;
  robot.setMinRadius(2);  // m
  robot.linear_curv_change = 0.1;  // 1/m^2


  std::cout << "####### Tutorial 6.1 Dubins curves ######" << std::endl;
  f2c::pp::DubinsCurves dubins(robot);
  F2CPath path_dubins = path_planner.searchBestPath(swaths, dubins);

  f2c::Visualizer::figure(61);
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(path_dubins);
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::save("Tutorial_6_1_Dubins");

  std::cout << "####### Tutorial 6.2 Dubins curves with Continuous curvature ######" << std::endl;
  f2c::pp::DubinsCurvesCC dubins_cc(robot);
  F2CPath path_dubins_cc = path_planner.searchBestPath(swaths, dubins_cc);

  f2c::Visualizer::figure(62);
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(path_dubins_cc);
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::save("Tutorial_6_2_Dubins_CC");

  std::cout << "####### Tutorial 6.3 Reeds-Shepp curves ######" << std::endl;
  f2c::pp::ReedsSheppCurves reeds_shepp(robot);
  F2CPath path_reeds_shepp = path_planner.searchBestPath(swaths, reeds_shepp);

  f2c::Visualizer::figure(63);
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(path_reeds_shepp);
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::save("Tutorial_6_3_Reeds_Shepp");

  std::cout << "####### Tutorial 6.4 Reeds-Shepp curves with Continuous curvature ######" << std::endl;
  f2c::pp::ReedsSheppCurvesHC reeds_shepp_hc(robot);
  F2CPath path_reeds_shepp_hc = path_planner.searchBestPath(swaths, reeds_shepp_hc);

  f2c::Visualizer::figure(64);
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(path_reeds_shepp_hc);
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::save("Tutorial_6_4_Reeds_Shepp_HC");

  return 0;
}


