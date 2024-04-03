//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include "fields2cover.h"
#include <iostream>

int main() {
  std::cout << "####### Tutorial 7.1 Why and when decompose ######" << std::endl;

  F2CCells cells(F2CCell(F2CLinearRing({
        F2CPoint(  0,  0),
        F2CPoint( 90,  0),
        F2CPoint( 90, 20),
        F2CPoint( 20, 20),
        F2CPoint( 20, 90),
        F2CPoint(  0, 90),
        F2CPoint(  0,  0)})));

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::xlim(-5,95);
  f2c::Visualizer::ylim(-5,95);
  f2c::Visualizer::save("Tutorial_7_1_field.png");

  F2CRobot robot (1.0, 1.0);
  double r_w = robot.getCovWidth();

  f2c::hg::ConstHL const_hl;
  f2c::sg::BruteForce bf;
  f2c::obj::NSwathModified obj;

  F2CCells no_hl_wo_decomp = const_hl.generateHeadlands(cells, 3.0 * r_w);
  F2CSwathsByCells swaths_wo_decomp = bf.generateBestSwaths(obj, r_w, no_hl_wo_decomp);
  std::cout << "Without decomposition >> " << obj.computeCost(swaths_wo_decomp) << std::endl;

  f2c::decomp::TrapezoidalDecomp decomp;
  decomp.setSplitAngle(0.5*M_PI);
  F2CCells decomp_cell = decomp.decompose(cells);

  F2CCells no_hl_decomp = const_hl.generateHeadlands(decomp_cell, 3.0 * r_w);
  F2CSwathsByCells swaths_decomp = bf.generateBestSwaths(obj, r_w, no_hl_decomp);
  std::cout << "With decomposition >> " << obj.computeCost(swaths_decomp) << std::endl;


  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl_wo_decomp);
  f2c::Visualizer::plot(swaths_wo_decomp);
  f2c::Visualizer::xlim(-5,95);
  f2c::Visualizer::ylim(-5,95);
  f2c::Visualizer::save("Tutorial_7_1_without_decomposition.png");

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(decomp_cell);
  f2c::Visualizer::plot(no_hl_decomp);
  f2c::Visualizer::plot(swaths_decomp);
  f2c::Visualizer::xlim(-5,95);
  f2c::Visualizer::ylim(-5,95);
  f2c::Visualizer::save("Tutorial_7_1_with_decomposition.png");

  std::cout << "####### Tutorial 7.2 Decomposition with route planner ######" << std::endl;


  F2CCells mid_hl = const_hl.generateHeadlands(cells, 1.5 * r_w);
  F2CCells decomp_mid_hl = decomp.decompose(mid_hl);
  F2CCells no_hl = const_hl.generateHeadlands(decomp_mid_hl, 1.5 * r_w);
  F2CSwathsByCells swaths = bf.generateBestSwaths(obj, r_w, no_hl);

  f2c::rp::RoutePlannerBase route_planner;
  F2CRoute route = route_planner.genRoute(mid_hl, swaths);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(route);
  f2c::Visualizer::xlim(-5,95);
  f2c::Visualizer::ylim(-5,95);
  f2c::Visualizer::save("Tutorial_7_2_route.png");

  return 0;
}


