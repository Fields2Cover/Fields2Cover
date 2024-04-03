//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include "fields2cover.h"
#include <iostream>

int main() {
  f2c::Random rand(42);
  F2CRobot robot (2.0, 6.0);
  f2c::hg::ConstHL const_hl;
  F2CCells cells = rand.generateRandField(1e4, 5).getField();
  F2CCells no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.getWidth());
  f2c::sg::BruteForce bf;
  F2CSwaths swaths = bf.generateSwaths(M_PI, robot.getCovWidth(), no_hl.getGeometry(0));
  f2c::rp::SnakeOrder snake_sorter;
  swaths = snake_sorter.genSortedSwaths(swaths);


  f2c::pp::PathPlanning path_planner;
  robot.setMinTurningRadius(2);  // m
  robot.setMaxDiffCurv(0.1);  // 1/m^2


  std::cout << "####### Tutorial 6.1 Dubins curves ######" << std::endl;
  f2c::pp::DubinsCurves dubins;
  F2CPath path_dubins = path_planner.planPath(robot, swaths, dubins);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(path_dubins);
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::save("Tutorial_6_1_Dubins.png");

  std::cout << "####### Tutorial 6.2 Dubins curves with Continuous curvature ######" << std::endl;
  f2c::pp::DubinsCurvesCC dubins_cc;
  F2CPath path_dubins_cc = path_planner.planPath(robot, swaths, dubins_cc);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(path_dubins_cc);
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::save("Tutorial_6_2_Dubins_CC.png");

  std::cout << "####### Tutorial 6.3 Reeds-Shepp curves ######" << std::endl;
  f2c::pp::ReedsSheppCurves reeds_shepp;
  F2CPath path_reeds_shepp = path_planner.planPath(robot, swaths, reeds_shepp);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(path_reeds_shepp);
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::save("Tutorial_6_3_Reeds_Shepp.png");

  std::cout << "####### Tutorial 6.4 Reeds-Shepp curves with Continuous curvature ######" << std::endl;
  f2c::pp::ReedsSheppCurvesHC reeds_shepp_hc;
  F2CPath path_reeds_shepp_hc = path_planner.planPath(robot, swaths, reeds_shepp_hc);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(path_reeds_shepp_hc);
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::save("Tutorial_6_4_Reeds_Shepp_HC.png");

  return 0;
}


