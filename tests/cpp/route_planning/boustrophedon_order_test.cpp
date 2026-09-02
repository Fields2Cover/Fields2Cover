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
#include "fields2cover/route_planning/snake_order.h"

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

// Two cells side by side, three swaths each, shuffled within each cell.
static F2CSwathsByCells genTwoCellSwaths() {
  F2CSwathsByCells sbc;
  auto rng = std::default_random_engine {};
  for (int cell = 0; cell < 2; ++cell) {
    F2CSwaths swaths;
    for (int i = 0; i < 3; ++i) {
      swaths.append(F2CLineString(
            {F2CPoint(10 * cell + i, 0), F2CPoint(10 * cell + i, 5)}), 1);
    }
    std::shuffle(swaths.begin(), swaths.end(), rng);
    sbc.emplace_back(swaths);
  }
  return sbc;
}

TEST(fields2cover_route_boustrophedon, genSortedSwathsByCellsKeepsCellsApart) {
  F2CSwathsByCells sbc = genTwoCellSwaths();
  f2c::rp::BoustrophedonOrder swath_sorter;

  F2CSwathsByCells sorted = swath_sorter.genSortedSwaths(sbc);
  ASSERT_EQ(sorted.size(), sbc.size());
  for (size_t c = 0; c < sorted.size(); ++c) {
    ASSERT_EQ(sorted.at(c).size(), sbc.at(c).size());
    // Every swath stays in the cell it came from.
    for (auto&& s : sorted.at(c)) {
      EXPECT_NEAR(s.startPoint().getX(), 10.0 * c, 2.0);
    }
    // and is ordered inside it
    for (size_t i = 1; i < sorted.at(c).size(); ++i) {
      EXPECT_LT(sorted.at(c).at(i - 1).startPoint().getX(),
                sorted.at(c).at(i).startPoint().getX());
    }
  }
}

TEST(fields2cover_route_boustrophedon, genSortedSwathsByCellsSwitchesCellOnce) {
  F2CSwathsByCells sbc = genTwoCellSwaths();

  // Flattening first collides the per-cell ids, so the sort interleaves
  // the cells; ordering per cell crosses between them exactly once.
  auto countSwitches = [](const F2CSwaths& s) {
    size_t switches = 0;
    for (size_t i = 1; i < s.size(); ++i) {
      if ((s.at(i - 1).startPoint().getX() < 5.0) !=
          (s.at(i).startPoint().getX() < 5.0)) {
        ++switches;
      }
    }
    return switches;
  };

  // A snake over the whole set skips two at a time and crosses back and forth;
  // per cell it crosses between the cells exactly once.
  f2c::rp::SnakeOrder snake;
  EXPECT_GT(countSwitches(snake.genSortedSwaths(sbc.flatten())), sbc.size() - 1);

  F2CSwaths per_cell;
  for (auto&& cell_swaths : snake.genSortedSwaths(sbc)) {
    for (auto&& s : cell_swaths) { per_cell.emplace_back(s); }
  }
  EXPECT_EQ(countSwitches(per_cell), sbc.size() - 1);
}
