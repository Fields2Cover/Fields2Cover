#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/route_planning/spiral_order.h"

TEST(fields2cover_route_spiral, genSortedSwaths_even) {
  const int n = 11;
  const int size = 6;
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(0, i), F2CPoint(1, i)}), i, i);
  }

  f2c::rp::SpiralOrder swath_sorter(swaths, size);

  swaths = swath_sorter.genSortedSwaths();

  EXPECT_EQ(swaths[0].getWidth(), 1);
  EXPECT_EQ(swaths[1].getWidth(), 6);
  EXPECT_EQ(swaths[2].getWidth(), 2);
  EXPECT_EQ(swaths[3].getWidth(), 5);
  EXPECT_EQ(swaths[4].getWidth(), 3);
  EXPECT_EQ(swaths[5].getWidth(), 4);
  EXPECT_EQ(swaths[6].getWidth(), 7);
  EXPECT_EQ(swaths[7].getWidth(), 10);
  EXPECT_EQ(swaths[8].getWidth(), 8);
  EXPECT_EQ(swaths[9].getWidth(), 9);
}

TEST(fields2cover_route_spiral, genSortedSwaths_odd) {
  const int n = 11;
  const int size = 5;
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(0, i), F2CPoint(1, i)}), i, i);
  }

  f2c::rp::SpiralOrder swath_sorter(swaths, size);

  swaths = swath_sorter.genSortedSwaths();

  EXPECT_EQ(swaths[0].getWidth(), 1);
  EXPECT_EQ(swaths[1].getWidth(), 5);
  EXPECT_EQ(swaths[2].getWidth(), 2);
  EXPECT_EQ(swaths[3].getWidth(), 4);
  EXPECT_EQ(swaths[4].getWidth(), 3);
  EXPECT_EQ(swaths[5].getWidth(), 10);
  EXPECT_EQ(swaths[6].getWidth(), 6);
  EXPECT_EQ(swaths[7].getWidth(), 9);
  EXPECT_EQ(swaths[8].getWidth(), 7);
  EXPECT_EQ(swaths[9].getWidth(), 8);
}

TEST(fields2cover_route_spiral, genSortedSwaths_even_size) {
  const int n = 11;
  const int size = 6;
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(0, i), F2CPoint(1, i)}), i, i);
  }

  f2c::rp::SpiralOrder swath_sorter(swaths);
  swath_sorter.set_spiral_size(size);

  swaths = swath_sorter.genSortedSwaths();

  EXPECT_EQ(swaths[0].getWidth(), 1);
  EXPECT_EQ(swaths[1].getWidth(), 6);
  EXPECT_EQ(swaths[2].getWidth(), 2);
  EXPECT_EQ(swaths[3].getWidth(), 5);
  EXPECT_EQ(swaths[4].getWidth(), 3);
  EXPECT_EQ(swaths[5].getWidth(), 4);
  EXPECT_EQ(swaths[6].getWidth(), 7);
  EXPECT_EQ(swaths[7].getWidth(), 10);
  EXPECT_EQ(swaths[8].getWidth(), 8);
  EXPECT_EQ(swaths[9].getWidth(), 9);
}

TEST(fields2cover_route_spiral, genSortedSwaths_odd_size) {
  const int n = 11;
  const int size = 5;
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(0, i), F2CPoint(1, i)}), i, i);
  }

  f2c::rp::SpiralOrder swath_sorter(swaths);
  swath_sorter.set_spiral_size(size);

  swaths = swath_sorter.genSortedSwaths();

  EXPECT_EQ(swaths[0].getWidth(), 1);
  EXPECT_EQ(swaths[1].getWidth(), 5);
  EXPECT_EQ(swaths[2].getWidth(), 2);
  EXPECT_EQ(swaths[3].getWidth(), 4);
  EXPECT_EQ(swaths[4].getWidth(), 3);
  EXPECT_EQ(swaths[5].getWidth(), 10);
  EXPECT_EQ(swaths[6].getWidth(), 6);
  EXPECT_EQ(swaths[7].getWidth(), 9);
  EXPECT_EQ(swaths[8].getWidth(), 7);
  EXPECT_EQ(swaths[9].getWidth(), 8);
}

TEST(fields2cover_route_spiral, genSortedSwaths_default_size) {
  const int n = 11;
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(0, i), F2CPoint(1, i)}), i, i);
  }

  f2c::rp::SpiralOrder swath_sorter(swaths);

  swaths = swath_sorter.genSortedSwaths();

  EXPECT_EQ(swaths[0].getWidth(), 2);
  EXPECT_EQ(swaths[1].getWidth(), 1);
  EXPECT_EQ(swaths[2].getWidth(), 4);
  EXPECT_EQ(swaths[3].getWidth(), 3);
  EXPECT_EQ(swaths[4].getWidth(), 6);
  EXPECT_EQ(swaths[5].getWidth(), 5);
  EXPECT_EQ(swaths[6].getWidth(), 8);
  EXPECT_EQ(swaths[7].getWidth(), 7);
  EXPECT_EQ(swaths[8].getWidth(), 10);
  EXPECT_EQ(swaths[9].getWidth(), 9);
}

TEST(fields2cover_route_spiral, genSortedSwaths_default_cstr) {
  const int n = 11;
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(0, i), F2CPoint(1, i)}), i, i);
  }

  f2c::rp::SpiralOrder swath_sorter;
  swath_sorter.setSwaths(swaths);
  swath_sorter.set_spiral_size(1);

  swaths = swath_sorter.genSortedSwaths();

  EXPECT_EQ(swaths[0].getWidth(), 2);
  EXPECT_EQ(swaths[1].getWidth(), 1);
  EXPECT_EQ(swaths[2].getWidth(), 4);
  EXPECT_EQ(swaths[3].getWidth(), 3);
  EXPECT_EQ(swaths[4].getWidth(), 6);
  EXPECT_EQ(swaths[5].getWidth(), 5);
  EXPECT_EQ(swaths[6].getWidth(), 8);
  EXPECT_EQ(swaths[7].getWidth(), 7);
  EXPECT_EQ(swaths[8].getWidth(), 10);
  EXPECT_EQ(swaths[9].getWidth(), 9);
}
