//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <cmath>
#include <numeric>
#include <fstream>
#include "fields2cover.h"
#include "../test_helpers/path_planning_checker.hpp"
#include "../test_helpers/robot_data.hpp"


namespace {

F2CRobot connectionTestRobot(double min_turning_radius = 0.4, double cov_width = 2.5) {
  F2CRobot robot(2.1, cov_width);
  robot.setMinTurningRadius(min_turning_radius);
  robot.setMaxDiffCurv(2.0);
  return robot;
}

// Two opposing swaths with a gap between them, for connections to span.
void makeSwaths(double x_second, F2CSwaths& first, F2CSwaths& second) {
  first.emplace_back(F2CSwath(F2CLineString({F2CPoint(0, 0), F2CPoint(0, 20)})));
  second.emplace_back(
      F2CSwath(F2CLineString({F2CPoint(x_second, 20), F2CPoint(x_second, 0)})));
}

}  // namespace

TEST(fields2cover_pp_pp, planPathForConnection) {
  F2CRobot robot = getSimpleRobot();
  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurves dubins;
  F2CMultiPoint mp;
  auto path1 = path_planner.planPathForConnection(robot,
      F2CPoint(0, 0), M_PI * 0.5, mp, F2CPoint(2, 0), M_PI * 1.5,  dubins);
  EXPECT_NEAR(path1.length(), M_PI, 1e-2);

  mp.addPoint(30, 40);

  auto path2 = path_planner.planPathForConnection(robot,
      F2CPoint(0, 0), M_PI * 0.5, mp, F2CPoint(60, 0), M_PI * 1.5,  dubins);
  EXPECT_NEAR(path2.length(), 100, 10.0);

  mp.addPoint(33, 40);
  auto path3 = path_planner.planPathForConnection(robot,
      F2CPoint(30, 38), M_PI * 0.5, mp, F2CPoint(33, 37), M_PI * 1.5,  dubins);
  EXPECT_NEAR(path3.length(), 2 + 3 + 3, 1);

  EXPECT_TRUE(isPathCorrect(path1));
  EXPECT_TRUE(isPathCorrect(path2));
  EXPECT_TRUE(isPathCorrect(path3));
}

TEST(fields2cover_pp_pp, connectionCornersAreSmoothed) {
  // Corners of a detouring connection must go through the turn planner: a raw
  // boundary vertex is a heading step no turning radius can hold.
  F2CRobot robot = connectionTestRobot();
  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurvesCC dubins_cc;

  F2CSwaths first, second;
  makeSwaths(30.0, first, second);

  // Connection detours over the top with two right-angle corners.
  F2CRoute route;
  route.addConnectedSwaths(F2CMultiPoint(), first);
  route.addConnectedSwaths(
      F2CMultiPoint({F2CPoint(0, 20), F2CPoint(0, 40), F2CPoint(30, 40), F2CPoint(30, 20)}),
      second);

  auto path = path_planner.planPath(robot, route, dubins_cc);
  ASSERT_GT(path.size(), 2u);

  EXPECT_LT(maxHeadingStep(path), 1.0);  // a square corner would leave a ~1.57 rad step

  // Smoothing must round the corners, not skip the detour: a path that cut
  // straight across would satisfy the heading bound above on its own.
  EXPECT_LT(distanceToPath(path, F2CPoint(15, 40)), 0.5);
  EXPECT_LT(distanceToPath(path, F2CPoint(0, 30)), 0.5);
  EXPECT_LT(distanceToPath(path, F2CPoint(30, 30)), 0.5);
  // No connection turn may sweep past a u-turn: more than that is a loop,
  // which is smooth and barely longer, so nothing else here would catch it.
  EXPECT_LT(maxTurnSweep(path), M_PI + 0.2);
}

TEST(fields2cover_pp_pp, connectionJogIsSmoothed) {
  // A jog with two corners a couple of metres apart: too tight to take one at
  // a time, so it has to be rounded as a single S rather than driven square.
  F2CRobot robot = connectionTestRobot();
  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurvesCC dubins_cc;

  F2CSwaths first, second;
  makeSwaths(30.0, first, second);

  F2CRoute route;
  route.addConnectedSwaths(F2CMultiPoint(), first);
  route.addConnectedSwaths(
      F2CMultiPoint({
          F2CPoint(0, 20), F2CPoint(0, 40), F2CPoint(14, 40), F2CPoint(16, 42),
          F2CPoint(30, 42), F2CPoint(30, 20)}),
      second);

  auto path = path_planner.planPath(robot, route, dubins_cc);
  ASSERT_GT(path.size(), 2u);

  EXPECT_LT(maxHeadingStep(path), 1.0);
  EXPECT_LT(distanceToPath(path, F2CPoint(25, 42)), 0.5);
  // No connection turn may sweep past a u-turn: more than that is a loop,
  // which is smooth and barely longer, so nothing else here would catch it.
  EXPECT_LT(maxTurnSweep(path), M_PI + 0.2);
}

TEST(fields2cover_pp_pp, neighbourUturnIsPlannedAsATurn) {
  // Neighbouring swaths doubling back on each other. The track wanders out
  // past the straight-hop tolerance, but this is still a u-turn, not a corner
  // to round: driving its two vertices square would put a pair of right
  // angles where the headland maneuver belongs.
  F2CRobot robot = connectionTestRobot();
  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurvesCC dubins_cc;

  F2CSwaths first, second;
  first.emplace_back(F2CSwath(F2CLineString({F2CPoint(0, 0), F2CPoint(0, 20)})));
  second.emplace_back(F2CSwath(F2CLineString({F2CPoint(2.5, 20), F2CPoint(2.5, 0)})));

  F2CRoute route;
  route.addConnectedSwaths(F2CMultiPoint(), first);
  route.addConnectedSwaths(
      F2CMultiPoint(
          {F2CPoint(0, 20), F2CPoint(0, 21.5), F2CPoint(2.5, 21.5), F2CPoint(2.5, 20)}),
      second);

  auto path = path_planner.planPath(robot, route, dubins_cc);
  ASSERT_GT(path.size(), 2u);

  // Squaring off the two vertices would leave ~1.57 rad steps at each.
  EXPECT_LT(maxHeadingStep(path), 1.0);
  // No connection turn may sweep past a u-turn: more than that is a loop,
  // which is smooth and barely longer, so nothing else here would catch it.
  EXPECT_LT(maxTurnSweep(path), M_PI + 0.2);
}

TEST(fields2cover_pp_pp, curvedConnectionIsNotFlattened) {
  // Every point of this arc sits millimetres from the chord of its immediate
  // neighbours, so simplifying by local collinearity would drop them one by
  // one and collapse a 5m bulge onto its chord. The kept track must stay on
  // the arc.
  F2CRobot robot = connectionTestRobot();
  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurvesCC dubins_cc;

  F2CSwaths first, second;
  makeSwaths(40.0, first, second);

  F2CMultiPoint arc;
  for (int k = 0; k <= 40; ++k) {
    const double x = static_cast<double>(k);
    arc.addPoint(F2CPoint(x, 20.0 + 5.0 * std::sin(M_PI * x / 40.0)));
  }

  F2CRoute route;
  route.addConnectedSwaths(F2CMultiPoint(), first);
  route.addConnectedSwaths(arc, second);

  auto path = path_planner.planPath(robot, route, dubins_cc);
  ASSERT_GT(path.size(), 2u);

  // A junction corner left unrounded shows up here as a ~70 degree step.
  EXPECT_LT(maxHeadingStep(path), 1.0);

  EXPECT_LT(distanceToPath(path, F2CPoint(20, 25)), 0.5);   // apex
  EXPECT_LT(distanceToPath(path, F2CPoint(10, 23.54)), 0.5);
  EXPECT_LT(distanceToPath(path, F2CPoint(30, 23.54)), 0.5);
  // No connection turn may sweep past a u-turn: more than that is a loop,
  // which is smooth and barely longer, so nothing else here would catch it.
  EXPECT_LT(maxTurnSweep(path), M_PI + 0.2);
}

TEST(fields2cover_pp_pp, unroundableCornerKeepsTheTrack) {
  // With a turning circle far wider than the corridor, no maneuver fits
  // through the corner. The corner then stays sharp on purpose: cutting it
  // would put the vehicle off the planned track, which is worse than a step
  // the controller has to absorb.
  F2CRobot robot = connectionTestRobot(8.0, 2.5);
  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurvesCC dubins_cc;

  F2CSwaths first, second;
  makeSwaths(30.0, first, second);

  F2CRoute route;
  route.addConnectedSwaths(F2CMultiPoint(), first);
  route.addConnectedSwaths(
      F2CMultiPoint({F2CPoint(0, 20), F2CPoint(0, 40), F2CPoint(30, 40), F2CPoint(30, 20)}),
      second);

  auto path = path_planner.planPath(robot, route, dubins_cc);
  ASSERT_GT(path.size(), 2u);

  // Corners themselves, not just their neighbourhood: nothing may be cut here.
  EXPECT_LT(distanceToPath(path, F2CPoint(0, 40)), 0.1);
  EXPECT_LT(distanceToPath(path, F2CPoint(30, 40)), 0.1);
}

TEST(fields2cover_pp_pp, swathHeadingRoundsTheJunction) {
  // The junction with a swath is a corner: the connection need not leave
  // along the heading the swath holds. Without that heading it is not seen
  // as one.
  F2CRobot robot = connectionTestRobot();
  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurvesCC dubins_cc;

  // Swath runs east and ends where the connection sets off north.
  F2CSwaths first, second;
  first.emplace_back(F2CSwath(F2CLineString({F2CPoint(-20, 20), F2CPoint(0, 20)})));
  second.emplace_back(F2CSwath(F2CLineString({F2CPoint(30, 20), F2CPoint(30, 0)})));

  F2CRoute route;
  route.addConnectedSwaths(F2CMultiPoint(), first);
  route.addConnectedSwaths(
      F2CMultiPoint({F2CPoint(0, 20), F2CPoint(0, 40), F2CPoint(30, 40), F2CPoint(30, 20)}),
      second);

  auto path = path_planner.planPath(robot, route, dubins_cc);
  ASSERT_GT(path.size(), 2u);

  EXPECT_LT(maxHeadingStep(path), 1.0);  // driven straight off, a ~1.57 rad step

  // Starts on the swath's own end pose, not up the leg at the next vertex.
  auto turns = turnStarts(path);
  ASSERT_FALSE(turns.empty());
  EXPECT_NEAR(turns.front().getX(), 0.0, 1e-3);
  EXPECT_NEAR(turns.front().getY(), 20.0, 1e-3);
  // No connection turn may sweep past a u-turn: more than that is a loop,
  // which is smooth and barely longer, so nothing else here would catch it.
  EXPECT_LT(maxTurnSweep(path), M_PI + 0.2);
}

TEST(fields2cover_pp_pp, openEndedConnectionFollowsTheTrack) {
  // Route-end connections have a swath on one side only. The open end holds
  // no heading, so it is no corner, and nothing may be read off it.
  F2CRobot robot = connectionTestRobot();
  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurvesCC dubins_cc;

  F2CSwaths swaths;
  swaths.emplace_back(F2CSwath(F2CLineString({F2CPoint(0, 0), F2CPoint(0, 20)})));

  // A lead-in from off the field, and a lead-out back off it.
  F2CRoute route;
  route.addConnectedSwaths(
      F2CMultiPoint({F2CPoint(-20, -20), F2CPoint(0, -20), F2CPoint(0, 0)}), swaths);
  route.addConnection(
      F2CMultiPoint({F2CPoint(0, 20), F2CPoint(0, 40), F2CPoint(-20, 40)}));

  auto path = path_planner.planPath(robot, route, dubins_cc);
  ASSERT_GT(path.size(), 2u);

  EXPECT_LT(maxHeadingStep(path), 1.0);

  // One corner each; the open ends are driven through.
  EXPECT_EQ(turnStarts(path).size(), 2u);

  EXPECT_LT(distanceToPath(path, F2CPoint(-10, -20)), 0.5);
  EXPECT_LT(distanceToPath(path, F2CPoint(0, 30)), 0.5);
  // No connection turn may sweep past a u-turn: more than that is a loop,
  // which is smooth and barely longer, so nothing else here would catch it.
  EXPECT_LT(maxTurnSweep(path), M_PI + 0.2);
}

TEST(fields2cover_pp_pp, shallowCornerKeepsAnApproach) {
  // A shallow corner's fillet tangent collapses with it. The floor under
  // that is what leaves the planner a maneuver rather than two poses 6cm
  // apart.
  F2CRobot robot = connectionTestRobot();
  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurvesCC dubins_cc;

  F2CSwaths swaths;
  swaths.emplace_back(F2CSwath(F2CLineString({F2CPoint(0, 0), F2CPoint(0, 20)})));

  // Lead-out bending 15deg at (0, 50), well over the ~3deg driven straight.
  const double bend = 15.0 * M_PI / 180.0;
  const F2CPoint corner(0, 50);
  const F2CPoint tip(30.0 * std::sin(bend), 50.0 + 30.0 * std::cos(bend));

  F2CRoute route;
  route.addConnectedSwaths(F2CMultiPoint(), swaths);
  route.addConnection(F2CMultiPoint({F2CPoint(0, 20), corner, tip}));

  auto path = path_planner.planPath(robot, route, dubins_cc);
  ASSERT_GT(path.size(), 2u);

  // Rounded rather than left sharp, and the only corner on the route.
  auto turns = turnStarts(path);
  ASSERT_EQ(turns.size(), 1u);

  // Two-sided: 6cm of tangent here, nothing to turn in, so it must start
  // further back -- but not so far back that it leaves the leg early and
  // cuts ground the swath either side of it is covering. A shallow corner
  // is driven at a wider radius than a sharp one, and sizing the approach
  // from the sharp-corner radius is what pushes the start out.
  EXPECT_GT(turns.front().distance(corner), 0.15);
  EXPECT_LT(turns.front().distance(corner), 2.0);

  EXPECT_LT(distanceToPath(path, F2CPoint(0, 35)), 0.5);  // leg in followed
  // The leg out is one state with no point at its end, so check its heading.
  EXPECT_NEAR(path[path.size() - 1].angle, M_PI / 2.0 - bend, 1e-3);
  // No connection turn may sweep past a u-turn: more than that is a loop,
  // which is smooth and barely longer, so nothing else here would catch it.
  EXPECT_LT(maxTurnSweep(path), M_PI + 0.2);
}

TEST(fields2cover_pp_pp, tightCornersAreFoldedIntoOneTurn) {
  // Corners too close to round one at a time: rounding the first leaves the
  // second no approach, so the pair has to be taken as one S.
  //
  // The radius sets how close is too close. At the default 0.4m the jog
  // would be small enough to pass as a straight hop and never reach the
  // rounding.
  F2CRobot robot = connectionTestRobot(2.0, 2.5);
  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurvesCC dubins_cc;

  F2CSwaths first, second;
  first.emplace_back(F2CSwath(F2CLineString({F2CPoint(0, 0), F2CPoint(0, 20)})));
  second.emplace_back(F2CSwath(F2CLineString({F2CPoint(3, 63), F2CPoint(3, 83)})));

  // Corners at (0,40) and (3,43): 4.24m apart, inside the 4.8m a turn needs.
  F2CRoute route;
  route.addConnectedSwaths(F2CMultiPoint(), first);
  route.addConnectedSwaths(
      F2CMultiPoint({F2CPoint(0, 20), F2CPoint(0, 40), F2CPoint(3, 43), F2CPoint(3, 63)}),
      second);

  auto path = path_planner.planPath(robot, route, dubins_cc);
  ASSERT_GT(path.size(), 2u);

  // Taken one at a time they would each round here, and leave two.
  EXPECT_EQ(turnStarts(path).size(), 1u);

  // Their deflections cancel, so nothing bounds the fold by the corner it
  // cuts. It still has to make the jog: this is the midpoint of the leg
  // between them.
  EXPECT_LT(distanceToPath(path, F2CPoint(1.5, 41.5)), 0.5);

  EXPECT_LT(maxHeadingStep(path), 0.2);  // square corners leave ~0.79 rad
  // No connection turn may sweep past a u-turn: more than that is a loop,
  // which is smooth and barely longer, so nothing else here would catch it.
  EXPECT_LT(maxTurnSweep(path), M_PI + 0.2);
}

