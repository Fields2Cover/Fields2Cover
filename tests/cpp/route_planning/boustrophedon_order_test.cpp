//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <random>
#include "fields2cover/types.h"
#include "fields2cover/objectives/rp_obj/direct_dist_path_obj.h"
#include "fields2cover/route_planning/boustrophedon_order.h"
#include "../test_helpers/route_order_checker.hpp"

TEST(fields2cover_route_boustrophedon, genSortedSwaths) {
  const int n = 5;
  F2CSwaths swaths = genSwathsTest(n);

  auto rng = std::default_random_engine {};
  std::shuffle(swaths.begin(), swaths.end(), rng);

  f2c::rp::BoustrophedonOrder swath_sorter;
  f2c::obj::DirectDistPathObj objective;

  swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_EQ(swaths.size(), n - 1);
  EXPECT_EQ(swaths[0].startPoint().getX(), 0);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {1, 2, 3, 4}));

  EXPECT_EQ(objective.computeCost(swaths), 2*(n-1)-1);
  swaths = swath_sorter.genSortedSwaths(swaths, 1);
  EXPECT_EQ(swaths[0].startPoint().getX(), 0);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {4, 3, 2, 1}));

  EXPECT_EQ(objective.computeCost(swaths), 2*(n-1)-1);
  swaths = swath_sorter.genSortedSwaths(swaths, 2);
  EXPECT_EQ(swaths[0].startPoint().getX(), 1);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {1, 2, 3, 4}));

  EXPECT_EQ(objective.computeCost(swaths), 2*(n-1)-1);
  swaths = swath_sorter.genSortedSwaths(swaths, 3);
  EXPECT_EQ(swaths[0].startPoint().getX(), 1);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {4, 3, 2, 1}));
  EXPECT_EQ(objective.computeCost(swaths), 2*(n-1)-1);
}

TEST(fields2cover_route_boustrophedon, genSortedSwaths_empty_swaths) {
  F2CSwaths swaths;
  f2c::rp::BoustrophedonOrder swath_sorter;
  auto new_swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_EQ(swaths.size(), 0);
  EXPECT_EQ(new_swaths.size(), 0);
}
