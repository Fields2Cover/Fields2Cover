//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_cells, constructor) {
  F2CLinearRing ring1{
    F2CPoint(0,0), F2CPoint(1,0),F2CPoint(1, 1),F2CPoint(0, 1), F2CPoint(0, 0)};
  F2CCell cell {ring1};
  F2CCells cells;
  EXPECT_EQ(cells.size(), 0);
  cells.addGeometry(cell);
  const F2CCells cells_clone{cells.get()->clone()};

  EXPECT_EQ(cells.getArea(), 1);
  EXPECT_EQ(cells_clone.getArea(), 1);
  EXPECT_THROW(F2CCells(ring1.get()->clone()), std::invalid_argument);
  EXPECT_EQ(cells_clone.getGeometry(0).getArea(), 1);


  auto ring2 = ring1.clone();
  ring2 *= 2.0;
  auto ring3 = ring1 + F2CPoint(0.5, 0.5);
  F2CCell cell2;
  EXPECT_EQ(cell2.getArea(), 0);
  cells.addGeometry(cell2);
  cells.addRing(1, ring2);
  cells.addRing(1, ring3);
  EXPECT_EQ(cells.getInteriorRing(1, 0).getLength(), 4);
}

TEST(fields2cover_types_cells, Difference) {
  F2CCell cell1, cell2;
  F2CLinearRing ring1{F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  F2CLinearRing ring2{F2CPoint(1,0), F2CPoint(3,0),F2CPoint(3,2),F2CPoint(1,2), F2CPoint(1,0)};
  cell1.addRing(ring1);
  cell2.addRing(ring2);

  EXPECT_EQ(ring1.size(), 5);
  EXPECT_EQ(ring2.size(), 5);
  EXPECT_EQ(cell1.getArea(), 4);
  EXPECT_EQ(cell2.getArea(), 4);
  EXPECT_EQ(F2CCells(cell1).getArea(), 4);
  EXPECT_EQ(F2CCells(cell2).getArea(), 4);
  EXPECT_EQ(F2CCells(cell1).Difference(F2CCells(cell2)).getArea(), 2);
}

TEST(fields2cover_types_cells, Buffer) {
  F2CLineString line {
    F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  F2CCells cells = F2CCells::Buffer(line, 2);
  EXPECT_GT(cells.getArea(), 4);
}

TEST(fields2cover_types_cells, isConvex) {
  F2CLineString line {
    F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  F2CCells cells = F2CCells::Buffer(line, 2);
  EXPECT_GT(cells.getArea(), 4);
}

TEST(fields2cover_types_cells, Intersection) {
  F2CCell cell1, cell2;
  F2CLinearRing ring1{F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  F2CLinearRing ring2{F2CPoint(1,0), F2CPoint(3,0),F2CPoint(3,2),F2CPoint(1,2), F2CPoint(1,0)};
  cell1.addRing(ring1);
  cell2.addRing(ring2);
  F2CCells cells2{cell2};
  EXPECT_EQ(F2CCells::Intersection(cell1, cell2).getArea(), 2);
  EXPECT_EQ(cells2.Intersection(cell1).getArea(), 2);
}


TEST(fields2cover_types_cells, LongLines) {
  F2CCells cells {
    F2CCell(F2CLinearRing({
          F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)
    }))
  };

  auto line1 = cells.getSemiLongCurve(F2CPoint(0,0), 3.0*M_PI/4.0);
  auto line2 = cells.getStraightLongCurve(F2CPoint(0,0), 3.0*M_PI/4.0);

  EXPECT_NEAR(line1.getX(0), 0, 1e-3);
  EXPECT_NEAR(line1.getY(0), 0, 1e-3);
  EXPECT_NEAR(line1.getX(1), -2*sqrt(2), 1e-3);
  EXPECT_NEAR(line1.getY(1), 2*sqrt(2), 1e-3);
  EXPECT_NEAR(line2.getX(0), -2*sqrt(2), 1e-3);
  EXPECT_NEAR(line2.getY(0), 2*sqrt(2), 1e-3);
  EXPECT_NEAR(line2.getX(1), 2*sqrt(2), 1e-3);
  EXPECT_NEAR(line2.getY(1), -2*sqrt(2), 1e-3);
}

TEST(fields2cover_types_cells, WherePoint) {
  F2CCells cells {
    F2CCell(F2CLinearRing({
          F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)
    }))
  };
  cells.addGeometry(F2CCell(F2CLinearRing({
    F2CPoint(10,9), F2CPoint(12,9),F2CPoint(12,12),F2CPoint(10,12), F2CPoint(10,9)
  })));

  EXPECT_TRUE(cells.isPointInBorder(F2CPoint(0, 0)));
  EXPECT_TRUE(cells.isPointInBorder(F2CPoint(1, 0)));
  EXPECT_TRUE(cells.isPointInBorder(F2CPoint(11, 12)));
  EXPECT_FALSE(cells.isPointInBorder(F2CPoint(1, 1)));
  EXPECT_FALSE(cells.isPointInBorder(F2CPoint(5, 5)));
  EXPECT_EQ(cells.getCellWherePoint(F2CPoint(5, 5)).getArea(), 0);
  EXPECT_EQ(cells.getCellWherePoint(F2CPoint(1, 1)).getArea(), 4);
  EXPECT_EQ(cells.getCellWherePoint(F2CPoint(0, 0)).getArea(), 4);
  EXPECT_EQ(cells.getCellWherePoint(F2CPoint(11, 11)).getArea(), 6);



}


