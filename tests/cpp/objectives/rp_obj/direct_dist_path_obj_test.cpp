//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/objectives/rp_obj/direct_dist_path_obj.h"

TEST(fields2cover_obj_direct_dist_path_obj, computeCost_cost) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 1.0), F2CPoint(4.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(4.0, 3.0), F2CPoint(0.0, 3.0)}));
  F2CSwath swath3(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(1.0, 1.0)}));

  auto createRoute = [](const F2CSwaths& s) {
    F2CRoute route;
    route.addConnection();
    route.addSwaths(s);
    route.addConnection();
    return route;
  };

  F2CSwaths swaths1 {swath1};
  F2CSwaths swaths2 {swath2};
  F2CSwaths swaths4 {swath1,swath2,swath1,swath2};

  auto r1 = createRoute(swaths1);
  auto r2 = createRoute(swaths2);
  auto r4 = createRoute(swaths4);

  f2c::obj::DirectDistPathObj length;

  F2CPoint p1(0,0), p2(4,0);

  EXPECT_EQ(length.computeCost(p1, p2), 4.0);

  std::vector<F2CPoint> ps1 {p1, p2};
  F2CMultiPoint ps2;
  ps2.addGeometry(p1);
  ps2.addGeometry(p2);

  EXPECT_EQ(length.computeCost(ps1), 4.0);
  EXPECT_EQ(length.computeCost(ps2), 4.0);
  EXPECT_EQ(length.computeCost(p1, p2, 0.0), 4.0);
  EXPECT_EQ(length.computeCost(p1, 0.0, p2), 4.0);
  EXPECT_EQ(length.computeCost(p1, 0.0, p2, 0.0), 4.0);
  EXPECT_EQ(length.computeCost(p1, swath2), 5.0);
  EXPECT_EQ(length.computeCost(swath2, p1), 3.0);
  EXPECT_EQ(length.computeCost(p1, 0.0, swath2), 5.0);
  EXPECT_EQ(length.computeCost(swath2, p1, 0.0), 3.0);

  EXPECT_EQ(length.computeCost(swath2, ps2), 3.0);
  EXPECT_EQ(length.computeCost(swaths2, ps2), 3.0);
  EXPECT_EQ(length.computeCost(ps2, swath2), 3.0);
  EXPECT_EQ(length.computeCost(ps2, swaths2), 3.0);

  EXPECT_EQ(length.computeCost(swath2), 4.0);
  EXPECT_EQ(length.computeCost(swaths4), 22.0);

  EXPECT_EQ(length.computeCost(F2CMultiPoint(), swath2), 0.0);
  EXPECT_EQ(length.computeCost(swath2, F2CMultiPoint()), 0.0);

  EXPECT_EQ(length.computeCost(r1), 4.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(r1), length.computeCost(r1));
  EXPECT_EQ(length.computeCost(r2), 4.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(r2), length.computeCost(r2));
  EXPECT_EQ(length.computeCost(r4), 22.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(r4), length.computeCost(r4));
}

TEST(fields2cover_obj_direct_dist_path_obj, params_check) {
  f2c::obj::DirectDistPathObj objective;

  EXPECT_FALSE(objective.isMaximizing());
  EXPECT_TRUE(objective.isMinimizing());
}

