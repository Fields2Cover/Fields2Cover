#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/objectives/direct_dist_path_obj.h"
#include "fields2cover/route_planning/custom_order.h"

TEST(fields2cover_custom_order, defined_order) {
  const int n = 11;
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(0, i), F2CPoint(1, i)}), i, i);
  }

  f2c::rp::CustomOrder swath_sorter(swaths);
  swath_sorter.set_custom_order({1,0,3,2,5,4,7,6,8,9});
  
  swaths = swath_sorter.genSortedSwaths();

  EXPECT_EQ(swaths[0].getWidth(), 1);
  EXPECT_EQ(swaths[1].getWidth(), 0);
  EXPECT_EQ(swaths[2].getWidth(), 3);
  EXPECT_EQ(swaths[3].getWidth(), 2);
  EXPECT_EQ(swaths[4].getWidth(), 5);
  EXPECT_EQ(swaths[5].getWidth(), 4);
  EXPECT_EQ(swaths[6].getWidth(), 7);
  EXPECT_EQ(swaths[7].getWidth(), 6);
  EXPECT_EQ(swaths[8].getWidth(), 8);
  EXPECT_EQ(swaths[9].getWidth(), 9);
}

TEST(fields2cover_custom_order, constructor_order) {
  const int n = 11;
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(0, i), F2CPoint(1, i)}), i, i);
  }

  f2c::rp::CustomOrder swath_sorter(swaths, {9,0,8,1,7,2,6,3,5,4});

  f2c::obj::DirectDistPathObj objective;
  
  swaths = swath_sorter.genSortedSwaths();  

  EXPECT_EQ(swaths[0].getWidth(), 9);
  EXPECT_EQ(swaths[1].getWidth(), 0);
  EXPECT_EQ(swaths[2].getWidth(), 8);
  EXPECT_EQ(swaths[3].getWidth(), 1);
  EXPECT_EQ(swaths[4].getWidth(), 7);
  EXPECT_EQ(swaths[5].getWidth(), 2);
  EXPECT_EQ(swaths[6].getWidth(), 6);
  EXPECT_EQ(swaths[7].getWidth(), 3);
  EXPECT_EQ(swaths[8].getWidth(), 5);
  EXPECT_EQ(swaths[9].getWidth(), 4);
}
