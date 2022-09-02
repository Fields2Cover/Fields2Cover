//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include "fields2cover.h"
#include <iostream>

int main() {
  f2c::Random rand(42);
  F2CField field = rand.generateRandField(5, 1e4);
  F2CCells cells = field.field;
  F2CRobot robot (2.0, 26.0);

  std::cout << "####### Tutorial 3.1 Constant width headland generator ######" << std::endl;
  f2c::hg::ConstHL const_hl;
  F2CCells no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.robot_width);
  std::cout << "The complete area is " << cells.getArea() <<
    ", and the area without headlands is " << no_hl.getArea() << std::endl;

  f2c::Visualizer::figure(1);
  f2c::Visualizer::plot(field);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::save("Tutorial_3_1_Const_width");

  return 0;
}

