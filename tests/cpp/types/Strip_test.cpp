//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_strip, clone) {
  F2CSwath swath(F2CLineString({F2CPoint(0, 0), F2CPoint(0, 10)}), 2.0, 1);
  F2CCells cells = swath.computeAreaCovered();
  F2CStrip strip1;
  strip1.cell = cells.getGeometry(0);
  strip1.name = "TEST";
  auto strip2 = strip1.clone();
  EXPECT_EQ(strip1.name, "TEST");
  EXPECT_EQ(strip2.name, "TEST");
  EXPECT_NEAR(strip1.cell.getArea(), 20.0, 1e-2);
  EXPECT_NEAR(strip2.cell.getArea(), 20.0, 1e-2);
}

