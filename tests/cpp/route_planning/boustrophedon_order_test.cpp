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

TEST(fields2cover_route_boustrophedon, genSortedSwaths) {
  const int n = 5;
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(i, 0), F2CPoint(i, 1)}), i, i);
  }

  auto rng = std::default_random_engine {};
  std::shuffle(swaths.begin(), swaths.end(), rng);

  f2c::rp::BoustrophedonOrder swath_sorter;
  f2c::obj::DirectDistPathObj objective;

  swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_EQ(swaths.size(), n - 1);
  EXPECT_EQ(swaths[0].startPoint().getY(), 0);
  for (int i = 1; i < n; ++i) {
    EXPECT_EQ(swaths[i - 1].getWidth(), i);
  }
  EXPECT_EQ(objective.computeCost(swaths), 2*(n-1)-1);
  swaths = swath_sorter.genSortedSwaths(swaths, 1);
  EXPECT_EQ(swaths[0].startPoint().getY(), 0);
  EXPECT_EQ(swaths[0].getWidth(), n-1);
  EXPECT_EQ(swaths[1].getWidth(), n-2);
  EXPECT_EQ(swaths[2].getWidth(), n-3);
  EXPECT_EQ(objective.computeCost(swaths), 2*(n-1)-1);
  swaths = swath_sorter.genSortedSwaths(swaths, 2);
  EXPECT_EQ(swaths[0].startPoint().getY(), 1);
  EXPECT_EQ(swaths[0].getWidth(), 1);
  EXPECT_EQ(swaths[1].getWidth(), 2);
  EXPECT_EQ(swaths[2].getWidth(), 3);
  EXPECT_EQ(objective.computeCost(swaths), 2*(n-1)-1);
  swaths = swath_sorter.genSortedSwaths(swaths, 3);
  EXPECT_EQ(swaths[0].startPoint().getY(), 1);
  EXPECT_EQ(swaths[0].getWidth(), n-1);
  EXPECT_EQ(swaths[1].getWidth(), n-2);
  EXPECT_EQ(swaths[2].getWidth(), n-3);
  EXPECT_EQ(objective.computeCost(swaths), 2*(n-1)-1);
}

TEST(fields2cover_route_boustrophedon, genSortedSwaths_empty_swaths) {
  F2CSwaths swaths;
  f2c::rp::BoustrophedonOrder swath_sorter;
  auto new_swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_EQ(swaths.size(), 0);
  EXPECT_EQ(new_swaths.size(), 0);
}
