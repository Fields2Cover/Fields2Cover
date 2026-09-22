//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/route_planning/boustrophedon_order.h"
#include "fields2cover/route_planning/snake_order.h"

namespace {

// Travel space: one open rectangle. Swaths are vertical lines inside it, so
// every swath end can reach the border and the route can connect them.
F2CCells genField() {
  return F2CCells(F2CCell(F2CLinearRing({
      F2CPoint(0, 0), F2CPoint(100, 0), F2CPoint(100, 50),
      F2CPoint(0, 50), F2CPoint(0, 0)})));
}

F2CSwaths genColumn(const std::vector<double>& xs, int first_id) {
  F2CSwaths swaths;
  int id = first_id;
  for (double x : xs) {
    swaths.emplace_back(
        F2CLineString({F2CPoint(x, 5), F2CPoint(x, 45)}), 4.0, id++);
  }
  return swaths;
}

// Swaths of the route, flattened in the order the route drives them.
F2CSwaths routeSwaths(const F2CRoute& route) {
  F2CSwaths out;
  for (size_t i = 0; i < route.sizeVectorSwaths(); ++i) {
    for (auto&& s : route.getSwaths(i)) {
      out.emplace_back(s);
    }
  }
  return out;
}

// Index of the first swath the route drives whose x lies in [lo, hi].
size_t firstIndexInBand(const F2CSwaths& swaths, double lo, double hi) {
  for (size_t i = 0; i < swaths.size(); ++i) {
    const double x = swaths.at(i).startPoint().getX();
    if (x >= lo && x <= hi) {
      return i;
    }
  }
  return swaths.size();
}

}  // namespace

// The cells are handed over in an order that makes the field be crossed twice:
// left, right, middle. A route that picks its own order goes left, middle,
// right instead.
TEST(fields2cover_route_multicell, visitsCellsInAReasonableOrder) {
  F2CCells cells = genField();
  F2CSwathsByCells sbc;
  sbc.emplace_back(genColumn({10, 15}, 0));    // left
  sbc.emplace_back(genColumn({90, 95}, 10));   // right
  sbc.emplace_back(genColumn({50, 55}, 20));   // middle

  f2c::rp::BoustrophedonOrder order;
  F2CSwaths driven = routeSwaths(order.genRoute(cells, sbc));
  ASSERT_EQ(driven.size(), sbc.sizeTotal());

  const size_t i_middle = firstIndexInBand(driven, 45, 60);
  const size_t i_right = firstIndexInBand(driven, 85, 100);
  ASSERT_LT(i_middle, driven.size());
  ASSERT_LT(i_right, driven.size());
  EXPECT_LT(i_middle, i_right);
}

// Every cell is driven as one block: the route never leaves a cell and comes
// back to it later.
TEST(fields2cover_route_multicell, drivesEachCellInOneBlock) {
  F2CCells cells = genField();
  F2CSwathsByCells sbc;
  sbc.emplace_back(genColumn({10, 15}, 0));
  sbc.emplace_back(genColumn({90, 95}, 10));
  sbc.emplace_back(genColumn({50, 55}, 20));

  f2c::rp::BoustrophedonOrder order;
  F2CSwaths driven = routeSwaths(order.genRoute(cells, sbc));

  auto band = [](double x) {
    if (x < 45.0) { return 0; }
    return x < 85.0 ? 1 : 2;
  };
  std::vector<bool> left_behind(3, false);
  int current = -1;
  for (size_t i = 0; i < driven.size(); ++i) {
    const int b = band(driven.at(i).startPoint().getX());
    if (b == current) { continue; }
    if (current >= 0) { left_behind[current] = true; }
    EXPECT_FALSE(left_behind[b]) << "cell " << b << " is entered twice";
    current = b;
  }
}

// The second cell is entered from whichever of its two ends is closer to where
// the first cell was left, which means its swaths may be driven in reverse.
TEST(fields2cover_route_multicell, entersACellFromTheNearerEnd) {
  // The far cell comes first, so the near cell is reached from its far side:
  // driving its swaths in sorted order would mean crossing it twice.
  F2CCells cells = genField();
  F2CSwathsByCells sbc;
  sbc.emplace_back(genColumn({90, 95}, 0));
  sbc.emplace_back(genColumn({10, 15}, 10));

  f2c::rp::BoustrophedonOrder order;
  F2CSwathsByCells sorted = order.genSortedSwaths(sbc);
  const F2CPoint entry_a = sorted.at(1).at(0).startPoint();
  const F2CPoint entry_b = sorted.at(1).back().endPoint();

  F2CSwaths driven = routeSwaths(order.genRoute(cells, sbc));
  const size_t i_second = firstIndexInBand(driven, 0, 20);
  ASSERT_LT(i_second, driven.size());
  ASSERT_GT(i_second, 0u);

  const F2CPoint left_at = driven.at(i_second - 1).endPoint();
  const F2CPoint entered_at = driven.at(i_second).startPoint();
  const double d_a = left_at.distance(entry_a);
  const double d_b = left_at.distance(entry_b);
  EXPECT_NEAR(left_at.distance(entered_at), std::min(d_a, d_b), 1e-6);
}

// A single cell has no order to choose, so its route must not change.
TEST(fields2cover_route_multicell, singleCellIsUntouched) {
  F2CCells cells = genField();
  F2CSwathsByCells sbc;
  sbc.emplace_back(genColumn({10, 20, 30, 40}, 0));

  f2c::rp::BoustrophedonOrder order;
  F2CSwaths expected = order.genSortedSwaths(sbc).flatten();
  F2CSwaths driven = routeSwaths(order.genRoute(cells, sbc));

  ASSERT_EQ(driven.size(), expected.size());
  for (size_t i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(driven.at(i).startPoint(), expected.at(i).startPoint());
    EXPECT_EQ(driven.at(i).endPoint(), expected.at(i).endPoint());
  }
}

// The machine does not start at the field's first cell just because the
// decomposition listed it first: the route starts at the cell nearest to it.
TEST(fields2cover_route_multicell, startsAtTheCellNearestTheStartPoint) {
  F2CCells cells = genField();
  F2CSwathsByCells sbc;
  sbc.emplace_back(genColumn({10, 15}, 0));    // listed first
  sbc.emplace_back(genColumn({90, 95}, 10));   // but this one is next to us

  f2c::rp::BoustrophedonOrder order;
  order.setStartAndEndPoint(F2CPoint(98, 25));
  F2CSwaths driven = routeSwaths(order.genRoute(cells, sbc));

  ASSERT_EQ(driven.size(), sbc.sizeTotal());
  EXPECT_GT(driven.at(0).startPoint().getX(), 85.0);
}

// The route reaches the start point and comes back to it, so a caller can
// drive it from where the machine stands.
TEST(fields2cover_route_multicell, connectsBothEndsToTheStartPoint) {
  F2CCells cells = genField();
  F2CSwathsByCells sbc;
  sbc.emplace_back(genColumn({10, 15}, 0));
  sbc.emplace_back(genColumn({90, 95}, 10));

  const F2CPoint start(50, 2);
  f2c::rp::BoustrophedonOrder order;
  order.setStartAndEndPoint(start);
  F2CRoute route = order.genRoute(cells, sbc);

  ASSERT_FALSE(route.isEmpty());
  EXPECT_NEAR(route.startPoint().distance(start), 0.0, 1e-6);
  EXPECT_NEAR(route.endPoint().distance(start), 0.0, 1e-6);
}

// Without a start point nothing about the route changes.
TEST(fields2cover_route_multicell, withoutAStartPointTheRouteIsUnchanged) {
  F2CCells cells = genField();
  F2CSwathsByCells sbc;
  sbc.emplace_back(genColumn({10, 15}, 0));
  sbc.emplace_back(genColumn({90, 95}, 10));

  f2c::rp::BoustrophedonOrder plain;
  F2CSwaths a = routeSwaths(plain.genRoute(cells, sbc));

  f2c::rp::BoustrophedonOrder with_point;
  with_point.setStartAndEndPoint(F2CPoint(98, 25));
  F2CSwaths b = routeSwaths(with_point.genRoute(cells, sbc));

  ASSERT_EQ(a.size(), b.size());
  EXPECT_NE(a.at(0).startPoint(), b.at(0).startPoint());
}

// Two rooms joined only at the far end. The cell on the other side of the
// dividing wall is a stone's throw away in a straight line and a long drive in
// practice. Measuring through the headland graph is what tells them apart.
TEST(fields2cover_route_multicell, doesNotMistakeACellBehindAWallForANearOne) {
  F2CCells cells(F2CCell(F2CLinearRing({
      F2CPoint(0, 0), F2CPoint(50, 0), F2CPoint(50, 95), F2CPoint(60, 95),
      F2CPoint(60, 0), F2CPoint(110, 0), F2CPoint(110, 100), F2CPoint(0, 100),
      F2CPoint(0, 0)})));

  F2CSwathsByCells sbc;
  sbc.emplace_back(genColumn({40, 45}, 0));    // where the route starts
  sbc.emplace_back(genColumn({65, 70}, 10));   // just past the wall
  sbc.emplace_back(genColumn({5, 10}, 20));    // farther away, same room

  f2c::rp::BoustrophedonOrder order;
  F2CSwaths driven = routeSwaths(order.genRoute(cells, sbc));
  ASSERT_EQ(driven.size(), sbc.sizeTotal());

  // In a straight line the cell behind the wall is the nearer one.
  const F2CPoint leave(45, 45);
  ASSERT_LT(leave.distance(F2CPoint(65, 5)), leave.distance(F2CPoint(10, 5)));

  const size_t i_behind_wall = firstIndexInBand(driven, 60, 75);
  const size_t i_same_room = firstIndexInBand(driven, 0, 15);
  ASSERT_LT(i_behind_wall, driven.size());
  ASSERT_LT(i_same_room, driven.size());
  EXPECT_LT(i_same_room, i_behind_wall);
}

// Two separate fields have no path between them, so the graph cannot say how
// far apart their cells are. The order then falls back to the straight-line
// distance instead of leaving the cells where they came.
TEST(fields2cover_route_multicell, fallsBackToStraightLineWhereTheGraphCannotReach) {
  F2CCells cells;
  cells.addGeometry(F2CCell(F2CLinearRing({
      F2CPoint(0, 0), F2CPoint(40, 0), F2CPoint(40, 50),
      F2CPoint(0, 50), F2CPoint(0, 0)})));
  cells.addGeometry(F2CCell(F2CLinearRing({
      F2CPoint(200, 0), F2CPoint(240, 0), F2CPoint(240, 50),
      F2CPoint(200, 50), F2CPoint(200, 0)})));

  F2CSwaths near_cell;
  for (double x : {10.0, 20.0}) {
    near_cell.emplace_back(
        F2CLineString({F2CPoint(x, 5), F2CPoint(x, 45)}), 4.0, 0);
  }
  F2CSwaths far_cell;
  for (double x : {210.0, 220.0}) {
    far_cell.emplace_back(
        F2CLineString({F2CPoint(x, 5), F2CPoint(x, 45)}), 4.0, 10);
  }
  F2CSwaths middle_of_first;
  for (double x : {30.0, 35.0}) {
    middle_of_first.emplace_back(
        F2CLineString({F2CPoint(x, 5), F2CPoint(x, 45)}), 4.0, 20);
  }

  F2CSwathsByCells sbc;
  sbc.emplace_back(near_cell);
  sbc.emplace_back(far_cell);          // unreachable through the graph
  sbc.emplace_back(middle_of_first);

  f2c::rp::BoustrophedonOrder order;
  F2CSwaths driven = routeSwaths(order.genRoute(cells, sbc));

  ASSERT_EQ(driven.size(), sbc.sizeTotal());
  const size_t i_far = firstIndexInBand(driven, 200, 240);
  const size_t i_middle = firstIndexInBand(driven, 25, 40);
  ASSERT_LT(i_far, driven.size());
  ASSERT_LT(i_middle, driven.size());
  EXPECT_LT(i_middle, i_far);
}

// Cells the swath generator left empty are skipped, and a set that is empty
// altogether gives an empty route instead of tripping over itself.
TEST(fields2cover_route_multicell, emptyCellsAreSkipped) {
  F2CCells cells = genField();

  F2CSwathsByCells with_holes;
  with_holes.emplace_back(F2CSwaths());
  with_holes.emplace_back(genColumn({10, 15}, 0));
  with_holes.emplace_back(F2CSwaths());
  with_holes.emplace_back(genColumn({90, 95}, 10));

  f2c::rp::BoustrophedonOrder order;
  F2CSwaths driven = routeSwaths(order.genRoute(cells, with_holes));
  EXPECT_EQ(driven.size(), with_holes.sizeTotal());
  // The empty group in front is not what the route starts from.
  EXPECT_LT(driven.at(0).startPoint().getX(), 50.0);

  F2CSwathsByCells all_empty;
  all_empty.emplace_back(F2CSwaths());
  all_empty.emplace_back(F2CSwaths());
  EXPECT_TRUE(order.genRoute(cells, all_empty).isEmpty());

  order.setStartAndEndPoint(F2CPoint(50, 25));
  EXPECT_TRUE(order.genRoute(cells, all_empty).isEmpty());
}

// The order is a property of the base class, so every pattern gets it.
TEST(fields2cover_route_multicell, everyPatternOrdersItsCells) {
  F2CCells cells = genField();
  F2CSwathsByCells sbc;
  sbc.emplace_back(genColumn({10, 15}, 0));
  sbc.emplace_back(genColumn({90, 95}, 10));
  sbc.emplace_back(genColumn({50, 55}, 20));

  f2c::rp::SnakeOrder snake;
  F2CSwaths driven = routeSwaths(snake.genRoute(cells, sbc));
  ASSERT_EQ(driven.size(), sbc.sizeTotal());
  EXPECT_LT(firstIndexInBand(driven, 45, 60), firstIndexInBand(driven, 85, 100));
}
