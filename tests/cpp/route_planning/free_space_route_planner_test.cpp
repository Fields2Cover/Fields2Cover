//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/route_planning/free_space_route_planner.h"
#include "fields2cover.h"

namespace {

// A square field with a square of crop in the middle: the ground the robot may
// drive on is the ring between them, and the only way from one side to the
// other is around.
F2CCells ringGround(double outer, double inner) {
  F2CCell c(F2CLinearRing({
      F2CPoint(0, 0), F2CPoint(outer, 0), F2CPoint(outer, outer),
      F2CPoint(0, outer), F2CPoint(0, 0)}));
  const double lo = 0.5 * (outer - inner), hi = lo + inner;
  c.addRing(F2CLinearRing({
      F2CPoint(lo, lo), F2CPoint(lo, hi), F2CPoint(hi, hi),
      F2CPoint(hi, lo), F2CPoint(lo, lo)}));
  return F2CCells(c);
}

double pathLength(const std::vector<F2CPoint>& p) {
  double len = 0.0;
  for (size_t i = 0; i + 1 < p.size(); ++i) {
    len += p[i].distance(p[i + 1]);
  }
  return len;
}

F2CCells block(double x0, double y0, double x1, double y1) {
  return F2CCells(F2CCell(F2CLinearRing({
      F2CPoint(x0, y0), F2CPoint(x1, y0), F2CPoint(x1, y1),
      F2CPoint(x0, y1), F2CPoint(x0, y0)})));
}

}  // namespace

TEST(fields2cover_rp_free_space, every_edge_stays_on_the_ground) {
  const F2CCells ground = ringGround(100.0, 60.0);
  F2CSwathsByCells swaths;
  f2c::rp::FreeSpaceRoutePlanner planner;
  const F2CGraph2D g = planner.createShortestGraph(ground, swaths, 1e-4);

  EXPECT_GT(g.numNodes(), 0u);
  const auto edges = g.getEdges();
  size_t checked = 0;
  for (const auto& from : edges) {
    for (const auto& e : from.second) {
      const F2CPoint a = g.indexToNode(from.first);
      const F2CPoint b = g.indexToNode(e.first);
      // Sample the middle of the edge; the crop is a hole, so an edge that
      // crossed it would land inside.
      for (int k = 1; k < 8; ++k) {
        const double u = k / 8.0;
        const F2CPoint m(a.getX() + (b.getX() - a.getX()) * u,
                         a.getY() + (b.getY() - a.getY()) * u);
        EXPECT_TRUE(ground.isPointIn(m) || ground.isPointInBorder(m))
            << "edge leaves the drivable ground at " << m.getX()
            << ", " << m.getY();
      }
      ++checked;
    }
  }
  EXPECT_GT(checked, 0u);
}

TEST(fields2cover_rp_free_space, goes_around_the_crop_not_through_it) {
  const F2CCells ground = ringGround(100.0, 60.0);
  F2CSwathsByCells swaths;
  f2c::rp::FreeSpaceRoutePlanner planner;
  F2CGraph2D g = planner.createShortestGraph(ground, swaths, 1e-4);

  // Two corners of the field on opposite sides of the crop. The straight line
  // between them runs through the crop, so the path has to be longer than it.
  const F2CPoint a(0.0, 0.0), b(100.0, 100.0);
  ASSERT_TRUE(g.hasNode(a));
  ASSERT_TRUE(g.hasNode(b));
  const std::vector<F2CPoint> path = g.shortestPath(a, b);
  ASSERT_GE(path.size(), 2u);
  EXPECT_EQ(path.front(), a);
  EXPECT_EQ(path.back(), b);
  EXPECT_GT(pathLength(path), a.distance(b));
  EXPECT_EQ(planner.getComponentCount(), 1u);
}

TEST(fields2cover_rp_free_space, two_pieces_of_ground_are_not_joined_across_a_gap) {
  // Ground in two pieces with 2 cm between them. Nothing can be driven from
  // one to the other, but the segment joining them is only looked at every
  // sample step, so a gap narrower than that used to go unseen: the graph
  // reported one piece and offered six edges straight over the gap. The
  // crossing test does not catch it, because such an edge runs along the walls
  // instead of properly crossing them.
  const F2CCells ground =
      block(0.0, 0.0, 60.0, 10.0).difference(block(29.99, -1.0, 30.01, 11.0));
  ASSERT_EQ(ground.size(), 2u);

  F2CSwathsByCells swaths;
  f2c::rp::FreeSpaceRoutePlanner planner;
  F2CGraph2D g = planner.createShortestGraph(ground, swaths, 1e-4);

  EXPECT_EQ(planner.getComponentCount(), 2u)
      << "the two pieces were joined: a gap narrower than the sample step was "
      << "stepped over";
  const F2CPoint a(29.99, 0.0), b(30.01, 10.0);
  ASSERT_TRUE(g.hasNode(a));
  ASSERT_TRUE(g.hasNode(b));
  EXPECT_TRUE(g.shortestPath(a, b).empty())
      << "a route was found between two pieces of ground that do not touch";
}

TEST(fields2cover_rp_free_space, a_dip_in_a_wall_is_not_driven_over) {
  // The tripwire for the tolerance a sample on the border is given. That
  // tolerance is for exact-boundary arithmetic, and it must stay far below any
  // real geometry: the shallowest dip a chord along the wall can be allowed to
  // shave is exactly the tolerance, while the length it shaves is unbounded.
  // Tying it to the sample step -- a tempting tidy-up, since the walk beside it
  // steps by that -- would buy no edge at all and would licence shaving a
  // sample step of crop. This dip is 1 cm deep, so it fails the moment the
  // tolerance is raised anywhere near the step.
  F2CCells ground(F2CCell(F2CLinearRing({
      F2CPoint(0, 0), F2CPoint(60, 0), F2CPoint(60, 10),
      F2CPoint(35, 10), F2CPoint(35, 9.99), F2CPoint(25, 9.99),
      F2CPoint(25, 10), F2CPoint(0, 10), F2CPoint(0, 0)})));
  F2CSwathsByCells swaths;
  f2c::rp::FreeSpaceRoutePlanner planner;
  F2CGraph2D g = planner.createShortestGraph(ground, swaths, 1e-4);

  const F2CPoint a(25.0, 10.0), b(35.0, 10.0);
  ASSERT_TRUE(g.hasNode(a));
  ASSERT_TRUE(g.hasNode(b));
  const std::vector<F2CPoint> path = g.shortestPath(a, b);
  ASSERT_GE(path.size(), 2u);
  // Around the dip it is 10.02 m through its two corners; straight over it
  // would be the 10.00 m chord.
  EXPECT_GT(path.size(), 2u)
      << "the chord over the dip was taken as an edge: the border tolerance is "
      << "large enough to swallow 1 cm of ground";
  EXPECT_GT(pathLength(path), 10.01);
}

TEST(fields2cover_rp_free_space, a_run_along_a_wall_of_the_ground_is_drivable) {
  // A run along a side of the crop is ground the whole way: the wall is
  // ground. Whether the graph offered that run used to depend on which side of
  // the crop it was -- the ray cast that samples the segment answers "outside"
  // on a wall it leaves below or to its left, and "inside" on the other two --
  // so the same 60 m run was a straight line along the north and east walls
  // and a 110.75 m trip around the corner of the field along the south and
  // west ones.
  const F2CCells ground = ringGround(100.0, 60.0);   // crop from 20 to 80
  F2CSwathsByCells swaths;
  f2c::rp::FreeSpaceRoutePlanner planner;
  F2CGraph2D g = planner.createShortestGraph(ground, swaths, 1e-4);
  ASSERT_EQ(planner.getComponentCount(), 1u);

  struct Side { const char* name; F2CPoint a, b; };
  for (const Side& s : {
      Side{"south", F2CPoint(20, 20), F2CPoint(80, 20)},
      Side{"north", F2CPoint(20, 80), F2CPoint(80, 80)},
      Side{"west",  F2CPoint(20, 20), F2CPoint(20, 80)},
      Side{"east",  F2CPoint(80, 20), F2CPoint(80, 80)}}) {
    ASSERT_TRUE(g.hasNode(s.a));
    ASSERT_TRUE(g.hasNode(s.b));
    const std::vector<F2CPoint> path = g.shortestPath(s.a, s.b);
    ASSERT_GE(path.size(), 2u);
    EXPECT_NEAR(pathLength(path), 60.0, 1e-6)
        << "the run along the crop's " << s.name << " wall is 60 m of ground "
        << "from end to end, and the route takes " << path.size()
        << " points to get there: a segment lying on a wall was rejected, so "
        << "the graph went around the field instead";
  }
}

TEST(fields2cover_rp_free_space, counts_the_pieces_the_ground_falls_into) {
  // Two squares that do not touch: nothing can be routed between them, and the
  // planner says so instead of leaving the caller to find out from the route.
  F2CCells ground;
  ground.addGeometry(F2CCell(F2CLinearRing({
      F2CPoint(0, 0), F2CPoint(10, 0), F2CPoint(10, 10),
      F2CPoint(0, 10), F2CPoint(0, 0)})));
  ground.addGeometry(F2CCell(F2CLinearRing({
      F2CPoint(50, 0), F2CPoint(60, 0), F2CPoint(60, 10),
      F2CPoint(50, 10), F2CPoint(50, 0)})));

  F2CSwathsByCells swaths;
  f2c::rp::FreeSpaceRoutePlanner planner;
  const F2CGraph2D g = planner.createShortestGraph(ground, swaths, 1e-4);
  EXPECT_EQ(planner.getComponentCount(), 2u);
  EXPECT_GT(g.numNodes(), 0u);
}

TEST(fields2cover_rp_free_space, swath_ends_are_reachable_nodes) {
  const F2CCells ground = ringGround(100.0, 60.0);
  // A swath lying in the ring, its ends on the ground.
  F2CSwaths sw;
  sw.emplace_back(F2CLineString({F2CPoint(5, 20), F2CPoint(5, 80)}), 2.0);
  F2CSwathsByCells swaths;
  swaths.emplace_back(sw);

  f2c::rp::FreeSpaceRoutePlanner planner;
  const F2CGraph2D g = planner.createShortestGraph(ground, swaths, 1e-4);
  EXPECT_TRUE(g.hasNode(F2CPoint(5, 20)));
  EXPECT_TRUE(g.hasNode(F2CPoint(5, 80)));
}

TEST(fields2cover_rp_free_space, clearance_is_priced_not_forbidden) {
  const F2CCells ground = ringGround(100.0, 60.0);
  F2CSwathsByCells swaths;

  f2c::rp::FreeSpaceRoutePlanner plain;
  const F2CGraph2D g0 = plain.createShortestGraph(ground, swaths, 1e-4);

  f2c::rp::FreeSpaceRoutePlanner kept_clear;
  kept_clear.setClearance(3.0);
  kept_clear.setClearanceCost(10.0);
  const F2CGraph2D g1 = kept_clear.createShortestGraph(ground, swaths, 1e-4);

  // The band changes what an edge costs, never whether it exists: the ground
  // stays one piece and no node is cut off.
  EXPECT_EQ(g0.numNodes(), g1.numNodes());
  EXPECT_EQ(kept_clear.getComponentCount(), 1u);
  EXPECT_DOUBLE_EQ(kept_clear.getClearance(), 3.0);
  EXPECT_DOUBLE_EQ(kept_clear.getClearanceCost(), 10.0);
}

TEST(fields2cover_rp_free_space, empty_ground_is_not_a_crash) {
  F2CCells ground;
  F2CSwathsByCells swaths;
  f2c::rp::FreeSpaceRoutePlanner planner;
  const F2CGraph2D g = planner.createShortestGraph(ground, swaths, 1e-4);
  EXPECT_EQ(g.numNodes(), 0u);
  EXPECT_EQ(planner.getComponentCount(), 0u);
}
