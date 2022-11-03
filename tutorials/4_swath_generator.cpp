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

  std::cout << "####### Tutorial 4.1 Brute force swath generator ######" << std::endl;
  f2c::obj::NSwath n_swath_obj;
  f2c::sg::BruteForce bf_sw_gen;
  F2CSwaths swaths_bf_nswath = bf_sw_gen.generateBestSwaths(n_swath_obj, robot.op_width, no_hl.getGeometry(0));


  f2c::Visualizer::figure(4);
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(swaths_bf_nswath);
  f2c::Visualizer::save("Tutorial_4_1_Brute_force_NSwath");


  f2c::obj::SwathLength swathlength_obj;
  F2CSwaths swaths_bf_swathlength = bf_sw_gen.generateBestSwaths(swathlength_obj, robot.op_width, no_hl.getGeometry(0));


  f2c::Visualizer::figure(5);
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(swaths_bf_swathlength);
  f2c::Visualizer::save("Tutorial_4_1_Brute_force_SwathLength");


  F2CSwaths swaths_bf_angle = bf_sw_gen.generateSwaths(M_PI, robot.op_width, no_hl.getGeometry(0));
  f2c::Visualizer::figure(6);
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(swaths_bf_angle);
  f2c::Visualizer::save("Tutorial_4_1_Brute_force_Angle");

  return 0;
}

