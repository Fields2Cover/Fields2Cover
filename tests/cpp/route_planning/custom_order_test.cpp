#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/objectives/rp_obj/direct_dist_path_obj.h"
#include "fields2cover/route_planning/custom_order.h"
#include "../test_helpers/route_order_checker.hpp"

TEST(fields2cover_route_custom_order, defined_order) {
  const int n = 11;
  F2CSwaths swaths = genSwathsTest(n);

  f2c::rp::CustomOrder swath_sorter;
  swath_sorter.setCustomOrder({1,0,3,2,5,4,7,6,8,9});

  swaths = swath_sorter.genSortedSwaths(swaths, 0);
  EXPECT_TRUE(isRouteOrderCorrect(swaths, {2, 1, 4, 3, 6, 5, 8, 7, 9, 10}));
}

TEST(fields2cover_route_custom_order, constructor_order) {
  const int n = 11;
  F2CSwaths swaths = genSwathsTest(n);

  f2c::rp::CustomOrder swath_sorter({9,0,8,1,7,2,6,3,5,4});
  f2c::rp::CustomOrder swath_sorter2;
  swath_sorter2.setCustomOrder({9,0,8,1,7,2,6,3,5,4});

  f2c::obj::DirectDistPathObj objective;

  swaths = swath_sorter.genSortedSwaths(swaths);
  auto swaths2 = swath_sorter2.genSortedSwaths(swaths);

  EXPECT_TRUE(isRouteOrderCorrect(swaths, {10,1,9,2,8,3,7,4,6,5}));
  EXPECT_TRUE(isRouteOrderCorrect(swaths2, {10,1,9,2,8,3,7,4,6,5}));
}

TEST(fields2cover_route_custom_order, bad_order) {
  const int n = 11;
  F2CSwaths swaths = genSwathsTest(n);
  EXPECT_ANY_THROW(f2c::rp::CustomOrder({9,1,8,1,7,2,6,3,5,4}));
}

TEST(fields2cover_route_custom_order, out_of_range) {
  const int n = 11;
  F2CSwaths swaths = genSwathsTest(n);
  EXPECT_ANY_THROW(f2c::rp::CustomOrder({10,0,8,1,7,2,6,3,5,4}));
}

TEST(fields2cover_route_custom_order, shorter_order) {
  const int n = 10;
  F2CSwaths swaths = genSwathsTest(n);
  f2c::rp::CustomOrder swath_sorter({3,1,2,0});
  swaths = swath_sorter.genSortedSwaths(swaths);

  EXPECT_TRUE(isRouteOrderCorrect(swaths, {4,2,3,1,8,6,7,5,9}));
}
