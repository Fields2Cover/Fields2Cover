//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_linestring, init) {
  F2CLineString line1;
  line1.addPoint(1,1);
  line1.addPoint(2,1);
  EXPECT_EQ(line1.size(), 2);
  F2CLineString line2({F2CPoint(1,2), F2CPoint(3,2), F2CPoint(1,3)});
  EXPECT_EQ(line2.size(), 3);
  EXPECT_EQ(line2.getX(0), 1);
  EXPECT_EQ(line2.getX(1), 3);
  F2CLineString line3{F2CPoint(2,3)};
  EXPECT_EQ(line3.size(), 1);
  EXPECT_EQ(line3.getX(0), 2);
  const F2CPoint p {20, 30};
  F2CLineString line4{p};
  EXPECT_EQ(line4.size(), 1);
  auto p2 = line4.getGeometry(0);
  EXPECT_EQ(p2.getX(), 20);
  EXPECT_EQ(p2.getY(), 30);
}


