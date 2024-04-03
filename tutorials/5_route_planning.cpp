//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include "fields2cover.h"
#include <iostream>

int main() {

  std::cout << "####### Tutorial 5.1 Route planning for all swaths ######" << std::endl;

  F2CRobot robot_c (1.0);
  F2CCells cells_c {
    F2CCell(F2CLinearRing({
          F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)
    }))
  };
  cells_c.addRing(0, F2CLinearRing({
        F2CPoint(.4,.4), F2CPoint(.4,.6),F2CPoint(.6,.6),F2CPoint(.6,.4), F2CPoint(.4,.4)
        }));
  cells_c.addRing(0, F2CLinearRing({
        F2CPoint(1.2,1.2), F2CPoint(1.2,1.6),F2CPoint(1.6,1.6),F2CPoint(1.6,1.2), F2CPoint(1.2,    1.2)
        }));
  cells_c *= 3e1;

  f2c::hg::ConstHL const_hl;
  F2CCells mid_hl_c = const_hl.generateHeadlands(cells_c, 1.5 * robot_c.getWidth());
  F2CCells no_hl_c = const_hl.generateHeadlands(cells_c, 3.0 * robot_c.getWidth());

  f2c::sg::BruteForce bf;
  F2CSwathsByCells swaths_c = bf.generateSwaths(M_PI/2.0, robot_c.getCovWidth(), no_hl_c);

  f2c::rp::RoutePlannerBase route_planner;
  F2CRoute route = route_planner.genRoute(mid_hl_c, swaths_c);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells_c);
  f2c::Visualizer::plot(no_hl_c);
  f2c::Visualizer::xlim(-5,65);
  f2c::Visualizer::ylim(-5,65);
  f2c::Visualizer::save("Tutorial_5_0_field.png");

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells_c);
  f2c::Visualizer::plot(no_hl_c);
  f2c::Visualizer::plot(route);
  f2c::Visualizer::xlim(-5,65);
  f2c::Visualizer::ylim(-5,65);
  f2c::Visualizer::save("Tutorial_5_0_route.png");




  std::cout << "####### Tutorial 5.2 Known Patterns ######" << std::endl;


  f2c::Random rand(42);
  F2CRobot robot (2.0, 6.0);
  F2CCells cells = rand.generateRandField(1e4, 5).getField();
  F2CCells no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.getWidth());
  F2CSwaths swaths = bf.generateSwaths(M_PI, robot.getCovWidth(), no_hl.getGeometry(0));

  std::cout << "####### Tutorial 5.2.1 Boustrophedon ######" << std::endl;
  f2c::rp::BoustrophedonOrder boustrophedon_sorter;
  auto boustrophedon_swaths = boustrophedon_sorter.genSortedSwaths(swaths);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(boustrophedon_swaths);
  f2c::Visualizer::save("Tutorial_5_1_Boustrophedon_1.png");


  for (int i = 1; i < 4; ++i) {
    f2c::Visualizer::figure();
    f2c::Visualizer::plot(cells);
    f2c::Visualizer::plot(no_hl);
    f2c::Visualizer::plot(boustrophedon_sorter.genSortedSwaths(swaths, i));
    f2c::Visualizer::save("Tutorial_5_1_Boustrophedon_" + std::to_string(i+1) + ".png" );
  }


  std::cout << "####### Tutorial 5.2.2 Snake order ######" << std::endl;
  f2c::rp::SnakeOrder snake_sorter;
  auto snake_swaths = snake_sorter.genSortedSwaths(swaths);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(snake_swaths);
  f2c::Visualizer::save("Tutorial_5_2_Snake_1.png");

  for (int i = 1; i < 4; ++i) {
    f2c::Visualizer::figure();
    f2c::Visualizer::plot(cells);
    f2c::Visualizer::plot(no_hl);
    f2c::Visualizer::plot(snake_sorter.genSortedSwaths(swaths, i));
    f2c::Visualizer::save("Tutorial_5_2_Snake_" + std::to_string(i+1) + ".png");
  }

  swaths = bf.generateSwaths(M_PI, robot.getCovWidth(), no_hl.getGeometry(0));
  std::cout << "####### Tutorial 5.2.3 Spiral order ######" << std::endl;
  f2c::rp::SpiralOrder spiral_sorter(6);
  auto spiral_swaths = spiral_sorter.genSortedSwaths(swaths);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(spiral_swaths);
  f2c::Visualizer::save("Tutorial_5_3_Spiral_1.png");

  for (int i = 1; i < 4; ++i) {
    f2c::Visualizer::figure();
    f2c::Visualizer::plot(cells);
    f2c::Visualizer::plot(no_hl);
    f2c::Visualizer::plot(spiral_sorter.genSortedSwaths(swaths, i));
    f2c::Visualizer::save("Tutorial_5_3_Spiral_" + std::to_string(i+1) + ".png");
  }

  return 0;
}

