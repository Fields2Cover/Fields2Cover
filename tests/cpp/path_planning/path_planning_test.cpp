//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <numeric>
#include <fstream>
#include "fields2cover/types.h"
#include "fields2cover/path_planning/dubins_curves.h"
#include "fields2cover/path_planning/path_planning.h"
#include "fields2cover/path_planning/dubins_curves_cc.h"
#include "fields2cover/route_planning/boustrophedon_order.h"
#include "fields2cover/swath_generator/brute_force.h"

TEST(fields2cover_pp_pp, turn_dist) {
  F2CLineString path1, path2, path3;
  path1.addPoint( 0.0, 0.0);
  path1.addPoint( 0.0, 1.0);
  path2.addPoint( 3.0, 1.0);
  path2.addPoint( 3.0, 0.0);
  F2CSwath swath1(path1);
  F2CSwath swath2(path2);
  F2CSwaths swaths {swath1, swath2};

  F2CRobot robot;
  robot.max_icc = 1.0 / 1.5;
  f2c::pp::PathPlanning path_planner;
  path_planner.turn_point_dist = 0.1;
  f2c::pp::DubinsCurves dubins;

  auto path = path_planner.searchBestPath(robot, swaths, dubins);
  // Precision is quite low so this is just checking a prediction is done
  EXPECT_NEAR(path.length(), 2 + 1.5 * M_PI, 0.1);
}

TEST(fields2cover_pp_pp, discretize_swath) {
    // Define field and robot
    F2CRobot robot (2.0, 5.0);
    double field_Y = 100;
    double field_X = 40;
    F2CCells field(F2CCell(F2CLinearRing({F2CPoint(0,0), F2CPoint(0,field_Y),
                                          F2CPoint(field_X,field_Y), F2CPoint(field_X,0),
                                          F2CPoint(0,0)})));
    // Swath generation
    f2c::sg::BruteForce bf;
    F2CSwaths swaths = bf.generateSwaths(M_PI, robot.op_width, field.getGeometry(0));
    f2c::rp::BoustrophedonOrder boustrophedon_sorter;
    auto boustrophedon_swaths = boustrophedon_sorter.genSortedSwaths(swaths, 1);
    // Path planner
    f2c::pp::PathPlanning path_planner;
    robot.setMinRadius(2.5);  // m
    // Create swath connections using Dubins curves with Continuous curvature
    f2c::pp::DubinsCurvesCC dubins_cc;
    F2CPath path_dubins_cc = path_planner.searchBestPath(robot, boustrophedon_swaths, dubins_cc);

    /**
     *  Test if step size is correct when step_size can be divided into swath length without leaving
     *  a residual
     */
    // Discretize swath lines in path object
    // Specify the step size and the robot velocity for the swath section
    double discretize_step_size = 2; // Step size for discretization in [m]
    F2CPath new_path = path_dubins_cc.discretize_swath(discretize_step_size);
    for (size_t i = 0; i < new_path.size()-1; i++) {
        if (new_path.states.at(i).type == f2c::types::PathSectionType::SWATH &&
            new_path.states.at(i+1).type == f2c::types::PathSectionType::SWATH) {
            std::cout << "Distance = "
            << new_path.states.at(i).point.Distance(new_path.states.at(i+1).point) << std::endl;
            EXPECT_NEAR(new_path.states.at(i).point.Distance(new_path.states.at(i+1).point),
                        discretize_step_size, 1e-6);
        }
    }

    /**
     *  Test if discretize is correct if the step size is greater than the swath distance
    */
    double discretize_step_size_2 = 150; // Step size for discretization in [m]
    F2CPath new_path_2 = path_dubins_cc.discretize_swath(discretize_step_size_2);
    for (size_t i = 0; i < new_path_2.size()-1; i++) {
        if (new_path_2.states.at(i).type == f2c::types::PathSectionType::SWATH &&
            new_path_2.states.at(i+1).type == f2c::types::PathSectionType::SWATH) {
            EXPECT_NEAR(new_path_2.states.at(i).point.Distance(new_path_2.states.at(i+1).point),
                        field_Y, 1e-6);
        }
    }

    /**
     *  Test if discretize is correct when the step size is approximated
    */
    double discretize_step_size_3 = 30; // Step size for discretization in [m]
    double approximated_step_size_3 = 33.333333; // Known approx step size
    F2CPath new_path_3 = path_dubins_cc.discretize_swath(discretize_step_size_3);
    for (size_t i = 0; i < new_path_3.size()-1; i++) {
        if (new_path_3.states.at(i).type == f2c::types::PathSectionType::SWATH &&
            new_path_3.states.at(i+1).type == f2c::types::PathSectionType::SWATH) {
            EXPECT_NEAR(new_path_3.states.at(i).point.Distance(new_path_3.states.at(i+1).point),
                        approximated_step_size_3, 1e-6);
        }
    }

    /**
     * Test a simple single diagonal swath
    */
    F2CPath path;
    F2CPathState state1;
    state1.point = F2CPoint(0, 0, 0);
    state1.type = f2c::types::PathSectionType::SWATH;
    F2CPathState state2;
    state2.point = F2CPoint(10, 10, 10);
    state2.type = f2c::types::PathSectionType::SWATH;
    path.states.push_back(state1);
    path.states.push_back(state2);
    // Discretize swath with a step size
    double step_size_4 = 5;
    double approximated_step_size_4 = 4.71405;
    F2CPath discretized_path = path.discretize_swath(step_size_4);
    // Check if the discretized path has the expected number of states
    EXPECT_EQ(discretized_path.size(), 4);  // Original start, one midpoint, and original end
    // Check if the first and last point is equal to original start and end
    EXPECT_EQ(discretized_path.states.at(0).point, state1.point);
    EXPECT_EQ(discretized_path.states.at(3).point, state2.point);
    // Check the midpoint's step_size
    EXPECT_NEAR(discretized_path.states.at(0).point.Distance(discretized_path.states.at(1).point),
                approximated_step_size_4 , 1e-4);
    EXPECT_NEAR(discretized_path.states.at(1).point.Distance(discretized_path.states.at(2).point),
                approximated_step_size_4 , 1e-4);
    EXPECT_NEAR(discretized_path.states.at(2).point.Distance(discretized_path.states.at(3).point),
                approximated_step_size_4 , 1e-4);
}

