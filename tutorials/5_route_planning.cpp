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
  f2c::sg::BruteForce bf;
  F2CSwaths swaths = bf.generateSwaths(M_PI, robot.op_width, no_hl.getGeometry(0));

  std::cout << "####### Tutorial 5.1 Boustrophedon ######" << std::endl;
  f2c::rp::BoustrophedonOrder boustrophedon_sorter;
  auto boustrophedon_swaths = boustrophedon_sorter.genSortedSwaths(swaths);

  f2c::Visualizer::figure(11);
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(boustrophedon_swaths);
  f2c::Visualizer::save("Tutorial_5_1_Boustrophedon_1");


  for (int i = 1; i < 4; ++i) {
    f2c::Visualizer::figure(11 + i);
    f2c::Visualizer::plot(cells);
    f2c::Visualizer::plot(no_hl);
    f2c::Visualizer::plot(boustrophedon_sorter.genSortedSwaths(swaths, i));
    f2c::Visualizer::save("Tutorial_5_1_Boustrophedon_" + std::to_string(i+1));
  }


  std::cout << "####### Tutorial 5.2 Snake order ######" << std::endl;
  f2c::rp::SnakeOrder snake_sorter;
  auto snake_swaths = snake_sorter.genSortedSwaths(swaths);

  f2c::Visualizer::figure(21);
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(snake_swaths);
  f2c::Visualizer::save("Tutorial_5_2_Snake_1");

  for (int i = 1; i < 4; ++i) {
    f2c::Visualizer::figure(21 + i);
    f2c::Visualizer::plot(cells);
    f2c::Visualizer::plot(no_hl);
    f2c::Visualizer::plot(snake_sorter.genSortedSwaths(swaths, i));
    f2c::Visualizer::save("Tutorial_5_2_Snake_" + std::to_string(i+1));
  }

  swaths = bf.generateSwaths(M_PI, robot.op_width, no_hl.getGeometry(0));
  std::cout << "####### Tutorial 5.3 Spiral order ######" << std::endl;
  f2c::rp::SpiralOrder spiral_sorter(6);
  auto spiral_swaths = spiral_sorter.genSortedSwaths(swaths);

  f2c::Visualizer::figure(31);
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(spiral_swaths);
  f2c::Visualizer::save("Tutorial_5_3_Spiral_1");

  for (int i = 1; i < 4; ++i) {
    f2c::Visualizer::figure(31 + i);
    f2c::Visualizer::plot(cells);
    f2c::Visualizer::plot(no_hl);
    f2c::Visualizer::plot(spiral_sorter.genSortedSwaths(swaths, i));
    f2c::Visualizer::save("Tutorial_5_3_Spiral_" + std::to_string(i+1));
  }

  return 0;
}

