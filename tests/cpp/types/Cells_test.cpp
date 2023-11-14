//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
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
  const F2CCells cells_clone {cells.get()};

  EXPECT_EQ(cells.getArea(), 1);
  EXPECT_EQ(cells_clone.getArea(), 1);
  EXPECT_THROW(F2CCells(ring1.get()), std::invalid_argument);
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

  OGRGeometryCollection gc;
  F2CCells empty_cells{&gc};
  EXPECT_EQ(empty_cells.size(), 0);

  F2CCell error_cell;
  EXPECT_THROW(cells.getGeometry(100, error_cell), std::out_of_range);
  EXPECT_THROW(cells_clone.getGeometry(100, error_cell), std::out_of_range);
  EXPECT_THROW(cells.getGeometry(100), std::out_of_range);
  EXPECT_THROW(cells_clone.getGeometry(100), std::out_of_range);
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

TEST(fields2cover_types_cells, Union) {
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
  EXPECT_EQ(F2CCells(cell1).Union(F2CCells(cell2)).getArea(), 6);
}

TEST(fields2cover_types_cells, Buffer) {
  F2CLineString line {
    F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  F2CCells cells = F2CCells::Buffer(line, 2);
  EXPECT_GE(cells.getArea(), 32);
  EXPECT_LE(cells.getArea(), 36);
  cells = F2CCells::Buffer(line, -1);
  EXPECT_EQ(cells.getArea(), 0);

  cells = F2CCells::Buffer(line, 2);
  F2CCells cells2 = F2CCells::Buffer(cells, -0.1);
  EXPECT_LT(cells2.getArea(), cells.getArea());
  EXPECT_GT(cells2.getArea(), 0.8 * cells.getArea());

  cells = F2CCells::Buffer(F2CLineString(), 20);
  EXPECT_EQ(cells.getArea(), 0);
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

TEST(fields2cover_types_cells, setGeometry) {
  F2CLinearRing line {
    F2CPoint(0,0), F2CPoint(2,0), F2CPoint(2,2), F2CPoint(0,2), F2CPoint(0,0)};
  F2CLinearRing line2 {
    F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,4), F2CPoint(0,4), F2CPoint(0,0)};
  F2CCell cell1 {line + F2CPoint(100,100)};
  F2CCell cell2 {line2 + F2CPoint(200,0)};
  F2CCell cell3 {line2 + F2CPoint(-30,-80)};
  F2CCells cells;
  EXPECT_EQ(cells.getArea(), 0);
  EXPECT_EQ(cells.size(), 0);

  cells.setGeometry(0, cell2);
  EXPECT_EQ(cells.getArea(), 16);
  EXPECT_EQ(cells.size(), 1);

  cells.setGeometry(0, cell1);
  EXPECT_EQ(cells.getArea(), 4);
  EXPECT_EQ(cells.size(), 1);

  cells.setGeometry(1, F2CCell());
  EXPECT_EQ(cells.getArea(), 4);
  EXPECT_EQ(cells.size(), 2);

  cells.setGeometry(10, cell2);
  EXPECT_EQ(cells.getArea(), 20);
  EXPECT_EQ(cells.size(), 11);

  cells.setGeometry(0, cell3);
  EXPECT_EQ(cells.getArea(), 32);
  EXPECT_EQ(cells.size(), 11);
}


TEST(fields2cover_types_cells, splitByLine) {
  F2CCells cells {F2CCell(F2CLinearRing({
    F2CPoint(0,0), F2CPoint(5,0), F2CPoint(5,4), F2CPoint(0,4), F2CPoint(0,0)}))};
  F2CLineString line1{F2CPoint(3,0), F2CPoint(3,4)};
  F2CLineString line2 = cells.createLineUntilBorder(F2CPoint(0,3), 0.0);
  F2CMultiLineString lines;
  lines.setGeometry(0, line1);
  lines.setGeometry(1, line2);

  F2CCells split_cells = cells.splitByLine(lines);
  EXPECT_NEAR(split_cells.size(), 4, 1e-7);
  EXPECT_NEAR(split_cells.getGeometry(0).getArea(), 9, 1e-7);
  EXPECT_NEAR(split_cells.getGeometry(1).getArea(), 3, 1e-7);
  EXPECT_NEAR(split_cells.getGeometry(2).getArea(), 2, 1e-7);
  EXPECT_NEAR(split_cells.getGeometry(3).getArea(), 6, 1e-7);
}

TEST(fields2cover_types_cells, isPointIn) {
  F2CCell cell;
  F2CLinearRing ring{
    F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  cell.addRing(ring);
  F2CCells cells{cell};

  EXPECT_TRUE(cells.isPointIn(F2CPoint(1,1)));
  EXPECT_FALSE(cells.isPointIn(F2CPoint(2,2)));
  EXPECT_FALSE(cells.isPointIn(F2CPoint(-1,1)));
  EXPECT_FALSE(cells.isPointIn(F2CPoint(10,-10)));
}
