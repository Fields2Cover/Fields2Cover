//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include "fields2cover.h"
#include <iostream>

int main() {
  // Import field
  F2CField field = f2c::Parser::importFieldGml(std::string(DATA_PATH) + "test1.xml");
  F2CField orig_field = field.clone();
  // Transform into UTM to work in meters
  f2c::Transform::transformToUTM(field);

  F2CRobot robot (2.0, 6.0);
  f2c::hg::ConstHL const_hl;
  F2CCells no_hl = const_hl.generateHeadlands(field.getField(), 3.0 * robot.getWidth());
  f2c::sg::BruteForce bf;
  F2CSwaths swaths = bf.generateSwaths(M_PI, robot.getCovWidth(), no_hl.getGeometry(0));
  f2c::rp::SnakeOrder snake_sorter;
  swaths = snake_sorter.genSortedSwaths(swaths);
  f2c::pp::PathPlanning path_planner;
  robot.setMinTurningRadius(2);  // m
  f2c::pp::DubinsCurves dubins;
  F2CPath path = path_planner.planPath(robot, swaths, dubins);


  f2c::Visualizer::figure();
  f2c::Visualizer::plot(field);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(path);
  f2c::Visualizer::save("Tutorial_8_1_UTM.png");


  // Transform the generated path back to the previousa CRS.
  F2CPath path_gps = f2c::Transform::transformToPrevCRS(path, field);
  f2c::Transform::transformToPrevCRS(field);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(orig_field.getCellsAbsPosition());
  f2c::Visualizer::plot(path_gps);
  f2c::Visualizer::save("Tutorial_8_1_GPS.png");

  return 0;
}


