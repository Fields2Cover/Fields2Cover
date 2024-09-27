//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Tom Schut
//                        BSD-3 License
//=============================================================================

#include "fields2cover.h"
#include <iostream>

// Imports transport lanes and working lanes from the data folder.
// Transport lanes are specific lanes where the robot can move, in contrast to
// headlands Calculates the route

int main() {
  F2CSwaths imported_swaths = f2c::Parser::importSwathsJson(
      std::string(DATA_PATH) + "workinglanes.geojson");

  F2CFields transport_lanes_import;
  f2c::Parser::importJson(std::string(DATA_PATH) + "transportlanes.geojson",
      transport_lanes_import);

  F2CCells transport_lanes_cells;
  for (auto& field : transport_lanes_import) {
    transport_lanes_cells.addGeometry(
        field.getCellsAbsPosition().getGeometry(0));
  }

  F2CRobot robot(2.0, 2.0, 3.14, 3.14);
  f2c::hg::ConstHL const_hl;
  f2c::sg::BruteForce bf;

  f2c::rp::SnakeOrder snake_sorter;
  auto swaths = snake_sorter.genSortedSwaths(imported_swaths);

  f2c::rp::BoustrophedonOrder boustrophedon_sorter;
  swaths = boustrophedon_sorter.genSortedSwaths(imported_swaths);

  f2c::rp::SpiralOrder spiral;
  swaths = spiral.genSortedSwaths(imported_swaths);

  f2c::rp::RoutePlannerBase route_planner;
  auto swathsByCells = new f2c::types::SwathsByCells();

  swathsByCells->emplace_back(swaths);
  route_planner.setStartAndEndPoint(
      //   F2CPoint(-25.433197685573184, -36.10820096737386));
      F2CPoint(-25.433197685573184, -36.10820096737386),
      F2CPoint(-3.12291100766424, 9.92889852672394));
  F2CRoute route = route_planner.genRoute(
      transport_lanes_cells, *swathsByCells, true, false, false);
  F2CRoute sroute = route_planner.genShortestRoute(transport_lanes_cells,
      *swathsByCells, F2CPoint(-2.41464793852428, 19.8445814946835),
      F2CPoint(-25.433197685573184, -36.10820096737386));

  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurves dubins;
  f2c::pp::ReedsSheppCurvesHC reeds_shepp_hc;
  f2c::pp::ReedsSheppCurves reeds_shepp;

  robot.setMinTurningRadius(0.5);  // m
  // F2CPath path = path_planner.planPath(robot, route, dubins, true);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::plot(transport_lanes_cells);
  f2c::Visualizer::plot(route);
  //   f2c::Visualizer::plot(sroute);
  //   f2c::Visualizer::plot(F2CPoint(104.710141268895995, 10.460095828578901));
  f2c::Visualizer::show();
  f2c::Visualizer::save("Tutorial_9_1.png");

  return 0;
}
