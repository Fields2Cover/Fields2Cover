//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover.h"
#include <iostream>

int main()
{
    // Import field
    // F2CField field = f2c::Parser::importFieldGml(std::string(DATA_PATH) + "test1.xml");
    auto field_json = f2c::Parser::importCellJson(std::string(DATA_PATH) + "field.geojson");
    auto field_cells = F2CCells(field_json);
    auto field = F2CField(field_cells);
    F2CSwaths imported_swaths = f2c::Parser::importSwathsJson(std::string(DATA_PATH) + "workinglanes.geojson");

    F2CFields fields;
    f2c::Parser::importJson(std::string(DATA_PATH) + "transportlanes.geojson", fields);
    // convert fields to cell array
    F2CCells transport_lanes_cells;
    for (auto& field : fields) {
        transport_lanes_cells.addGeometry(field.getCellsAbsPosition().getGeometry(0));
    }

    // F2CField orig_field = field.clone();
    // Transform into UTM to work in meters
    // f2c::Transform::transformToUTM(field);

    F2CRobot robot(2.0, 6.0);
    f2c::hg::ConstHL const_hl;
    // F2CCells path_middle_hl = const_hl.generateHeadlands(field_cells, 3.0 * robot.getWidth());
    f2c::sg::BruteForce bf;
    // F2CSwaths swaths = bf.generateSwaths(M_PI, robot.getCovWidth(), no_hl.getGeometry(0));

    // this is the route planner, it sorts the swaths, that is all.
    f2c::rp::SnakeOrder snake_sorter;
    auto swaths = snake_sorter.genSortedSwaths(imported_swaths);

    f2c::rp::BoustrophedonOrder boustrophedon_sorter;
    // auto swaths = boustrophedon_sorter.genSortedSwaths(imported_swaths);

    f2c::rp::RoutePlannerBase route_planner;
    auto swathsByCells = new f2c::types::SwathsByCells();
    swathsByCells->emplace_back(swaths);

    F2CRoute route = route_planner.genRoute(transport_lanes_cells, *swathsByCells);

    // the path planner actually creates a path from the sorted swaths.
    f2c::pp::PathPlanning path_planner;
    f2c::pp::DubinsCurves dubins;
    robot.setMinTurningRadius(2); // m
    F2CPath path = path_planner.planPath(robot, route, dubins);

    f2c::Visualizer::figure();
    // f2c::Visualizer::plot(field_cells);
    // f2c::Visualizer::plot(transport_lanes_cells);
    // f2c::Visualizer::plot(swaths);

    f2c::Visualizer::plot(route);
    // f2c::Visualizer::show();
    f2c::Visualizer::plot(path);
    f2c::Visualizer::show();
    // f2c::Visualizer::save("Tutorial_8_1_UTM.png");

    // f2c::Visualizer::plot(route);
    // f2c::Visualizer::plot(path);
    // f2c::Visualizer::show();
    // Transform the generated path back to the previousa CRS.
    // F2CPath path_gps = f2c::Transform::transformToPrevCRS(path, field);
    // f2c::Transform::transformToPrevCRS(field);

    // f2c::Visualizer::figure();
    // f2c::Visualizer::plot(field.getCellsAbsPosition());
    // f2c::Visualizer::plot(path_gps);
    // f2c::Visualizer::save("Tutorial_8_1_GPS.png");

    return 0;
}
