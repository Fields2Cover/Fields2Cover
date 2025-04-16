//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/objectives/rp_obj/direct_dist_path_obj.h"
#include "fields2cover/route_planning/snake_order.h"
#include "../test_helpers/route_order_checker.hpp"

TEST(fields2cover_route_snake, genSortedSwaths_even) {
  const int n = 11;
  F2CSwaths swaths = genSwathsTest(n);

  f2c::rp::SnakeOrder swath_sorter;
  swaths = swath_sorter.genSortedSwaths(swaths);

  EXPECT_TRUE(isRouteOrderCorrect(swaths, {1, 3, 5, 7, 9, 10, 8, 6, 4, 2}));

  f2c::obj::DirectDistPathObj objective;
  EXPECT_EQ(objective.computeCost(swaths), 3*((n-1)-1));
}


TEST(fields2cover_route_snake, genSortedSwaths_odd) {
  const int n = 10;
  F2CSwaths swaths = genSwathsTest(n);

  f2c::rp::SnakeOrder swath_sorter;
  swaths = swath_sorter.genSortedSwaths(swaths);

  EXPECT_TRUE(isRouteOrderCorrect(swaths, {1, 3, 5, 7, 9, 8, 6, 4, 2}));

  f2c::obj::DirectDistPathObj objective;
  EXPECT_EQ(objective.computeCost(swaths), 3*((n-1)-1));
}

