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
  robot.setMinTurningRadius(3.0);
  f2c::hg::ReqHL req_hl;
  F2CCells req_no_hl = req_hl.generateHeadlands(cells, robot, {0.0});
  std::cout << "With swaths at 0 rad, the area without headlands is " <<
    req_no_hl.area() << ", while a constant headland leaves " <<
    const_hl.generateHeadlands(cells, robot, {0.0}).area() << std::endl;

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(field);
  f2c::Visualizer::plot(req_no_hl);
  f2c::Visualizer::save("Tutorial_3_2_Req_width.png");

  return 0;
}

