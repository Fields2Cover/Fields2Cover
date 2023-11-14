//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_multipoint, init) {
  F2CMultiPoint ps1;
  EXPECT_EQ(ps1.size(), 0);
  ps1.addPoint(1,1);
  ps1.addPoint(2,1);
  EXPECT_EQ(ps1.size(), 2);
  F2CMultiPoint ps2{F2CPoint(1,2), F2CPoint(3,2), F2CPoint(1,3)};
  EXPECT_EQ(ps2.size(), 3);
  F2CMultiPoint ps3(std::vector<F2CPoint>({F2CPoint(1,2), F2CPoint(3,2), F2CPoint(1,3)}));
  EXPECT_EQ(ps3.size(), 3);

  F2CMultiPoint ps4;
  for (int i = 0; i < 10; ++i) {
    ps4.addPoint( i, -i, 3 * i);
  }
  EXPECT_EQ(ps4.size(), 10);
  int i = 0;
  for (const auto& p : ps4) {
    EXPECT_EQ(p.getX(), i);
    EXPECT_EQ(p.getY(), -i);
    EXPECT_EQ(p.getZ(), 3*i);
    ++i;
  }
  for (auto&& p : ps4) {
    p = p + F2CPoint(-1,2,-3);
  }
  i = 0;
  for (auto&& p : ps4) {
    EXPECT_EQ(p.getX(), i -1);
    EXPECT_EQ(p.getY(), -i +2);
    EXPECT_EQ(p.getZ(), 3*i -3);
    ++i;
  }
  F2CPoint p_error;
  EXPECT_THROW(ps4.getGeometry(30, p_error), std::out_of_range);
  EXPECT_THROW(ps4.getGeometry(30), std::out_of_range);

  ps4 *= 2;
  const auto ps5 = ps4.clone();
  EXPECT_EQ(ps5.size(), 10);
  for (int i = 0; i < ps5.size(); ++i) {
    F2CPoint p = ps5.getGeometry(i);
    EXPECT_EQ(p.getX(), 2*(i -1));
    EXPECT_EQ(p.getY(), 2*(-i +2));
    EXPECT_EQ(p.getZ(), 2*(3*i -3));
  }
  EXPECT_THROW(ps5.getGeometry(30, p_error), std::out_of_range);
  EXPECT_THROW(ps5.getGeometry(30), std::out_of_range);

  ps4.addPoints(ps1);
  EXPECT_EQ(ps4.size(), 12);
  for (int i = 0; i < ps5.size(); ++i) {
    F2CPoint p = ps4.getGeometry(i);
    EXPECT_EQ(p.getX(), 2*(i -1));
    EXPECT_EQ(p.getY(), 2*(-i +2));
    EXPECT_EQ(p.getZ(), 2*(3*i -3));
  }
  EXPECT_EQ(ps4.getGeometry(10), F2CPoint(1,1));
  EXPECT_EQ(ps4.getGeometry(11), F2CPoint(2,1));

}

TEST(fields2cover_types_multipoint, getGeometry) {
  F2CMultiPoint ps1{F2CPoint(1,2), F2CPoint(3,2,5), F2CPoint(1,3), F2CPoint(9,4, 0.2)};
  EXPECT_EQ(ps1.size(), 4);
  EXPECT_EQ(ps1.getGeometry(0).getX(), 1);
  EXPECT_EQ(ps1.getGeometry(0).getY(), 2);
  EXPECT_EQ(ps1.getGeometry(0).getZ(), 0);
  EXPECT_EQ(ps1.getGeometry(3).getX(), 9);
  EXPECT_EQ(ps1.getGeometry(3).getY(), 4);
  EXPECT_EQ(ps1.getGeometry(3).getZ(), 0.2);

  const F2CMultiPoint ps2 = ps1.clone();
  EXPECT_EQ(ps2.size(), 4);
  EXPECT_EQ(ps2.getGeometry(1).getX(), 3);
  EXPECT_EQ(ps2.getGeometry(1).getY(), 2);
  EXPECT_EQ(ps2.getGeometry(1).getZ(), 5);
  EXPECT_EQ(ps2.getGeometry(2).getX(), 1);
  EXPECT_EQ(ps2.getGeometry(2).getY(), 3);
  EXPECT_EQ(ps2.getGeometry(2).getZ(), 0);
}



