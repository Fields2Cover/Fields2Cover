//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_field, area) {
  F2CField field;
  EXPECT_EQ(field.getArea(), 0);

  F2CCells cells {
    F2CCell(F2CLinearRing({
          F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)
    }))
  };
  field.field = cells;
  EXPECT_EQ(field.getArea(), 4);
  const auto field2 = field;
  EXPECT_EQ(field2.getArea(), 4);
}


