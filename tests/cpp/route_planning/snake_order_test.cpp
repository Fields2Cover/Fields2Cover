//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/objectives/rp_obj/direct_dist_path_obj.h"
#include "fields2cover/route_planning/snake_order.h"

TEST(fields2cover_route_snake, genSortedSwaths_even) {
  const int n = 11;
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(0, i), F2CPoint(1, i)}), i, i);
  }

  f2c::rp::SnakeOrder swath_sorter;
  f2c::obj::DirectDistPathObj objective;

  swaths = swath_sorter.genSortedSwaths(swaths);

  EXPECT_EQ(swaths[0].getWidth(), 1);
  EXPECT_EQ(swaths[1].getWidth(), 3);
  EXPECT_EQ(swaths[2].getWidth(), 5);
  EXPECT_EQ(swaths.back().getWidth(), 2);

  EXPECT_EQ(objective.computeCost(swaths), 3*((n-1)-1));
}


TEST(fields2cover_route_snake, genSortedSwaths_odd) {
  const int n = 100;
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(0, i), F2CPoint(1, i)}), i, i);
  }

  f2c::rp::SnakeOrder swath_sorter;
  f2c::obj::DirectDistPathObj objective;

  swaths = swath_sorter.genSortedSwaths(swaths);

  EXPECT_EQ(swaths[0].getWidth(), 1);
  EXPECT_EQ(swaths[1].getWidth(), 3);
  EXPECT_EQ(swaths[2].getWidth(), 5);
  EXPECT_EQ(swaths.back().getWidth(), 2);
  EXPECT_EQ(objective.computeCost(swaths), 3*((n-1)-1));
}


// L-shaped cell: a straight hop over the notch leaves the field.
static F2CCells genLCell() {
  return F2CCells(F2CCell(F2CLinearRing({
        F2CPoint(0, 0), F2CPoint(90, 0), F2CPoint(90, 20), F2CPoint(20, 20),
        F2CPoint(20, 90), F2CPoint(0, 90), F2CPoint(0, 0)})));
}

static F2CSwathsByCells genLSwaths(const F2CCells& cell) {
  F2CSwaths swaths;
  for (int x = 2; x < 90; x += 4) {
    swaths.append(F2CLineString({F2CPoint(x, 1), F2CPoint(x, 89)}), cell, 4);
  }
  F2CSwathsByCells sbc;
  sbc.emplace_back(swaths);
  return sbc;
}

TEST(fields2cover_route_snake, genRouteAddsConnections) {
  F2CCells cell = genLCell();
  F2CSwathsByCells sbc = genLSwaths(cell);
  f2c::rp::SnakeOrder swath_sorter;

  F2CRoute route = swath_sorter.genRoute(cell, sbc);
  EXPECT_FALSE(route.isEmpty());
  EXPECT_EQ(route.sizeVectorSwaths(), route.sizeConnections());

  size_t filled = 0;
  for (auto&& c : route.getConnections()) {
    if (c.size() >= 2) { ++filled; }
  }
  EXPECT_GT(filled, 0);
}

TEST(fields2cover_route_snake, genRouteKeepsConnectionsInsideTheCell) {
  F2CCells cell = genLCell();
  F2CSwathsByCells sbc = genLSwaths(cell);
  f2c::rp::SnakeOrder swath_sorter;

  F2CSwaths ordered = swath_sorter.genSortedSwaths(sbc).flatten();
  // The test only means something if a naked hop would have left the cell.
  size_t naked_outside = 0;
  for (size_t i = 1; i < ordered.size(); ++i) {
    F2CLineString hop {ordered.at(i - 1).endPoint(), ordered.at(i).startPoint()};
    if (hop.length() - cell.getLinesInside(hop).length() > 1e-6) {
      ++naked_outside;
    }
  }
  ASSERT_GT(naked_outside, 0);

  F2CRoute route = swath_sorter.genRoute(cell, sbc);
  size_t checked = 0;
  for (auto&& c : route.getConnections()) {
    if (c.size() < 2) { continue; }
    F2CLineString line;
    for (auto&& p : c) { line.addPoint(p); }
    EXPECT_NEAR(cell.getLinesInside(line).length(), line.length(), 1e-6);
    ++checked;
  }
  // Without connections there would be nothing to check and this would pass
  // no matter what genRoute did.
  EXPECT_GE(checked, naked_outside);
}

TEST(fields2cover_route_snake, genRouteCoversTheSameSwathsInTheSameOrder) {
  F2CCells cell = genLCell();
  F2CSwathsByCells sbc = genLSwaths(cell);
  f2c::rp::SnakeOrder swath_sorter;

  F2CSwaths ordered = swath_sorter.genSortedSwaths(sbc).flatten();
  F2CRoute route = swath_sorter.genRoute(cell, sbc);

  F2CSwaths from_route;
  for (size_t i = 0; i < route.sizeVectorSwaths(); ++i) {
    from_route.append(route.getSwaths(i));
  }
  ASSERT_EQ(from_route.size(), ordered.size());
  for (size_t i = 0; i < ordered.size(); ++i) {
    EXPECT_EQ(from_route.at(i).startPoint(), ordered.at(i).startPoint());
    EXPECT_EQ(from_route.at(i).endPoint(), ordered.at(i).endPoint());
  }
}
