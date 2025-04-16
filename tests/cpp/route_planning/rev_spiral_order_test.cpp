#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/route_planning/rev_spiral_order.h"
#include "../test_helpers/route_order_checker.hpp"

  /*
TEST(fields2cover_route_rev_spiral, genSortedSwaths_even) {
  const int n = 11;
  const int size = 6;
  F2CSwaths swaths = genSwathsTest(n);
  f2c::rp::RevSpiralOrder swath_sorter(size);
  swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {5, 3, 1, 2, 4, 6, 10, 8, 7, 9}));
}

TEST(fields2cover_route_rev_spiral, genSortedSwaths_odd) {
  const int n = 11;
  const int size = 5;
  F2CSwaths swaths = genSwathsTest(n);
  f2c::rp::RevSpiralOrder swath_sorter(size);
  swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {4, 2, 1, 3, 5, 9, 7, 6, 8, 10}));
}

TEST(fields2cover_route_rev_spiral, genSortedSwaths_even_size) {
  const int n = 10;
  const int size = 4;
  F2CSwaths swaths = genSwathsTest(n);
  f2c::rp::RevSpiralOrder swath_sorter;
  swath_sorter.setSpiralSize(size);
  swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {3, 1, 2, 4, 3, 7, 5, 6, 8}));
}

TEST(fields2cover_route_rev_spiral, genSortedSwaths_odd_size) {
  const int n = 10;
  const int size = 5;
  F2CSwaths swaths = genSwathsTest(n);
  f2c::rp::RevSpiralOrder swath_sorter;
  swath_sorter.setSpiralSize(size);

  swaths = swath_sorter.genSortedSwaths(swaths);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {4, 2, 1, 3, 5, 3, 1, 2, 4}));
}

*/
