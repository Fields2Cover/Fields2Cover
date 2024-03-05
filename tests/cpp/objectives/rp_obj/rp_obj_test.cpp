//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/objectives/rp_obj/rp_objective.h"

TEST(fields2cover_obj_rp_obj, computeCost_cost) {
  f2c::obj::RPObjective length;
  F2CPoint p1(0,0), p2(4,0);
  F2CMultiPoint ps {p1, p2};
  std::vector<F2CPoint> vp {p1, p2};
  EXPECT_EQ(length.computeCost(p1, p2), 4.0);
  EXPECT_EQ(length.computeCost(p1, 0.0, p2, 0.0), 4.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(p1, 0.0, p2, 0.0), 4.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(p1, p2, 0.0), 4.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(p1, 0.0, p2), 4.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(p1, p2), 4.0);

  F2CSwath s1(F2CLineString({F2CPoint(0, 0), F2CPoint(1, 1)}), 1.0, 1);
  F2CSwath s2(F2CLineString({F2CPoint(0, 0), F2CPoint(2, 2)}), 1.0, 2);
  F2CSwaths ss {s1, s2};

  EXPECT_EQ(length.computeCostWithMinimizingSign(s1), sqrt(2.0));
  EXPECT_EQ(length.computeCostWithMinimizingSign(s1, s2), sqrt(2.0));
  EXPECT_EQ(length.computeCostWithMinimizingSign(ss), 4*sqrt(2.0));
  EXPECT_EQ(length.computeCostWithMinimizingSign(s1, p1, 0.0), sqrt(2.0));
  EXPECT_EQ(length.computeCostWithMinimizingSign(p2, 0.0, s1), 4.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(p2, s1), 4.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(ps, s1), 4.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(s2, p1), 2.0*sqrt(2.0));
  EXPECT_EQ(length.computeCostWithMinimizingSign(s2, ps), 2.0*sqrt(2.0));
  EXPECT_EQ(length.computeCostWithMinimizingSign(ps, ss), 4.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(ss, ps), 2.0*sqrt(2.0));
  EXPECT_EQ(length.computeCostWithMinimizingSign(ps), 4.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(vp), 4.0);
}

