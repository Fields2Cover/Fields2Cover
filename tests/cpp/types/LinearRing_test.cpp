//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_linearring, init) {
  F2CLinearRing ring1;
  EXPECT_EQ(ring1.size(), 0);

  ring1.addPoint(1,1);
  ring1.addPoint(2,1);
  EXPECT_EQ(ring1.size(), 2);

  F2CLinearRing ring2{F2CPoint(1,2), F2CPoint(3,2), F2CPoint(1,3)};
  EXPECT_EQ(ring2.size(), 3);


  F2CPoint error_p;
  const F2CLinearRing c_line = ring1.clone();
  EXPECT_THROW(ring1.getGeometry(100, error_p), std::out_of_range);
  EXPECT_THROW(c_line.getGeometry(100, error_p), std::out_of_range);
  EXPECT_THROW(ring1.getGeometry(100), std::out_of_range);
  EXPECT_THROW(c_line.getGeometry(100), std::out_of_range);
}

TEST(fields2cover_types_linearring, getArea) {
  std::vector<F2CPoint> ps{F2CPoint(-1,2), F2CPoint(3,2), F2CPoint(-1,3), F2CPoint(-1,2)};
  F2CLinearRing ring{ps};
  EXPECT_EQ(ring.getArea(), 2);
}

TEST(fields2cover_types_linearring, mult_equal) {
  std::vector<F2CPoint> ps{F2CPoint(1,2), F2CPoint(3,2), F2CPoint(1,3)};
  F2CLinearRing ring{ps};
  ring *= 10.0;
  EXPECT_EQ(ring.size(), 3);
  EXPECT_EQ(ring.getGeometry(0).getX(), 10);
  EXPECT_EQ(ring.getGeometry(1).getX(), 30);
  EXPECT_EQ(ring.getGeometry(2).getX(), 10);
  EXPECT_EQ(ring.getY(0), 20);
  EXPECT_EQ(ring.getY(1), 20);
  EXPECT_EQ(ring.getY(2), 30);
  EXPECT_EQ(ring.getZ(2), 0);
  ring *= -0.01;
  auto p = ring.EndPoint();
  EXPECT_EQ(p.getX(), -0.1);
  EXPECT_EQ(p.getY(), -0.3);
  EXPECT_EQ(p.getZ(), 0);
  const auto ring2 = ring.clone();
  auto c_p = ring2.getGeometry(1);
  EXPECT_EQ(c_p.getX(), -0.3);
  EXPECT_EQ(c_p.getY(), -0.2);
}


TEST(fields2cover_types_linearring, loop) {
  F2CLinearRing line(std::vector<F2CPoint>({F2CPoint(1,2), F2CPoint(3,4), F2CPoint(5,6)}));
  int i = 1;
  for (const F2CPoint& p : line) {
    EXPECT_EQ(p, F2CPoint(i, i+1));
    i += 2;
  }
  i = 1;
  for (F2CPoint& p : line) {
    EXPECT_EQ(p, F2CPoint(i, i+1));
    i += 2;
  }
  i = 1;
  for (auto&& p : line) {
    EXPECT_EQ(p, F2CPoint(i, i+1));
    i += 2;
  }
}


