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

