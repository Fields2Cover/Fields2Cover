//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include <iostream>
#include "fields2cover.h"

int main() {
  // Import field
  F2CField field = f2c::Parser::importFieldGml(std::string(DATA_PATH) + "test1.xml");
  F2CRobot robot (2.0, 6.0, 0.5, 0.2);

  F2CRoute cov_route = f2c::planCovRoute(robot, field, false);
  F2CPath cov_path = f2c::planCovPath(robot, field, false);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(field.getCellsAbsPosition());
  f2c::Visualizer::plot(cov_route);
  f2c::Visualizer::save("cov_route_quick_start.png");

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(field.getCellsAbsPosition());
  f2c::Visualizer::plot(cov_path);
  f2c::Visualizer::show();
  f2c::Visualizer::save("cov_path_quick_start.png");

  return 0;
}


