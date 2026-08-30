//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include "fields2cover.h"
#include <iostream>

int main() {
  f2c::Random rand(42);
  F2CField field = rand.generateRandField(1e4, 5);
  F2CCells cells = field.getField();
  F2CRobot robot (2.0, 6.0);

  std::cout << "####### Tutorial 3.1 Constant width headland generator ######" << std::endl;
  f2c::hg::ConstHL const_hl;
  F2CCells no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.getWidth());
  std::cout << "The complete area is " << cells.area() <<
    ", and the area without headlands is " << no_hl.area() << std::endl;

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(field);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::save("Tutorial_3_1_Const_width.png");

  std::cout << "####### Tutorial 3.2 Required width headland generator ######" << std::endl;
  robot.setMinTurningRadius(8.0);
  f2c::hg::ReqHL req_hl;
  F2CCells req_no_hl = req_hl.generateHeadlands(cells, robot, {0.0});
  F2CCells const_no_hl = const_hl.generateHeadlands(cells, robot, {0.0});
  std::cout << "With swaths at 0 rad, the area without headlands is " <<
    req_no_hl.area() << ", while a constant headland leaves " <<
    const_no_hl.area() << std::endl;

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(field);
  f2c::Visualizer::plot(const_no_hl);
  f2c::Visualizer::plot(req_no_hl);
  f2c::Visualizer::save("Tutorial_3_2_Req_width.png");

  std::cout << "####### Tutorial 3.3 Corridor headland generator ######" << std::endl;
  F2CCells l_cells(F2CCell(F2CLinearRing({
        F2CPoint(  0,  0), F2CPoint( 90,  0), F2CPoint( 90, 20),
        F2CPoint( 20, 20), F2CPoint( 20, 90), F2CPoint(  0, 90),
        F2CPoint(  0,  0)})));
  const double hl_width = robot.getWidth();
  const double r_w = 3.0;  // narrower than getCovWidth(), so more swaths fit

  F2CCells single_no_hl = const_hl.generateHeadlands(l_cells, hl_width);
  f2c::decomp::TrapezoidalDecomp decomp;
  decomp.setSplitAngle(0.5*M_PI);
  F2CCells decomp_cells = decomp.decompose(single_no_hl);

  f2c::sg::BruteForce bf;
  f2c::obj::NSwathModified obj;
  f2c::rp::RoutePlannerBase route_planner;
  f2c::hg::CorridorHL corridor_hl;
  robot.setMinTurningRadius(0.5);
  robot.setMaxDiffCurv(1.5);
  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurvesCC turn;

  // 1. Double headland: decompose, then headland every border again, so the
  // new internal border loses a headland from both cells that meet there.
  F2CCells double_no_hl = const_hl.generateHeadlands(decomp_cells, hl_width);
  F2CSwathsByCells double_swaths =
    bf.generateBestSwaths(obj, r_w, double_no_hl);
  F2CRoute double_route = route_planner.genRoute(single_no_hl, double_swaths);
  F2CPath double_path = path_planner.planPath(robot, double_route, turn);
  std::cout << "Double headland leaves " << double_no_hl.area() << std::endl;

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(l_cells);
  f2c::Visualizer::plot(double_no_hl);
  f2c::Visualizer::plot(double_path);
  f2c::Visualizer::xlim(-5, 95);
  f2c::Visualizer::ylim(-5, 95);
  f2c::Visualizer::save("Tutorial_3_3_Double.png");

  // 2. Corridor, ASYMMETRIC (the default): the smaller cell gives up the
  // whole corridor and the larger one keeps its full width up to the border.
  corridor_hl.setShareMode(f2c::hg::CorridorShareMode::ASYMMETRIC);
  F2CCells corridor_no_hl =
    corridor_hl.generateHeadlands(decomp_cells, hl_width);
  F2CSwathsByCells corridor_swaths =
    bf.generateBestSwaths(obj, r_w, corridor_no_hl);
  F2CRoute corridor_route =
    route_planner.genRoute(single_no_hl, corridor_swaths);
  F2CPath corridor_path = path_planner.planPath(robot, corridor_route, turn);
  std::cout << "Asymmetric corridor leaves " << corridor_no_hl.area() <<
    std::endl;
  for (auto&& share : corridor_hl.corridorShares(decomp_cells)) {
    std::cout << "Cell " << share.cell_i << " gives " << share.share
      << " of the corridor over " << share.shared_length
      << " m of border with cell " << share.cell_k << std::endl;
  }

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(l_cells);
  f2c::Visualizer::plot(corridor_no_hl);
  f2c::Visualizer::plot(corridor_path);
  f2c::Visualizer::xlim(-5, 95);
  f2c::Visualizer::ylim(-5, 95);
  f2c::Visualizer::save("Tutorial_3_3_Corridor.png");

  // 3. Shared corridor, SYMMETRIC: both cells give half the corridor,
  // regardless of size.
  corridor_hl.setShareMode(f2c::hg::CorridorShareMode::SYMMETRIC);
  F2CCells shared_no_hl = corridor_hl.generateHeadlands(decomp_cells, hl_width);
  F2CSwathsByCells shared_swaths =
    bf.generateBestSwaths(obj, r_w, shared_no_hl);
  F2CRoute shared_route = route_planner.genRoute(single_no_hl, shared_swaths);
  F2CPath shared_path = path_planner.planPath(robot, shared_route, turn);
  std::cout << "Symmetric shared corridor leaves " << shared_no_hl.area() <<
    std::endl;

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(l_cells);
  f2c::Visualizer::plot(shared_no_hl);
  f2c::Visualizer::plot(shared_path);
  f2c::Visualizer::xlim(-5, 95);
  f2c::Visualizer::ylim(-5, 95);
  f2c::Visualizer::save("Tutorial_3_3_Shared.png");

  return 0;
}

