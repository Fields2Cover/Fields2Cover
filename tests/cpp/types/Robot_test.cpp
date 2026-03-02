//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover.h"
#include "../test_helpers/robot_data.hpp"
#include "../test_helpers/path_planning_checker.hpp"

TEST(fields2cover_types_robot, init_implement) {
  F2CRobot robot(1.0, 2.0);

  // --- No implement attached ---
  EXPECT_TRUE(robot.isRobotSymmetric());
  EXPECT_DOUBLE_EQ(robot.getRobotWidth(),   1.0);
  EXPECT_DOUBLE_EQ(robot.getRobotLength(),  0.0);

  EXPECT_DOUBLE_EQ(robot.getImplWidth(),        0.0);
  EXPECT_DOUBLE_EQ(robot.getImplLength(),       0.0);
  EXPECT_DOUBLE_EQ(robot.getImplCovWidth(),     1.0);  // defaults to robot width
  EXPECT_DOUBLE_EQ(robot.getImplCovLength(),    0.0);

  EXPECT_EQ(robot.getCenterImpl(),    F2CPoint(0.0, 0.0));
  EXPECT_EQ(robot.getCenterImplCov(), F2CPoint(0.0, 0.0));

  EXPECT_EQ(robot.getBackImplCov(),  F2CPoint(0.0, 0.0));
  EXPECT_EQ(robot.getFrontImplCov(), F2CPoint(0.0, 0.0));

  EXPECT_EQ(
    robot.getFrontImplCov(F2CPoint(10, 100), M_PI_2),
    F2CPoint(10.0, 100.0)
  );

  // --- Check velocities ---
  robot.setCruiseVel(5.0);
  EXPECT_DOUBLE_EQ(robot.getCruiseVel(true),  5.0);
  EXPECT_DOUBLE_EQ(robot.getCruiseVel(false),  5.0);

  robot.setTurnVel(3.0);
  EXPECT_DOUBLE_EQ(robot.getTurnVel(true),  3.0);
  EXPECT_DOUBLE_EQ(robot.getTurnVel(false),  3.0);

  // --- Check curvatures ---
  robot.setMaxCurv(0.2);
  EXPECT_DOUBLE_EQ(robot.getMaxCurv(true),   0.2);
  EXPECT_DOUBLE_EQ(robot.getMaxCurv(false),  0.2);

  EXPECT_NEAR(robot.getMinTurningRadius(), 1.0/0.2, 1e-5);

  robot.setMaxDiffCurv(0.02);
  EXPECT_DOUBLE_EQ(robot.getMaxDiffCurv(true),   0.02);
  EXPECT_DOUBLE_EQ(robot.getMaxDiffCurv(false),  0.02);

  robot.setMinTurningRadius(4.0);
  EXPECT_NEAR(robot.getMinTurningRadius(), 4.0, 1e-5);
  EXPECT_NEAR(robot.getMaxCurv(true),   0.25, 1e-5);
  EXPECT_NEAR(robot.getMaxCurv(false),  0.25, 1e-5);



  // --- Attach implement ---
  f2c::types::GenericImplement impl;
  impl.c_impl  = F2CPoint(-0.75, 0.0);
  impl.c_cov   = F2CPoint(-0.1,  0.0);
  impl.width   = 0.5;
  impl.length  = 1.8;
  impl.cov_width  = 0.3;
  impl.cov_length = 1.6;

  impl.max_icc_impl_off            = 1.0 / 1.5;
  impl.max_icc_impl_on             = 1.0 / 20.0;
  impl.linear_curv_change_impl_off = 0.01;
  impl.linear_curv_change_impl_on  = 0.1;
  impl.vel_impl_on  = 10.0;
  impl.vel_impl_off = 20.0;

  robot.setImpl(F2CPoint(-4.0, 0.0), impl);

  EXPECT_TRUE(robot.isRobotSymmetric());

  // --- Velocities ---
  EXPECT_DOUBLE_EQ(robot.getCruiseVel(true),  10.0);
  EXPECT_DOUBLE_EQ(robot.getCruiseVel(false), 20.0);
  EXPECT_DOUBLE_EQ(robot.getTurnVel(true),    10.0);
  EXPECT_DOUBLE_EQ(robot.getTurnVel(false),   20.0);

  // If implement, velocities are taken from implement
  robot.setTurnVel(15.0);
  EXPECT_DOUBLE_EQ(robot.getTurnVel(true),    10.0);
  EXPECT_DOUBLE_EQ(robot.getTurnVel(false),   20.0);



  // --- Curvature limits ---
  EXPECT_NEAR(robot.getMaxCurv(true),      1.0 / 20.0, 1e-9);
  EXPECT_NEAR(robot.getMaxCurv(false),     1.0 / 1.5,  1e-9);
  EXPECT_NEAR(robot.getMaxDiffCurv(true),  0.1,        1e-9);
  EXPECT_NEAR(robot.getMaxDiffCurv(false), 0.01,       1e-9);

  // --- Implement geometry ---
  EXPECT_DOUBLE_EQ(robot.getImplWidth(),     0.5);
  EXPECT_DOUBLE_EQ(robot.getImplLength(),    1.8);
  EXPECT_DOUBLE_EQ(robot.getImplCovWidth(),  0.3);
  EXPECT_DOUBLE_EQ(robot.getImplCovLength(), 1.6);

  EXPECT_EQ(robot.getCenterImpl(),    F2CPoint(-4.75, 0.0));
  EXPECT_EQ(robot.getCenterImplCov(), F2CPoint(-4.85, 0.0));

  EXPECT_NEAR(robot.getBackImplCov().getX(),  -5.65, 1e-7);
  EXPECT_NEAR(robot.getFrontImplCov().getX(), -4.05, 1e-7);

  // --- World transform checks ---
  EXPECT_EQ(
    robot.getCenterImplCov(F2CPoint(10, 100), M_PI_2),
    F2CPoint(10.0, 95.15)
  );
  EXPECT_EQ(
    robot.getFrontImplCov(F2CPoint(10, 100), M_PI_2),
    F2CPoint(10.0, 95.95)
  );
  EXPECT_EQ(
    robot.getBackImplCov(F2CPoint(10, 100), M_PI_2),
    F2CPoint(10.0, 94.35)
  );

  robot.setWidth(100.7);
  EXPECT_DOUBLE_EQ(robot.getRobotWidth(), 100.7);
  robot.setCovWidth(200.7);
  EXPECT_DOUBLE_EQ(robot.getCovWidth(), 200.7);

}

TEST(fields2cover_types_robot, invalid_widths_throw) {
  EXPECT_THROW(F2CRobot(-1.0, 1.0, 1.0, 1.0), std::out_of_range);
  EXPECT_THROW(F2CRobot( 0.0, 1.0, 1.0, 1.0), std::out_of_range);
  EXPECT_THROW(F2CRobot( 1.0,-1.0, 1.0, 1.0), std::out_of_range);
}

TEST(fields2cover_types_robot, cov_width_zero_defaults_to_width) {
  F2CRobot r(2.0, 0.0, 1.0, 0.1);
  EXPECT_DOUBLE_EQ(r.getCovWidth(), 2.0);
}

TEST(fields2cover_types_robot, getImpl_empty_and_attached) {
  F2CRobot robot(2.0);

  // No implement
  EXPECT_FALSE(robot.getImpl().has_value());

  // Attach implement
  f2c::types::GenericImplement impl;
  impl.width  = 1.0;
  impl.length = 2.0;
  F2CPoint attach(-3.0, 0.5);

  robot.setImpl(attach, impl);

  auto impl_opt = robot.getImpl();
  ASSERT_TRUE(impl_opt.has_value());
  EXPECT_EQ(impl_opt->first, attach);
  EXPECT_DOUBLE_EQ(impl_opt->second.width,  1.0);
  EXPECT_DOUBLE_EQ(impl_opt->second.length, 2.0);
}

TEST(fields2cover_types_robot, getImpl_non_const_reference) {
  F2CRobot robot(2.0);

  f2c::types::GenericImplement impl;
  impl.width = 0.5;
  robot.setImpl(F2CPoint(-1.0, 0.0), impl);

  EXPECT_DOUBLE_EQ(robot.getImpl()->second.width, 0.5);

  auto& impl_ref = robot.getImpl();
  ASSERT_TRUE(impl_ref.has_value());

  impl_ref->second.width = 1.5;

  EXPECT_DOUBLE_EQ(robot.getImpl()->second.width, 1.5);
}

TEST(fields2cover_types_robot, getWidth_robot_only) {
  F2CRobot robot(2.0);
  robot.setRobotLength(4.0);

  EXPECT_DOUBLE_EQ(robot.getWidth(), 2.0);
}

TEST(fields2cover_types_robot, getWidth_with_centered_implement) {
  F2CRobot robot(2.0);
  robot.setRobotLength(4.0);

  f2c::types::GenericImplement impl;
  impl.width  = 1.0;
  impl.length = 1.0;
  impl.c_impl = F2CPoint(0.0, 0.0);

  robot.setImpl(F2CPoint(-2.0, 0.0), impl);

  // Robot wider than implement
  EXPECT_DOUBLE_EQ(robot.getWidth(), 2.0);
}

TEST(fields2cover_types_robot, getWidth_with_lateral_offset_implement) {
  F2CRobot robot(2.0);
  robot.setRobotLength(4.0);

  f2c::types::GenericImplement impl;
  impl.width  = 1.0;
  impl.length = 1.0;
  impl.c_impl = F2CPoint(0.0, 1.0);  // lateral offset

  robot.setImpl(F2CPoint(-2.0, 0.0), impl);

  // Robot half width = 1.0
  // Implement spans [1.0 - 0.5, 1.0 + 0.5] => [0.5, 1.5]
  // Total width = 1.5 - (-1.0) = 2.5
  EXPECT_DOUBLE_EQ(robot.getWidth(), 2.5);
}

TEST(fields2cover_types_robot, getLength_robot_only) {
  F2CRobot robot(2.0);
  robot.setRobotLength(6.0);

  EXPECT_DOUBLE_EQ(robot.getLength(), 6.0);
}

TEST(fields2cover_types_robot, getLength_with_implement_extending_back) {
  F2CRobot robot(2.0);
  robot.setRobotLength(4.0);

  f2c::types::GenericImplement impl;
  impl.length = 2.0;
  impl.c_impl = F2CPoint(-1.0, 0.0);

  robot.setImpl(F2CPoint(-3.0, 0.0), impl);

  // Robot: [-2, 2]
  // Impl center: -4 -> [-5, -3]
  EXPECT_DOUBLE_EQ(robot.getLength(), 7.0);
}

TEST(fields2cover_types_robot, name_set_and_get) {
  F2CRobot robot(1.0);

  EXPECT_TRUE(robot.getName().empty());

  robot.setName("tractor_A");
  EXPECT_EQ(robot.getName(), "tractor_A");

  robot.setName("robot_42");
  EXPECT_EQ(robot.getName(), "robot_42");
}


TEST(fields2cover_types_robot, setCruiseVel_with_implement) {
  F2CRobot robot(1.0);
  robot.setCruiseVel(300.7);
  EXPECT_DOUBLE_EQ(robot.getCruiseVel(true), 300.7);
  EXPECT_DOUBLE_EQ(robot.getCruiseVel(false), 300.7);

  f2c::types::GenericImplement impl;
  robot.setImpl(F2CPoint(-4.0, 0.0), impl);

  robot.setCruiseVel(400.7);

  // If implement is attached, cruise vel is gotten from implement if any
  EXPECT_DOUBLE_EQ(robot.getCruiseVel(true), 400.7);
  EXPECT_DOUBLE_EQ(robot.getCruiseVel(false), 400.7);

  impl.vel_impl_on  = 10.0;
  robot.setImpl(F2CPoint(-4.0, 0.0), impl);
  EXPECT_DOUBLE_EQ(robot.getCruiseVel(true), 10.0);
  EXPECT_DOUBLE_EQ(robot.getCruiseVel(false), 400.7);

  impl.vel_impl_off = 20.0;
  robot.setImpl(F2CPoint(-4.0, 0.0), impl);
  EXPECT_DOUBLE_EQ(robot.getCruiseVel(true), 10.0);
  EXPECT_DOUBLE_EQ(robot.getCruiseVel(false), 20.0);
}








/*
TEST(fields2cover_types_robot, init) {
  F2CRobot robot_def_const(1., 2.);
  EXPECT_NEAR(robot_def_const.getWidth(),    1., 1e-5);
  EXPECT_NEAR(robot_def_const.getCovWidth(), 2., 1e-5);

  F2CRobot robot;
  robot.setName("robot test");
  EXPECT_EQ(robot.getName(), "robot test");
  robot.setWidth(5.0);
  EXPECT_NEAR(robot.getWidth(),              5.0, 1e-5);
  robot.setCovWidth(10.0);
  EXPECT_NEAR(robot.getCovWidth(),           10., 1e-5);

  robot.setMinTurningRadius(10.0);
  EXPECT_NEAR(robot.getMaxCurv(),            0.1, 1e-5);
  EXPECT_NEAR(robot.getMinTurningRadius(),   10., 1e-5);

  robot.setMaxCurv(0.4);
  EXPECT_NEAR(robot.getMaxCurv(),            0.4, 1e-5);
  EXPECT_NEAR(robot.getMinTurningRadius(),   2.5, 1e-5);
  robot.setMaxDiffCurv(0.3);
  EXPECT_NEAR(robot.getMaxDiffCurv(),        0.3, 1e-5);

  robot.setCruiseVel(2.0);
  EXPECT_NEAR(robot.getCruiseVel(),          2.0, 1e-5);
  EXPECT_NEAR(robot.getTurnVel(),            2.0, 1e-5);
  robot.setTurnVel(5.0);
  EXPECT_NEAR(robot.getTurnVel(),            5.0, 1e-5);


  EXPECT_THROW(F2CRobot(0), std::out_of_range);
  EXPECT_THROW(F2CRobot(-3), std::out_of_range);

  F2CRobot r1 = robot;
  EXPECT_NEAR(r1.getWidth(), 5.0, 1e-5);

  const F2CRobot r2 {robot};
  EXPECT_EQ(r2.getWidth(), 5.0);
}

TEST(fields2cover_types_robot, implement_attach_and_get) {
  F2CRobot r;
  F2CPoint p(1.0, 2.0);

  f2c::types::GenericImplement impl;
  impl.width = 2.0;
  impl.length = 4.0;

  r.setImpl(p, impl);

  ASSERT_TRUE(r.getImpl().has_value());
  EXPECT_EQ(r.getImpl()->first, p);
  EXPECT_NEAR(r.getImpl()->second.width, 2.0, 1e-6);
}

TEST(fields2cover_types_robot, width_length_no_implement) {
  F2CRobot r;
  r.setWidth(2.0);
  r.setRobotLength(6.0);

  EXPECT_NEAR(r.getWidth(), 2.0, 1e-6);
  EXPECT_NEAR(r.getLength(), 6.0, 1e-6);
}
*/

/*
TEST(fields2cover_types_robot, init_implement) {
  F2CRobot robot(1., 2.);
  EXPECT_TRUE(robot.isRobotSymmetric());
  EXPECT_EQ(robot.getRobotWidth(),            1.);
  EXPECT_EQ(robot.getRobotLength(),           2.);
  EXPECT_EQ(robot.getImplWidth(),             0.);
  EXPECT_EQ(robot.getImplLength(),            0.);
  EXPECT_EQ(robot.getImplCovWidth(),          1.);
  EXPECT_EQ(robot.getImplCovLength(),         0.);
  EXPECT_EQ(robot.getCenterImpl().getX(),     0.);
  EXPECT_EQ(robot.getCenterImpl().getY(),     0.);
  EXPECT_EQ(robot.getCenterImplCov().getX(), -1.);
  EXPECT_EQ(robot.getCenterImplCov().getY(),  0.);
  EXPECT_EQ(robot.getBackImplCov().getX(),    -1.);
  EXPECT_EQ(robot.getBackImplCov().getY(),    0.);
  EXPECT_EQ(robot.getFrontImplCov().getX(),   -1.);
  EXPECT_EQ(robot.getFrontImplCov().getY(),   0.);
  EXPECT_EQ(robot.getFrontImplCov(F2CPoint(10, 100), .5 * M_PI).getX(),  10);
  EXPECT_EQ(robot.getFrontImplCov(F2CPoint(10, 100), .5 * M_PI).getY(),  99);
  EXPECT_EQ(robot.getBackImplCov(F2CPoint(10, 100), .5 * M_PI).getX(),  10);
  EXPECT_EQ(robot.getBackImplCov(F2CPoint(10, 100), .5 * M_PI).getY(),  99);

  robot.center_rot = F2CPoint(-0.5, 0);
  f2c::types::GenericImplement impl;
  impl.c_impl = F2CPoint(-0.75, 0);
  impl.c_cov = F2CPoint(-0.1, 0);
  impl.width = 0.5;
  impl.length = 1.8;
  impl.cov_width = 0.3;
  impl.cov_length = 1.6;
  impl.max_icc_impl_off = 1./1.5;
  impl.max_icc_impl_on = 1./20.;
  impl.linear_curv_change_impl_off = 0.01;
  impl.linear_curv_change_impl_on = 0.1;
  impl.vel_impl_on = 10;
  impl.vel_impl_off = 20;
  robot.implement = std::make_pair(F2CPoint(-4, 0), impl);
  EXPECT_TRUE(robot.isRobotSymmetric());
  EXPECT_EQ(robot.getRobotWidth(),            1.00);
  EXPECT_EQ(robot.getRobotLength(),           2.00);
  EXPECT_EQ(robot.getCruiseVel(true),         10.0);
  EXPECT_EQ(robot.getCruiseVel(false),        20.0);
  EXPECT_EQ(robot.getTurnVel(true),           10.0);
  EXPECT_EQ(robot.getTurnVel(false),          20.0);
  EXPECT_EQ(robot.getMaxCurv(true),           1.0/20.);
  EXPECT_EQ(robot.getMaxCurv(false),          1.0/1.5);
  EXPECT_EQ(robot.getMaxDiffCurv(true),       0.1);
  EXPECT_EQ(robot.getMaxDiffCurv(false),      0.01);
  EXPECT_EQ(robot.getImplWidth(),             0.50);
  EXPECT_EQ(robot.getImplLength(),            1.80);
  EXPECT_EQ(robot.getImplCovWidth(),          0.30);
  EXPECT_EQ(robot.getImplCovLength(),         1.60);
  EXPECT_EQ(robot.getCenterImpl().getX(),    -4.75);
  EXPECT_EQ(robot.getCenterImpl().getY(),     0.00);
  EXPECT_EQ(robot.getCenterImplFromCRot().getX(),    -5.25);
  EXPECT_EQ(robot.getCenterImplFromCRot().getY(),     0.00);
  EXPECT_EQ(robot.getCenterImplCov().getX(), -4.85);
  EXPECT_EQ(robot.getCenterImplCov().getY(),  0.00);
  EXPECT_EQ(robot.getCenterImplCovFromCRot().getX(), -5.35);
  EXPECT_EQ(robot.getCenterImplCovFromCRot().getY(),  0.00);
  EXPECT_NEAR(robot.getBackImplCov().getX(), -5.65, 1e-7);
  EXPECT_EQ(robot.getBackImplCov().getY(),    0.00);
  EXPECT_EQ(robot.getFrontImplCov().getX(),  -4.05);
  EXPECT_EQ(robot.getFrontImplCov().getY(),   0.00);
  EXPECT_EQ(robot.getCenterImplCov(F2CPoint(10, 100), .5*M_PI), F2CPoint(10., 94.65));
  EXPECT_EQ(robot.getFrontImplCov(F2CPoint(10, 100),  .5*M_PI), F2CPoint(10., 95.45));
  EXPECT_EQ(robot.getBackImplCov(F2CPoint(10, 100),   .5*M_PI), F2CPoint(10., 93.85));
  EXPECT_EQ(robot.getFrontImplCov(F2CPoint(10, 100),    M_PI), F2CPoint(14.55, 100));
  EXPECT_EQ(robot.getBackImplCov(F2CPoint(10, 100),     M_PI), F2CPoint(16.15, 100));
}

TEST(fields2cover_types_robot, init) {
  F2CRobot robot(3);
  EXPECT_TRUE(robot.isRobotSymmetric());
  EXPECT_EQ(robot.getRobotWidth(),           3.);
  EXPECT_EQ(robot.getRobotLength(),          0.);

  EXPECT_THROW( F2CRobot(0), std::out_of_range);
  EXPECT_THROW( F2CRobot(-3), std::out_of_range);
  EXPECT_THROW( F2CRobot(3, -3), std::out_of_range);

  auto r1 = robot;
  EXPECT_EQ(r1.robot_width, 3);
  EXPECT_EQ(r1.getRobotWidth(), 3);

  auto r2 {robot};
  EXPECT_EQ(r2.robot_width, 3);
  EXPECT_EQ(r2.getRobotWidth(), 3);
}


TEST(fields2cover_types_robot, check_wheels) {
  F2CRobot robot = getAgbotWithImpl();
  F2CRobot simp_robot = getSimpleRobot();
  EXPECT_EQ(robot.getNumRobotWheels(), 2);
  EXPECT_EQ(robot.getNumImplWheels(), 4);
  EXPECT_EQ(robot.getNumWheels(), 6);

  EXPECT_EQ(simp_robot.getNumRobotWheels(), 0);
  EXPECT_EQ(simp_robot.getNumImplWheels(), 0);
  EXPECT_EQ(simp_robot.getNumWheels(), 0);

  EXPECT_EQ(robot.getWheel(0).size(), 4);
  EXPECT_EQ(robot.getWheel(2).size(), 1);
  EXPECT_EQ(robot.getRobotWheel(0).getPressurePoint(0), F2CPoint( 0.5, 0.0));
  EXPECT_EQ(robot.getRobotWheel(0).getPressurePoint(1), F2CPoint( 0.25, 0.0));
  EXPECT_EQ(robot.getRobotWheel(0).getPressurePoint(2), F2CPoint(-0.25, 0.0));
  EXPECT_EQ(robot.getRobotWheel(0).getPressurePoint(3), F2CPoint(-0.5, 0.0));
  EXPECT_EQ(robot.getRobotWheel(1).getPressurePoint(0), F2CPoint( 0.5, 0.0));
  EXPECT_EQ(robot.getRobotWheel(1).getPressurePoint(1), F2CPoint( 0.25, 0.0));
  EXPECT_EQ(robot.getRobotWheel(1).getPressurePoint(2), F2CPoint(-0.25, 0.0));
  EXPECT_EQ(robot.getRobotWheel(1).getPressurePoint(3), F2CPoint(-0.5, 0.0));
  EXPECT_EQ(robot.getRobotWheel(0).getPressurePoints().size(), 4);
  EXPECT_EQ(robot.getRobotWheel(0).getPressurePoints().getLength(), 1.0);
  EXPECT_EQ(robot.getRobotWheel(1).getPressurePoints().getLength(), 1.0);
  EXPECT_NEAR(robot.getImplWheel(0).getWidth(), 0.2, 1e-7);
  EXPECT_EQ(robot.getImplWheel(0).getPressurePoints().size(), 1);
  EXPECT_EQ(robot.getImplWheel(0).getPressurePoints().getLength(), 0.0);

  EXPECT_EQ(robot.getWheelPos(0, F2CPoint(1, 2), M_PI * 0.5), F2CPoint(-0.15, 2.0));
  EXPECT_EQ(robot.getWheelPos(2, F2CPoint(1, 2), M_PI * 0.5), F2CPoint(-0.4, 1.4));

  EXPECT_THROW(robot.getRobotWheel(100), std::out_of_range);
  EXPECT_THROW(robot.getRobotWheelPos(100), std::out_of_range);
  EXPECT_THROW(robot.getImplWheel(100), std::out_of_range);
  EXPECT_THROW(robot.getImplWheelPos(100), std::out_of_range);

  EXPECT_THROW(simp_robot.getImplWheel(100), std::out_of_range);
  EXPECT_THROW(simp_robot.getImplWheelPos(100), std::out_of_range);

  EXPECT_EQ(robot.getWheel(0).getAngleWheel(0), 0);
}





TEST(DISABLED_fields2cover_types_robot, getTracks) {
  F2CRobot robot(2.5, 2.5);
  robot.setMinTurningRadius(2.0);
  robot.linear_curv_change = 2.0;
  f2c::types::RobotWheel wheel1;
  wheel1.addPressurePoint(F2CPoint(0.5,0.0));
  wheel1.addPressurePoint(F2CPoint(0.25,0.));
  wheel1.addPressurePoint(F2CPoint(-0.25,0));
  wheel1.addPressurePoint(F2CPoint(-0.5,0.0));
  f2c::types::RobotWheel wheel2;
  wheel1.setWidth(0.1);
  wheel2.setWidth(0.2);
  robot.wheels.emplace_back(F2CPoint(0, 1.15), wheel1);
  robot.wheels.emplace_back(F2CPoint(0, -1.15), wheel2);

  F2CSwaths swaths ({
      F2CSwath(F2CLineString({F2CPoint(0,0),F2CPoint(0,10)})),
      F2CSwath(F2CLineString({F2CPoint(4,10),F2CPoint(4,0)})),
      F2CSwath(F2CLineString({F2CPoint(6,5),F2CPoint(-2,5)}))});
  f2c::pp::DubinsCurvesCC dubins;
  f2c::pp::PathPlanning path_planner;
  auto path = path_planner.searchBestPath(robot, swaths, dubins);

  F2CCells wheel0_tracks = robot.getTrackWheelOnPressurePoints(0, path);
  EXPECT_EQ(wheel0_tracks.size(), 4);

  EXPECT_NEAR(9.13, wheel0_tracks.UnionCascaded().getArea(), 1e-1);
  F2CCells wheel1_tracks = robot.getTrackWheelOnPressurePoints(1, path);
  EXPECT_EQ(wheel1_tracks.size(), 1);
  EXPECT_NEAR(19.55, wheel1_tracks.getArea(), 1e-1);

  F2CCells tracks = robot.getTracks(path);
  EXPECT_EQ(tracks.size(), 1);
  EXPECT_NEAR(28.32, tracks.getArea(), 1e-1);
  EXPECT_GE(wheel0_tracks.UnionCascaded().getArea() +
      wheel1_tracks.getArea(), tracks.getArea());
}



TEST(DISABLED_fields2cover_types_robot, getDetailedTracks) {
  F2CRobot robot(2.5, 2.5);
  robot.setMinTurningRadius(2.0);
  robot.linear_curv_change = 2.0;
  f2c::types::RobotWheel wheel1;
  wheel1.addPressurePoint(F2CPoint(0.5,0.0));
  wheel1.addPressurePoint(F2CPoint(0.25,0.));
  wheel1.addPressurePoint(F2CPoint(-0.25,0));
  wheel1.addPressurePoint(F2CPoint(-0.5,0.0));
  f2c::types::RobotWheel wheel2;
  wheel1.setWidth(0.1);
  wheel2.setWidth(0.2);
  robot.wheels.emplace_back(F2CPoint(0, 1.15), wheel1);
  robot.wheels.emplace_back(F2CPoint(0, -1.15), wheel2);

  F2CSwaths swaths ({
      F2CSwath(F2CLineString({F2CPoint(0,0),F2CPoint(0,10)})),
      F2CSwath(F2CLineString({F2CPoint(4,10),F2CPoint(4,0)})),
      F2CSwath(F2CLineString({F2CPoint(6,5),F2CPoint(-2,5)}))});
  f2c::pp::DubinsCurvesCC dubins;
  f2c::pp::PathPlanning path_planner;
  auto path = path_planner.searchBestPath(robot, swaths, dubins);

  F2CCell tracks = robot.getTrackWheelOnPressurePoint(0, 0, path);

  auto detailed_tracks = robot.getDetailedTrackWheelOnPressurePoint(0, 0, path);
  EXPECT_EQ(detailed_tracks.size(), path.size() - 1);
  EXPECT_GT(tracks.getArea(), detailed_tracks.UnionCascaded().getArea());
  EXPECT_NEAR(9.21353, detailed_tracks.getArea(), 1e-4);

  EXPECT_EQ(detailed_tracks.UnionCascaded().size(), 1);
  EXPECT_EQ(detailed_tracks.UnionCascaded().getGeometry(0).size(), 3);

  ///*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(path);
  //f2c::Visualizer::plot(detailed_tracks, "r");
  //f2c::Visualizer::plot(detailed_tracks.UnionCascaded() , "r");
  //f2c::Visualizer::plot(tracks);
  f2c::Visualizer::axis("equal");
  f2c::Visualizer::show();
  ////
}

TEST(DISABLED_fields2cover_types_robot, getDetailedTracks_tracked_wheel) {
  F2CRobot robot(2.5, 2.5);
  robot.setMinTurningRadius(3.0);
  robot.linear_curv_change = 2.0;
  f2c::types::RobotWheel wheel1;
  //wheel1.addPressurePoint(F2CPoint(1.09,0.0));
  wheel1.addPressurePoint(F2CPoint(0.92,0.0));
  wheel1.addPressurePoint(F2CPoint(0.48,0.));
  wheel1.addPressurePoint(F2CPoint(0.16,0.));
  wheel1.addPressurePoint(F2CPoint(-0.16,0));
  wheel1.addPressurePoint(F2CPoint(-0.48,0));
  wheel1.addPressurePoint(F2CPoint(-0.92,0.0));
  //wheel1.addPressurePoint(F2CPoint(-1.09,0.0));
  wheel1.setWidth(0.61/2.0);
  robot.wheels.emplace_back(F2CPoint(0, 1.125), wheel1);
  robot.wheels.emplace_back(F2CPoint(0, -1.125), wheel1);

  F2CSwaths swaths ({
      F2CSwath(F2CLineString({F2CPoint(0,0),F2CPoint(0,10)})),
      F2CSwath(F2CLineString({F2CPoint(4,10),F2CPoint(4,0)})),
      F2CSwath(F2CLineString({F2CPoint(6,5),F2CPoint(-2,5)}))});
  f2c::pp::DubinsCurvesCC dubins;
  f2c::pp::PathPlanning path_planner;
  auto path = path_planner.searchBestPath(robot, swaths, dubins);

  F2CCell tracks = robot.getTrackWheelOnPressurePoint(0, 0, path);

  auto detailed_tracks = robot.getDetailedTrackWheelOnPressurePoint(0, path);
  auto detailed_tracks2 = robot.getDetailedTrackWheelOnPressurePoint(1, path);
  F2CCells tracks2;
  tracks2.addGeometry(detailed_tracks.getGeometry(0));
  tracks2.addGeometry(detailed_tracks2.getGeometry(0));
  tracks2 = tracks2.UnionCascaded();



  EXPECT_LE(detailed_tracks.size(), path.size() - 1);
  EXPECT_GE(detailed_tracks.size(), 0.7 * path.size() - 1);

  EXPECT_GT(tracks.getArea() - M_PI * wheel1.getWidth() * wheel1.getWidth(),
      detailed_tracks.UnionCascaded().getArea());
  EXPECT_NEAR(9.21353, detailed_tracks.getArea(), 1e-4);

  EXPECT_EQ(detailed_tracks.UnionCascaded().size(), 1);
  EXPECT_EQ(detailed_tracks.UnionCascaded().getGeometry(0).size(), 3);

  F2CCells tracks3;
  for (int j = 0; j < robot.getWheel(0).size(); ++j) {
    tracks3.addGeometry(robot.getDetailedTrackWheelOnPressurePoint(0, j, path).UnionCascaded().getGeometry(0));
    tracks3.addGeometry(robot.getDetailedTrackWheelOnPressurePoint(1, j, path).UnionCascaded().getGeometry(0));
  }


  f2c::Visualizer::figure();
  f2c::Visualizer::plot(path);
  f2c::Visualizer::plot(detailed_tracks, "r");
  //f2c::Visualizer::plot(robot.getTracks(path), "b");
  //f2c::Visualizer::plot(detailed_tracks, "r");
  //f2c::Visualizer::plot(detailed_tracks2, "r");
  //f2c::Visualizer::plot(tracks2,"r");
  //f2c::Visualizer::plot(tracks3.UnionCascaded() , "--g");
  //f2c::Visualizer::axis("equal");
  f2c::Visualizer::show();
}

TEST(DISABLED_fields2cover_types_robot, getTracks1) {
  F2CRobot robot(2.5, 2.5);
  robot.name = "test";
  robot.center_rot = F2CPoint(0,0);
  f2c::types::RobotWheel wheel1;
  wheel1.addPressurePoint(F2CPoint(3,0.0));
  wheel1.addPressurePoint(F2CPoint(-3,0.0));
  //wheel1.addPressurePoint(F2CPoint(-0.1,0.));
  //wheel1.addPressurePoint(F2CPoint(-0.35,0));
  f2c::types::RobotWheel wheel2;
  wheel1.setWidth(0.2);
  wheel2.setWidth(0.02);
  robot.wheels.emplace_back(F2CPoint(0,1.15), wheel1);
  //robot.wheels.emplace_back(F2CPoint(0,-1.15), wheel2);


  f2c::Random rand(42);
  f2c::hg::ConstHL const_hl;
  F2CCells cells = rand.generateRandField(5, 1e4).field;
  F2CCells no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.robot_width);
  f2c::sg::BruteForce bf;
  F2CSwaths swaths = bf.generateSwaths(M_PI, robot.getImplWidth(), no_hl.getGeometry(0));
  f2c::rp::SnakeOrder snake_sorter;
  swaths = snake_sorter.genSortedSwaths(swaths);

  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurves dubins;
  F2CPath path_dubins = path_planner.searchBestPath(robot, swaths, dubins);

  auto tracks = robot.getTracks(path_dubins);

  //f2c::Visualizer::figure();
  //f2c::Visualizer::plot(cells);
  //f2c::Visualizer::plot(no_hl);
  //f2c::Visualizer::plot(tracks);
  //f2c::Visualizer::plot(path_dubins);
  //f2c::Visualizer::plot(robot.getCenterTrackWheelOnPressurePoint(0, 0, path_dubins),"b");
  //f2c::Visualizer::plot(robot.getCenterTrackWheelOnPressurePoint(0, 1, path_dubins),"b");
  //f2c::Visualizer::plot(robot.getCenterTrackWheelOnPressurePoint(0, 2, path_dubins),"b");
  //f2c::Visualizer::plot(robot.getCenterTrackWheelOnPressurePoint(0, 3, path_dubins),"b");
  //f2c::Visualizer::plot(robot.getCenterTrackWheelOnPressurePoint(1, 0, path_dubins),"r");
  //f2c::Visualizer::plot(robot.getTrackWheelOnPressurePoints(1, path_dubins),"y");
  //f2c::Visualizer::axis("equal");
  //f2c::Visualizer::save("test_tracks.png");
  //f2c::Visualizer::show();
}


TEST(fields2cover_types_robot, getCornersRIC) {
  F2CRobot agbot = getAgbotWithImpl();

  auto ps_ric = agbot.getCornersRIC();
  EXPECT_NEAR(2,        agbot.getFrontRobot().getX(),1e-7);
  EXPECT_NEAR(-2,       agbot.getBackRobot().getX(), 1e-7);
  EXPECT_NEAR(-2,       agbot.getFrontImpl().getX(), 1e-7);
  EXPECT_NEAR(-5.6,     agbot.getBackImpl().getX(),  1e-7);
  EXPECT_NEAR(1.5,      agbot.getRobotWidth() * 0.5, 1e-7);
  EXPECT_NEAR(3.15/2.0, agbot.getImplWidth() * 0.5,  1e-7);

  EXPECT_EQ(ps_ric[0].getX(), agbot.getFrontRobot().getX());
  EXPECT_EQ(ps_ric[1].getX(), agbot.getBackRobot().getX());
  EXPECT_EQ(ps_ric[2].getX(), agbot.getFrontRobot().getX());
  EXPECT_EQ(ps_ric[3].getX(), agbot.getBackRobot().getX());
  EXPECT_EQ(ps_ric[4].getX(), agbot.getFrontImpl().getX());
  EXPECT_EQ(ps_ric[5].getX(), agbot.getBackImpl().getX());
  EXPECT_EQ(ps_ric[6].getX(), agbot.getFrontImpl().getX());
  EXPECT_EQ(ps_ric[7].getX(), agbot.getBackImpl().getX());

  EXPECT_EQ(ps_ric[0].getY(), agbot.getRobotWidth() * 0.5);
  EXPECT_EQ(ps_ric[1].getY(), agbot.getRobotWidth() * 0.5);
  EXPECT_EQ(ps_ric[2].getY(), -agbot.getRobotWidth() * 0.5);
  EXPECT_EQ(ps_ric[3].getY(), -agbot.getRobotWidth() * 0.5);
  EXPECT_EQ(ps_ric[4].getY(), agbot.getImplWidth() * 0.5);
  EXPECT_EQ(ps_ric[5].getY(), agbot.getImplWidth() * 0.5);
  EXPECT_EQ(ps_ric[6].getY(), -agbot.getImplWidth() * 0.5);
  EXPECT_EQ(ps_ric[7].getY(), -agbot.getImplWidth() * 0.5);
}



TEST(fields2cover_types_robot, computeAreaCovered) {
  F2CRobot robot = getSimpleRobot();

  F2CPathSimp path_simp;
  path_simp.appendStraightLine(10);
  F2CPath path = path_simp.toPath();
  F2CCells cov = robot.computeAreaCovered(path);
  EXPECT_NEAR(cov.getArea(), 10, 1e-7);

  path_simp.appendClothoid(-10, -0.1, false);
  path = path_simp.toPath();
  EXPECT_TRUE(IsPathCorrect(path));
  cov = robot.computeAreaCovered(path);
  EXPECT_NEAR(cov.getArea(), 10, 1e-7);

  path_simp.appendStraightLine(3);
  path = path_simp.toPath();
  EXPECT_TRUE(IsPathCorrect(path));
  cov = robot.computeAreaCovered(path);
  EXPECT_NEAR(cov.getArea(), 13, 1e-7);
}

TEST(fields2cover_types_robot, computeAreaTravelled) {
  F2CRobot robot = getSimpleRobot();

  F2CPathSimp path_simp;
  path_simp.appendStraightLine(10);
  F2CPath path = path_simp.toPath();
  F2CCells cov = robot.computeAreaTravelled(path);
  EXPECT_NEAR(cov.getArea(), 11, 1e-3);

  path_simp.appendClothoid(2, -0.1);
  path_simp.appendClothoid(2, 0.1);
  path_simp.appendStraightLine(10);
  path = path_simp.toPath();
  EXPECT_TRUE(IsPathCorrect(path));
  cov = robot.computeAreaTravelled(path);
  EXPECT_NEAR(cov.getArea(), 25, 1e-1);
  ///*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cov);
  f2c::Visualizer::plot(path);
  f2c::Visualizer::show();
  ///
}

TEST(fields2cover_types_robot, getMinSignedDistToSegment) {
  F2CRobot robot = getAgbotWithImpl();
  F2CPathStateSimp state;
  F2CPathSimp path;
  state.type = f2c::types::SectionSimp::STRAIGHT;
  state.len = 10;

  path.states = {state};
  double o1 = robot.getMinSignedDistToSegment(path, F2CPoint(0,0),F2CPoint(1,0));
  EXPECT_EQ(o1, -robot.getImplWidth()/2.0);

  state.type = f2c::types::SectionSimp::CLOTHOID;
  state.len = 5;
  state.sigma = 0.1;
  path.states = {state};
  double o2 = robot.getMinSignedDistToSegment(path, F2CPoint(0,0),F2CPoint(1,0));
  EXPECT_NEAR(o2, -4.2, 1e-2);

  ///*
  f2c::Visualizer::figure();
  path.states = {state};
  f2c::Visualizer::plotFilled(
      robot.computeAreaTravelled(path.toPath()), {1, 0., 0.}, {0.8, 0., 0.});
  f2c::Visualizer::plot(path);
  f2c::Visualizer::plot(F2CLineString({F2CPoint(0, o2), F2CPoint(5, o2)}), {0.5,0.9,0.5});
  f2c::Visualizer::axis_equal();
  f2c::Visualizer::show();
  ///
}



TEST(fields2cover_types_robot, getPointOnRobot) {
  F2CRobot robot;
  EXPECT_EQ(robot.getPointOnRobot(F2CPoint(10,20), M_PI/4, F2CPoint(-1, 1)),
      F2CPoint(10-sqrt(2),20));
}

*/







