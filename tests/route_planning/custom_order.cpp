#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/objectives/direct_dist_path_obj.h"
#include "fields2cover/route_planning/custom_order.h"

TEST(fields2cover_custom_order, defined_order) {
  const int n = 11;
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(0, i), F2CPoint(1, i)}), i, i-1);
  }

  f2c::rp::CustomOrder swath_sorter(swaths);
  swath_sorter.set_custom_order({1,0,3,2,5,4,7,6,8,9});
  
  swaths = swath_sorter.genSortedSwaths();

  EXPECT_EQ(swaths[0].getId(), 1);
  EXPECT_EQ(swaths[1].getId(), 0);
  EXPECT_EQ(swaths[2].getId(), 3);
  EXPECT_EQ(swaths[3].getId(), 2);
  EXPECT_EQ(swaths[4].getId(), 5);
  EXPECT_EQ(swaths[5].getId(), 4);
  EXPECT_EQ(swaths[6].getId(), 7);
  EXPECT_EQ(swaths[7].getId(), 6);
  EXPECT_EQ(swaths[8].getId(), 8);
  EXPECT_EQ(swaths[9].getId(), 9);
}

TEST(fields2cover_custom_order, constructor_order) {
  const int n = 11;
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(0, i), F2CPoint(1, i)}), i, i-1);
  }

  f2c::rp::CustomOrder swath_sorter(swaths, {9,0,8,1,7,2,6,3,5,4});

  f2c::obj::DirectDistPathObj objective;
  
  swaths = swath_sorter.genSortedSwaths();  

  EXPECT_EQ(swaths[0].getId(), 9);
  EXPECT_EQ(swaths[1].getId(), 0);
  EXPECT_EQ(swaths[2].getId(), 8);
  EXPECT_EQ(swaths[3].getId(), 1);
  EXPECT_EQ(swaths[4].getId(), 7);
  EXPECT_EQ(swaths[5].getId(), 2);
  EXPECT_EQ(swaths[6].getId(), 6);
  EXPECT_EQ(swaths[7].getId(), 3);
  EXPECT_EQ(swaths[8].getId(), 5);
  EXPECT_EQ(swaths[9].getId(), 4);
}
