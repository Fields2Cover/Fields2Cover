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

TEST(fields2cover_types_field, clone) {
  F2CCells cells {F2CCell(F2CLinearRing({
      F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)
  }))};
  F2CField field{F2CCells()};
  field.field = cells;
  field.setUTMCoordSystem("42");
  EXPECT_EQ(field.getArea(), 4);
  EXPECT_TRUE(field.isCoordSystemUTM());
  EXPECT_EQ(field.getUTMCoordSystem(), "42");

  const auto field2 = field.clone();
  EXPECT_EQ(field.getArea(), 4);
  EXPECT_TRUE(field.isCoordSystemUTM());
  EXPECT_FALSE(field.isCoordSystemEPSG());
  EXPECT_EQ(field.getUTMCoordSystem(), "42");
  EXPECT_EQ(field.getEPSGCoordSystem(), -1);
  EXPECT_EQ(field2.getArea(), 4);
  EXPECT_TRUE(field2.isCoordSystemUTM());
  EXPECT_EQ(field2.getUTMCoordSystem(), "42");
}

TEST(fields2cover_types_field, getCellsAbsPosition) {
  F2CCells cells {F2CCell(F2CLinearRing({
      F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)
  }))};
  cells = cells + F2CPoint(5, 6);
  EXPECT_EQ(cells.getGeometry(0).getGeometry(0).getX(0), 5);
  EXPECT_EQ(cells.getGeometry(0).getGeometry(0).getY(0), 6);
  EXPECT_EQ(cells.getGeometry(0).getGeometry(0).getX(1), 7);
  EXPECT_EQ(cells.getGeometry(0).getGeometry(0).getY(1), 6);

  F2CField field(cells);
  field.setEPSGCoordSystem(12345);
  EXPECT_FALSE(field.isCoordSystemUTM());
  EXPECT_TRUE(field.isCoordSystemEPSG());
  EXPECT_EQ(field.getEPSGCoordSystem(), 12345);
  EXPECT_EQ(field.getUTMCoordSystem(), "");
  EXPECT_EQ(field.ref_point.getX(), 5);
  EXPECT_EQ(field.ref_point.getY(), 6);
  EXPECT_EQ(field.field.getGeometry(0).getGeometry(0).getX(0), 0);
  EXPECT_EQ(field.field.getGeometry(0).getGeometry(0).getY(0), 0);
  EXPECT_NE(field.field.getGeometry(0).getGeometry(0).getX(1), 0);
  EXPECT_EQ(field.field.getGeometry(0).getGeometry(0).getY(1), 0);

  F2CCells o_cells = field.getCellsAbsPosition();
  EXPECT_EQ(o_cells.getGeometry(0).getGeometry(0).getX(0), 5);
  EXPECT_EQ(o_cells.getGeometry(0).getGeometry(0).getY(0), 6);
  EXPECT_EQ(o_cells.getGeometry(0).getGeometry(0).getX(1), 7);
  EXPECT_EQ(o_cells.getGeometry(0).getGeometry(0).getY(1), 6);
}

