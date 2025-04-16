#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/route_planning/spiral_order.h"
#include "../test_helpers/route_order_checker.hpp"

TEST(fields2cover_route_spiral, genSortedSwaths_even) {
  const int n = 11;
  const int size = 6;
  F2CSwaths swaths = genSwathsTest(n);
  f2c::rp::SpiralOrder swath_sorter(size);
  swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {1, 6, 2, 5, 3, 4, 7, 10, 8, 9}));
}

TEST(fields2cover_route_spiral, genSortedSwaths_odd) {
  const int n = 11;
  const int size = 5;
  F2CSwaths swaths = genSwathsTest(n);
  f2c::rp::SpiralOrder swath_sorter(size);
  swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {1, 5, 2, 4, 3, 10, 6, 9, 7, 8}));
}

TEST(fields2cover_route_spiral, genSortedSwaths_even_size) {
  const int n = 11;
  const int size = 6;
  F2CSwaths swaths = genSwathsTest(n);
  f2c::rp::SpiralOrder swath_sorter;
  swath_sorter.setSpiralSize(size);
  swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {1, 6, 2, 5, 3, 4, 7, 10, 8, 9}));
}

TEST(fields2cover_route_spiral, genSortedSwaths_odd_size) {
  const int n = 11;
  const int size = 5;
  F2CSwaths swaths = genSwathsTest(n);
  f2c::rp::SpiralOrder swath_sorter;
  swath_sorter.setSpiralSize(size);

  swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {1, 5, 2, 4, 3, 10, 6, 9, 7, 8}));
}

TEST(fields2cover_route_spiral, genSortedSwaths_default_size) {
  const int n = 11;
  F2CSwaths swaths = genSwathsTest(n);
  f2c::rp::SpiralOrder swath_sorter;
  swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
}


TEST(fields2cover_route_spiral, genSortedSwaths_bigger_than_8_bulk) {
  const int n = 11;
  const int size = 10;
  F2CSwaths swaths = genSwathsTest(n);
  f2c::rp::SpiralOrder swath_sorter(size);
  swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {1, 10, 2, 9, 3, 8, 4, 7, 5, 6}));
}


