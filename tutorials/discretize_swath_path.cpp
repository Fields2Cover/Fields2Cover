#include "fields2cover.h"
#include <iostream>

int main() {
    // Define field, headland and robot
    f2c::Random rand(42);
    F2CRobot robot (2.0, 5.0);
    F2CCells field = rand.generateRandField(8, 1e4).field;
    f2c::hg::ConstHL const_hl;
    F2CCells no_hl = const_hl.generateHeadlands(field, 2.0 * robot.robot_width);

    // Swath generation
    f2c::sg::BruteForce bf;
    f2c::obj::NSwath n_swath_obj;
    F2CSwaths swaths = bf.generateSwaths(M_PI, robot.op_width, no_hl.getGeometry(0));
    f2c::rp::BoustrophedonOrder boustrophedon_sorter;
    auto boustrophedon_swaths = boustrophedon_sorter.genSortedSwaths(swaths, 1);

    // Path planner
    f2c::pp::PathPlanning path_planner;
    robot.setMinRadius(2.5);  // m
    // robot.linear_curv_change = 0.5;

    // Create swath connections using Dubins curves with Continuous curvature
    f2c::pp::DubinsCurvesCC dubins_cc;
    F2CPath path_dubins_cc = path_planner.searchBestPath(robot, boustrophedon_swaths, dubins_cc);

    // Discretize swath lines in path object
    // Specify the step size and the robot velocity for the swath section
    double discretize_step_size = 0.1; // Step size for discretization in [m]
    F2CPath new_path = path_dubins_cc.discretize_swath(discretize_step_size, robot.cruise_speed);

    // Save to file
    // new_path.saveToFile("discretized_swath_path.csv", 5); // Specify precision to the significant number

    // Visualize
    f2c::Visualizer::figure();
    f2c::Visualizer::plot(field);
    f2c::Visualizer::plot(no_hl);
    f2c::Visualizer::plot(new_path);
    f2c::Visualizer::plot(boustrophedon_swaths);
    f2c::Visualizer::show();
    f2c::Visualizer::save("discretized_swath_figure");

    return 0;
}


