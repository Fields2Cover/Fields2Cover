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

  std::cout << "####### Tutorial 4.1 Brute force swath generator ######" << std::endl;
  f2c::obj::NSwath n_swath_obj;
  f2c::sg::BruteForce bf_sw_gen;
  F2CSwaths swaths_bf_nswath = bf_sw_gen.generateBestSwaths(n_swath_obj, robot.getCovWidth(), no_hl.getGeometry(0));


  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(swaths_bf_nswath);
  f2c::Visualizer::save("Tutorial_4_1_Brute_force_NSwath.png");


  f2c::obj::SwathLength swathlength_obj;
  F2CSwaths swaths_bf_swathlength = bf_sw_gen.generateBestSwaths(swathlength_obj, robot.getCovWidth(), no_hl.getGeometry(0));


  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(swaths_bf_swathlength);
  f2c::Visualizer::save("Tutorial_4_1_Brute_force_SwathLength.png");


  F2CSwaths swaths_bf_angle = bf_sw_gen.generateSwaths(M_PI, robot.getCovWidth(), no_hl.getGeometry(0));
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(swaths_bf_angle);
  f2c::Visualizer::save("Tutorial_4_1_Brute_force_Angle.png");

  return 0;
}

