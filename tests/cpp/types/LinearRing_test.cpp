//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_linearring, init) {
  F2CLinearRing ring1;
  ring1.addPoint(1,1);
  ring1.addPoint(2,1);
  EXPECT_EQ(ring1.size(), 2);
  F2CLinearRing ring2{F2CPoint(1,2), F2CPoint(3,2), F2CPoint(1,3)};
  EXPECT_EQ(ring2.size(), 3);
}

TEST(fields2cover_types_linearring, mult_equal) {
  F2CLinearRing ring{F2CPoint(1,2), F2CPoint(3,2), F2CPoint(1,3)};
  ring *= 10.0;
  EXPECT_EQ(ring.size(), 3);
  EXPECT_EQ(ring.getGeometry(0).getX(), 10);
  EXPECT_EQ(ring.getGeometry(1).getX(), 30);
  EXPECT_EQ(ring.getGeometry(2).getX(), 10);
  EXPECT_EQ(ring.getGeometry(0).getY(), 20);
  EXPECT_EQ(ring.getGeometry(1).getY(), 20);
  EXPECT_EQ(ring.getGeometry(2).getY(), 30);
  ring *= -0.01;
  auto p = ring.EndPoint();
  EXPECT_EQ(p.getX(), -0.1);
  EXPECT_EQ(p.getY(), -0.3);
  const auto ring2 = ring.clone();
  auto c_p = ring2.getGeometry(1);
  EXPECT_EQ(c_p.getX(), -0.3);
  EXPECT_EQ(c_p.getY(), -0.2);
}


