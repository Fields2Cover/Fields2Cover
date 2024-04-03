//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/path_planning/dubins_curves.h"
#include "fields2cover/path_planning/path_planning.h"
#include "fields2cover/path_planning/dubins_curves_cc.h"
#include "fields2cover/route_planning/boustrophedon_order.h"
#include "fields2cover/swath_generator/brute_force.h"
#include "../test_helpers/path_planning_checker.hpp"

TEST(fields2cover_types_path, addState) {
  F2CPath path;
  path.addState(F2CPoint(1,2), 0.5*M_PI, 3, f2c::types::PathDirection::FORWARD, f2c::types::PathSectionType::TURN, 5);
  path.addState(F2CPoint(3,4), 5*M_PI, 5, f2c::types::PathDirection::BACKWARD, f2c::types::PathSectionType::SWATH, 3);

  const F2CPath const_path = path;

  auto state0 = path.getState(0);
  auto c_state0 = const_path.getState(0);
  EXPECT_EQ(state0.point, c_state0.point);
  EXPECT_EQ(state0.angle, c_state0.angle);

  EXPECT_EQ(state0.point, F2CPoint(1,2));
  EXPECT_EQ(state0.angle, 0.5*M_PI);
  EXPECT_EQ(state0.len, 3);
  EXPECT_EQ(int(state0.dir), int(f2c::types::PathDirection::FORWARD));
  EXPECT_EQ(int(state0.type), int(f2c::types::PathSectionType::TURN));
  EXPECT_EQ(state0.velocity, 5);

  auto states = path.getStates();
  auto c_states = const_path.getStates();
  EXPECT_EQ(states[0].point,   F2CPoint(1,2));
  EXPECT_EQ(c_states[0].point, F2CPoint(1,2));
  EXPECT_EQ(states[0].point,   path.atStart());
  EXPECT_EQ(c_states[0].point, const_path.atStart());
}



TEST(fields2cover_types_path, getDimMin_getDimMax) {
  F2CPath path;
  path.setStates(std::vector<F2CPathState>(3));
  path[0].point = F2CPoint(1, 4);
  path[1].point = F2CPoint(2, -2);
  path[2].point = F2CPoint(-3, 0);

  EXPECT_NEAR(path.getDimMinX(), -3, 1e-7);
  EXPECT_NEAR(path.getDimMinY(), -2, 1e-7);
  EXPECT_NEAR(path.getDimMaxX(), 2, 1e-7);
  EXPECT_NEAR(path.getDimMaxY(), 4, 1e-7);
}

TEST(fields2cover_types_path, mirrorX_mirrorY) {
  F2CPath path;
  path.setStates(std::vector<F2CPathState>(4));
  path[0].point = F2CPoint(1, 1);
  path[0].angle = 0;
  path[1].point = F2CPoint(2, 1);
  path[1].angle = 1.5*M_PI;
  path[2].point = F2CPoint(2, -2);
  path[2].angle = M_PI;
  path[3].point = F2CPoint(-2, -2);
  path[3].angle = 0;

  path.mirrorX();
  EXPECT_NEAR(path[0].point.getX(), -1, 1e-7);
  EXPECT_NEAR(path[0].point.getY(), 1, 1e-7);
  EXPECT_NEAR(path[0].angle, M_PI, 1e-7);
  EXPECT_NEAR(path[1].point.getX(), -2, 1e-7);
  EXPECT_NEAR(path[1].point.getY(), 1, 1e-7);
  EXPECT_NEAR(path[1].angle, 1.5*M_PI, 1e-7);
  EXPECT_NEAR(path[2].point.getX(), -2, 1e-7);
  EXPECT_NEAR(path[2].point.getY(), -2, 1e-7);
  EXPECT_NEAR(path[2].angle, 0, 1e-7);
  EXPECT_NEAR(path[3].point.getX(), 2, 1e-7);
  EXPECT_NEAR(path[3].point.getY(), -2, 1e-7);
  EXPECT_NEAR(path[3].angle, M_PI, 1e-7);

  path.mirrorY();
  EXPECT_NEAR(path[0].point.getX(), -1, 1e-7);
  EXPECT_NEAR(path[0].point.getY(), -1, 1e-7);
  EXPECT_NEAR(path[0].angle, M_PI, 1e-7);
  EXPECT_NEAR(path[1].point.getX(), -2, 1e-7);
  EXPECT_NEAR(path[1].point.getY(), -1, 1e-7);
  EXPECT_NEAR(path[1].angle, .5*M_PI, 1e-7);
  EXPECT_NEAR(path[2].point.getX(), -2, 1e-7);
  EXPECT_NEAR(path[2].point.getY(), 2, 1e-7);
  EXPECT_NEAR(path[2].angle, 0, 1e-7);
  EXPECT_NEAR(path[3].point.getX(), 2, 1e-7);
  EXPECT_NEAR(path[3].point.getY(), 2, 1e-7);
  EXPECT_NEAR(path[3].angle, M_PI, 1e-7);
}

TEST(fields2cover_types_path, setters) {
  F2CPath path;
  path.setStates(std::vector<F2CPathState>(4));
  path[0].point = F2CPoint(1, 1);
  path[0].angle = 0;
  path[1].point = F2CPoint(2, 1);
  path[1].angle = 1.5*M_PI;
  path[2].point = F2CPoint(2, -2);
  path[2].angle = M_PI;
  path[3].point = F2CPoint(-2, -2);
  path[3].angle = 0;

  path.setBackwardDir();
  for (auto&& s : path) {
    EXPECT_EQ(static_cast<int>(s.dir),
              static_cast<int>(f2c::types::PathDirection::BACKWARD));
  }
  path.setForwardDir();
  for (auto&& s : path) {
    EXPECT_EQ(static_cast<int>(s.dir),
              static_cast<int>(f2c::types::PathDirection::FORWARD));
  }
  path.setSwathType();
  for (auto&& s : path) {
    EXPECT_EQ(static_cast<int>(s.type),
              static_cast<int>(f2c::types::PathSectionType::SWATH));
  }
  path.setTurnType();
  for (auto&& s : path) {
    EXPECT_EQ(static_cast<int>(s.type),
              static_cast<int>(f2c::types::PathSectionType::TURN));
  }
}

TEST(fields2cover_types_path, at) {
  F2CPath path;
  path.setStates(std::vector<F2CPathState>(3));
  path[0].point = F2CPoint(-3, 1);
  path[0].angle = 0;
  path[0].len = 5.0;
  path[1].point = F2CPoint(2, 1);
  path[1].angle = 0.5*M_PI;
  path[1].len = 2.0;
  path[2].point = F2CPoint(2, 3);
  path[2].angle = 0;
  path[2].len = 3.0;
  path[2].dir = f2c::types::PathDirection::BACKWARD;

  EXPECT_NEAR(path.length(), 10.0, 1e-7);

  EXPECT_NEAR(path.at(0).point.getX(), -3, 1e-7);
  EXPECT_NEAR(path.at(0).point.getY(), 1, 1e-7);
  EXPECT_NEAR(path.at(0).angle, 0, 1e-7);
  EXPECT_NEAR(path.at(-1).point.getX(), -3, 1e-7);
  EXPECT_NEAR(path.at(-1).point.getY(), 1, 1e-7);
  EXPECT_NEAR(path.at(-1).angle, 0, 1e-7);

  EXPECT_NEAR(path.at(2.5).point.getX(), -0.5, 1e-7);
  EXPECT_NEAR(path.at(2.5).point.getY(), 1, 1e-7);
  EXPECT_NEAR(path.at(2.5).angle, 0, 1e-7);

  EXPECT_NEAR(path.at(6).point.getX(), 2, 1e-7);
  EXPECT_NEAR(path.at(6).point.getY(), 2, 1e-7);
  EXPECT_NEAR(path.at(6).angle, 0.5*M_PI, 1e-7);

  EXPECT_NEAR(path.at(7).point.getX(), 2, 1e-7);
  EXPECT_NEAR(path.at(7).point.getY(), 3, 1e-7);
  EXPECT_NEAR(path.at(7).angle, 0.5*M_PI, 1e-7);

  EXPECT_NEAR(path.at(8).point.getX(), 1, 1e-7);
  EXPECT_NEAR(path.at(8).point.getY(), 3, 1e-7);
  EXPECT_NEAR(path.at(8).angle, 0, 1e-7);

  EXPECT_NEAR(path.at(10).point.getX(), -1, 1e-7);
  EXPECT_NEAR(path.at(10).point.getY(), 3, 1e-7);
  EXPECT_NEAR(path.at(10).angle, 0, 1e-7);
  EXPECT_NEAR(path.at(17).point.getX(), -1, 1e-7);
  EXPECT_NEAR(path.at(17).point.getY(), 3, 1e-7);
  EXPECT_NEAR(path.at(17).angle, 0, 1e-7);

}


TEST(fields2cover_types_path, appendSwath) {
  F2CLineString line1({F2CPoint(0.0, 1.0), F2CPoint(1.0, 1.0), F2CPoint(1.0, 4.0)});
  F2CSwath swath1(line1);
  F2CPath path1;
  path1.appendSwath(swath1, 2.0);
  EXPECT_EQ(path1.size(), 2);
  EXPECT_EQ(path1[0].point.getX(), 0.0);
  EXPECT_EQ(path1[0].point.getY(), 1.0);
  EXPECT_EQ(path1[0].velocity, 2.0);
  EXPECT_EQ(path1[0].len, 1.0);
  EXPECT_EQ(static_cast<int>(path1[0].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1[1].point.getX(), 1.0);
  EXPECT_EQ(path1[1].point.getY(), 1.0);
  EXPECT_EQ(path1[1].velocity, 2.0);
  EXPECT_EQ(path1[1].len, 3.0);
  EXPECT_EQ(static_cast<int>(path1[1].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.getTaskTime(), 2.0);
}

TEST(fields2cover_types_path, op_plus_equal) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 1.0), F2CPoint(1.0, 1.0), F2CPoint(1.0, 4.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(1.0, 4.0), F2CPoint(1.0, 0.0), F2CPoint(0.0, 0.0)}));

  F2CPath path1, path2;
  path1.appendSwath(swath1, 2.0);
  path2.appendSwath(swath2, 1.0);
  path1 += path2;
  EXPECT_EQ(path1[0].point.getX(), 0.0);
  EXPECT_EQ(path1[0].point.getY(), 1.0);
  EXPECT_EQ(path1[0].len, 1.0);
  EXPECT_EQ(path1[0].velocity, 2.0);
  EXPECT_EQ(static_cast<int>(path1[0].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1[1].point.getX(), 1.0);
  EXPECT_EQ(path1[1].point.getY(), 1.0);
  EXPECT_EQ(path1[1].velocity, 2.0);
  EXPECT_EQ(path1[1].len, 3.0);
  EXPECT_EQ(static_cast<int>(path1[1].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1[2].point.getX(), 1.0);
  EXPECT_EQ(path1[2].point.getY(), 4.0);
  EXPECT_EQ(path1[2].velocity, 1.0);
  EXPECT_EQ(path1[2].len, 4.0);
  EXPECT_EQ(static_cast<int>(path1[2].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1[3].point.getX(), 1.0);
  EXPECT_EQ(path1[3].point.getY(), 0.0);
  EXPECT_EQ(path1[3].velocity, 1.0);
  EXPECT_EQ(path1[3].len, 1.0);
  EXPECT_EQ(static_cast<int>(path1[3].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.getTaskTime(), 7.0);
  EXPECT_EQ(path1.size(), 4);
}

TEST(fields2cover_types_path, populate_and_reduce) {
  F2CPath path1, path2;
  EXPECT_EQ(path1.size(), 0);
  EXPECT_EQ(path1.length(), 0.0);

  path1.populate(200);
  EXPECT_EQ(path1.size(), 0);
  EXPECT_EQ(path1.length(), 0.0);
  EXPECT_NEAR(path1.getTaskTime(), 0.0, 1e-6);

  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0), F2CPoint(2.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(2.0, 1.0), F2CPoint(2.0, 4.0), F2CPoint(1.0, 4.0)}));

  path1.appendSwath(swath1, 2.0);
  path2.appendSwath(swath2, 1.0);
  path1 += path2;

  EXPECT_EQ(path1.size(), 4);
  EXPECT_EQ(path1.length(), 7.0);
  EXPECT_NEAR(path1.getTaskTime(), 5.5, 1e-6);
  for (auto s : path1) {
    EXPECT_EQ(static_cast<int>(s.dir),
        static_cast<int>(f2c::types::PathDirection::FORWARD));
  }
  auto path_c = path1;
  path1.populate(200);
  EXPECT_EQ(path1.size(), 200);
  EXPECT_NEAR(path1.length(), 7.0, 1e-2);
  EXPECT_NEAR(path1.getTaskTime(), 5.5, 1e-2);
  for (auto s : path1) {
    EXPECT_EQ(static_cast<int>(s.dir),
        static_cast<int>(f2c::types::PathDirection::FORWARD));
    EXPECT_EQ(static_cast<int>(s.type),
        static_cast<int>(f2c::types::PathSectionType::SWATH));
  }
  path1.populate(10);
  EXPECT_EQ(path1.size(), 10);
  EXPECT_NEAR(path1.length(), 7.0, 1e-2);
  EXPECT_NEAR(path1.getTaskTime(), 5.5, 0.5);
  for (auto&& s : path1) {
    EXPECT_EQ(static_cast<int>(s.dir),
        static_cast<int>(f2c::types::PathDirection::FORWARD));
    EXPECT_EQ(static_cast<int>(s.type),
        static_cast<int>(f2c::types::PathSectionType::SWATH));
  }
  path1.populate(500);
  EXPECT_EQ(path1.size(), 500);
  EXPECT_NEAR(path1.length(), 7.0, 1e-2);
  EXPECT_NEAR(path1.getTaskTime(), 5.5, 0.5);
  path1.reduce(0.1);
  EXPECT_NEAR(path1.length(), 7.0, 1e-2);
  EXPECT_NEAR(path1.getTaskTime(), 5.5, 0.5);
  path1.reduce(0.1);
  EXPECT_NEAR(path1.length(), 7.0, 1e-2);
  EXPECT_NEAR(path1.getTaskTime(), 5.5, 0.5);
  EXPECT_LT(path1.size(), 100);

  for (auto s : path1) {
    EXPECT_EQ(static_cast<int>(s.dir),
        static_cast<int>(f2c::types::PathDirection::FORWARD));
    EXPECT_EQ(static_cast<int>(s.type),
        static_cast<int>(f2c::types::PathSectionType::SWATH));
  }
}

TEST(fields2cover_types_path, length) {
  F2CLineString line1, line2;
  line1.addPoint( 0.0, 1.0);
  line1.addPoint( 1.0, 1.0);
  line1.addPoint( 1.0, 4.0);
  line2.addPoint( 1.0, 4.0);
  line2.addPoint( 1.0, 0.0);
  line2.addPoint( 0.0, 0.0);
  F2CSwath swath1(line1);
  F2CSwath swath2(line2);

  F2CPath path1, path2;
  path1.appendSwath(swath1, 2.0);
  EXPECT_EQ(path1.length(), 4.0);
  path2.appendSwath(swath2, 1.0);
  path1 += path2;
  EXPECT_EQ(path1.length(), 9.0);
  path1 = path2;
  EXPECT_EQ(path1.length(), 5.0);
}

TEST(fields2cover_types_path, serialize) {
  F2CPath path, path_read;
  f2c::types::PathState state;

  EXPECT_EQ(0, path.serializePath().compare(""));
  state.point = F2CPoint(2.3, -5);
  state.angle = 0.1;
  state.velocity = 3.0;
  state.len = 6.0;
  state.dir = f2c::types::PathDirection::FORWARD;
  state.type = f2c::types::PathSectionType::TURN;
  path.addState(state);

  EXPECT_EQ(path.serializePath(),
        "2.3 -5 0 0.1 3 6 1 2\n");

  state.point = F2CPoint(0.1234567890123, -9.87654);
  state.angle = 0.2;
  state.velocity = 3.2;
  state.len = 6.2;
  state.dir = f2c::types::PathDirection::BACKWARD;
  state.type = f2c::types::PathSectionType::TURN;
  path.addState(state);

  EXPECT_EQ(path.serializePath(10),
        "2.3 -5 0 0.1 3 6 1 2\n0.123456789 -9.87654 0 0.2 3.2 6.2 -1 2\n");

  path.saveToFile("/tmp/test_path");
  path_read.loadFile("/tmp/test_path");
  EXPECT_EQ(path.serializePath(), path_read.serializePath());
}


TEST(fields2cover_types_path, moveTo) {
  F2CLineString line1({F2CPoint(0.0, 1.0), F2CPoint(1.0, 1.0), F2CPoint(1.0, 4.0)});
  F2CSwath swath1(line1);
  F2CPath path1;
  path1.appendSwath(swath1, 2.0);
  path1.moveTo(F2CPoint(100, 200));
  EXPECT_EQ(path1[0].point.getX(), 100.0);
  EXPECT_EQ(path1[0].point.getY(), 201.0);
  EXPECT_EQ(path1[0].velocity, 2.0);
  EXPECT_EQ(path1[0].len, 1.0);
  EXPECT_EQ(static_cast<int>(path1[0].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1[1].point.getX(), 101.0);
  EXPECT_EQ(path1[1].point.getY(), 201.0);
  EXPECT_EQ(path1[1].velocity, 2.0);
  EXPECT_EQ(path1[1].len, 3.0);
  EXPECT_EQ(static_cast<int>(path1[1].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.size(), 2);
}


TEST(fields2cover_types_path, toLineString) {
  F2CPath path;
  path.setStates(std::vector<F2CPathState>(3));
  path[0].point = F2CPoint(-3, 1);
  path[0].angle = 0.0;
  path[0].len = 5.0;
  path[1].point = F2CPoint(2, 1);
  path[1].angle = 0.5 * M_PI;
  path[1].len = 2.0;
  path[2].point = F2CPoint(2, 3);
  path[2].angle = 0.0;
  path[2].len = -3.0;

  F2CLineString line = path.toLineString();

  EXPECT_EQ(line.size(), path.size() + 1);
  EXPECT_NEAR(line.getGeometry(0).getX(), -3, 1e-7);
  EXPECT_NEAR(line.getGeometry(0).getY(), 1, 1e-7);
  EXPECT_NEAR(line.getGeometry(1).getX(), 2, 1e-7);
  EXPECT_NEAR(line.getGeometry(1).getY(), 1, 1e-7);
  EXPECT_NEAR(line.getGeometry(2).getX(), 2, 1e-7);
  EXPECT_NEAR(line.getGeometry(2).getY(), 3, 1e-7);
  EXPECT_NEAR(line.getGeometry(3).getX(), -1, 1e-7);
  EXPECT_NEAR(line.getGeometry(3).getY(), 3, 1e-7);
}

TEST(fields2cover_types_path, discretize_swath) {
  // Define field and robot
  F2CRobot robot (2.0, 5.0);
  double field_Y = 100;
  double field_X = 40;
  F2CCells field(F2CCell(F2CLinearRing({F2CPoint(0,0), F2CPoint(0,field_Y),
                                        F2CPoint(field_X,field_Y), F2CPoint(field_X,0),
                                        F2CPoint(0,0)})));
  // Swath generation
  f2c::sg::BruteForce bf;
  F2CSwaths swaths = bf.generateSwaths(0, robot.getCovWidth(), field.getGeometry(0));
  f2c::rp::BoustrophedonOrder boustrophedon_sorter;
  auto boustrophedon_swaths = boustrophedon_sorter.genSortedSwaths(swaths, 1);
  // Path planner
  f2c::pp::PathPlanning path_planner;
  robot.setMinTurningRadius(2.5);  // m
  // Create swath connections using Dubins curves with Continuous curvature
  f2c::pp::DubinsCurvesCC dubins_cc;
  F2CPath path_dubins_cc = path_planner.planPath(robot, boustrophedon_swaths, dubins_cc);
  EXPECT_TRUE(IsPathCorrect(path_dubins_cc,
        path_dubins_cc[0].point, path_dubins_cc[0].angle,
        path_dubins_cc.atEnd(), path_dubins_cc.back().angle, false));

  /**
   *  Test if step size is correct when step_size can be divided into swath length without leaving
   *  a residual
   */
  // Discretize swath lines in path object
  // Specify the step size and the robot velocity for the swath section
  double discretize_step_size = 2; // Step size for discretization in [m]
  F2CPath new_path = path_dubins_cc.discretizeSwath(discretize_step_size);
  for (size_t i = 0; i < new_path.size()-1; i++) {
    if (new_path[i].type == f2c::types::PathSectionType::SWATH &&
        new_path[i+1].type == f2c::types::PathSectionType::SWATH) {
      EXPECT_NEAR(new_path[i].point.distance(new_path[i+1].point),
                  discretize_step_size, 1e-6);
    }
  }
  EXPECT_TRUE(IsPathCorrect(new_path,
        path_dubins_cc[0].point, path_dubins_cc[0].angle,
        path_dubins_cc.atEnd(), path_dubins_cc.back().angle, false));

  /**
   *  Test if discretize is correct if the step size is greater than the swath distance
  */
  double discretize_step_size_2 = 150; // Step size for discretization in [m]
  F2CPath new_path_2 = path_dubins_cc.discretizeSwath(discretize_step_size_2);
  for (size_t i = 0; i < new_path_2.size()-1; i++) {
    if (new_path_2[i].type == f2c::types::PathSectionType::SWATH &&
        new_path_2[i+1].type == f2c::types::PathSectionType::SWATH) {
      EXPECT_NEAR(new_path_2[i].point.distance(new_path_2[i+1].point),
                    field_Y, 1e-6);
    }
  }
  EXPECT_TRUE(IsPathCorrect(new_path_2,
        path_dubins_cc[0].point, path_dubins_cc[0].angle,
        path_dubins_cc.atEnd(), path_dubins_cc.back().angle, false));

  /**
   * Test a simple single diagonal swath
  */
  F2CPath path;
  F2CPathState state1;
  state1.point = F2CPoint(0, 0);
  state1.angle = 0.5 * boost::math::constants::half_pi<double>();
  state1.len = F2CPoint(10, 10).distance(state1.point);
  state1.type = f2c::types::PathSectionType::SWATH;
  F2CPathState state2;
  state2.point = F2CPoint(10, 10);
  state2.angle = 0.5 * boost::math::constants::half_pi<double>();
  state2.type = f2c::types::PathSectionType::SWATH;
  path.addState(state1);
  path.addState(state2);
  // Discretize swath with a step size
  double step_size_4 = 5;
  double approximated_step_size_4 = 4.71405;
  F2CPath discretized_path = path.discretizeSwath(step_size_4);
  // Check if the discretized path has the expected number of states
  EXPECT_EQ(discretized_path.size(), 4);  // Original start, one midpoint, and original end
  // Check if the first and last point is equal to original start and end
  EXPECT_EQ(discretized_path[0].point, state1.point);
  EXPECT_EQ(discretized_path[2].atEnd(), state2.point);
  // Check the midpoint's step_size
  EXPECT_NEAR(discretized_path[0].point.distance(discretized_path[1].point),
      approximated_step_size_4 , 1e-4);
  EXPECT_NEAR(discretized_path[1].point.distance(discretized_path[2].point),
      approximated_step_size_4 , 1e-4);

  EXPECT_TRUE(IsPathCorrect(discretized_path, state1.point,
        0.5*boost::math::constants::half_pi<double>(),
      state2.point, 0.5 * boost::math::constants::half_pi<double>(), false));
}

