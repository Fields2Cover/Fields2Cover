//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
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

  EXPECT_EQ(cells.area(), 1);
  EXPECT_EQ(cells_clone.area(), 1);
  EXPECT_EQ(cells_clone.getGeometry(0).area(), 1);


  auto ring2 = ring1.clone();
  ring2 *= 2.0;
  auto ring3 = ring1 + F2CPoint(0.5, 0.5);
  F2CCell cell2;
  EXPECT_EQ(cell2.area(), 0);
  cells.addGeometry(cell2);
  cells.addRing(1, ring2);
  cells.addRing(1, ring3);
  EXPECT_EQ(cells.getInteriorRing(1, 0).length(), 4);

  OGRGeometryCollection gc;
  F2CCells empty_cells{&gc};
  EXPECT_EQ(empty_cells.size(), 0);

  F2CCell error_cell;
  EXPECT_THROW(cells.getGeometry(100, error_cell), std::out_of_range);
  EXPECT_THROW(cells_clone.getGeometry(100, error_cell), std::out_of_range);
  EXPECT_THROW(cells.getGeometry(100), std::out_of_range);
  EXPECT_THROW(cells_clone.getGeometry(100), std::out_of_range);
}

TEST(fields2cover_types_cells, difference) {
  F2CCell cell1, cell2;
  F2CLinearRing ring1{F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  F2CLinearRing ring2{F2CPoint(1,0), F2CPoint(3,0),F2CPoint(3,2),F2CPoint(1,2), F2CPoint(1,0)};
  cell1.addRing(ring1);
  cell2.addRing(ring2);

  EXPECT_EQ(ring1.size(), 5);
  EXPECT_EQ(ring2.size(), 5);
  EXPECT_EQ(cell1.area(), 4);
  EXPECT_EQ(cell2.area(), 4);
  EXPECT_EQ(F2CCells(cell1).area(), 4);
  EXPECT_EQ(F2CCells(cell2).area(), 4);
  EXPECT_EQ(F2CCells(cell1).difference(F2CCells(cell2)).area(), 2);
}

TEST(fields2cover_types_cells, unionOp) {
  F2CCell cell1, cell2;
  F2CLinearRing ring1{F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  F2CLinearRing ring2{F2CPoint(1,0), F2CPoint(3,0),F2CPoint(3,2),F2CPoint(1,2), F2CPoint(1,0)};
  cell1.addRing(ring1);
  cell2.addRing(ring2);

  EXPECT_EQ(ring1.size(), 5);
  EXPECT_EQ(ring2.size(), 5);
  EXPECT_EQ(cell1.area(), 4);
  EXPECT_EQ(cell2.area(), 4);
  EXPECT_EQ(F2CCells(cell1).area(), 4);
  EXPECT_EQ(F2CCells(cell2).area(), 4);
  EXPECT_EQ(F2CCells(cell1).unionOp(F2CCells(cell2)).area(), 6);
}

TEST(fields2cover_types_cells, buffer) {
  F2CLineString line {
    F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  F2CCells cells = F2CCells::buffer(line, 2);
  EXPECT_GE(cells.area(), 32);
  EXPECT_LE(cells.area(), 36);
  cells = F2CCells::buffer(line, -1);
  EXPECT_EQ(cells.area(), 0);

  cells = F2CCells::buffer(line, 2);
  F2CCells cells2 = F2CCells::buffer(cells, -0.1);
  EXPECT_LT(cells2.area(), cells.area());
  EXPECT_GT(cells2.area(), 0.8 * cells.area());

  cells = F2CCells::buffer(F2CLineString(), 20);
  EXPECT_EQ(cells.area(), 0);
}

TEST(fields2cover_types_cells, intersection) {
  F2CCell cell1, cell2;
  F2CLinearRing ring1{F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  F2CLinearRing ring2{F2CPoint(1,0), F2CPoint(3,0),F2CPoint(3,2),F2CPoint(1,2), F2CPoint(1,0)};
  cell1.addRing(ring1);
  cell2.addRing(ring2);
  F2CCells cells2{cell2};
  EXPECT_EQ(F2CCells::intersection(cell1, cell2).area(), 2);
  EXPECT_EQ(cells2.intersection(cell1).area(), 2);
}


TEST(fields2cover_types_cells, LongLines) {
  F2CCells cells {
    F2CCell(F2CLinearRing({
          F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)
    }))
  };

  auto line1 = cells.createSemiLongLine(F2CPoint(0,0), 3.0*M_PI/4.0);
  auto line2 = cells.createStraightLongLine(F2CPoint(0,0), 3.0*M_PI/4.0);

  EXPECT_NEAR(line1.getX(0), 0, 1e-3);
  EXPECT_NEAR(line1.getY(0), 0, 1e-3);
  EXPECT_NEAR(line1.getX(1), -2*sqrt(2.), 1e-3);
  EXPECT_NEAR(line1.getY(1), 2*sqrt(2.), 1e-3);
  EXPECT_NEAR(line2.getX(0), 2*sqrt(2.), 1e-3);
  EXPECT_NEAR(line2.getY(0), -2*sqrt(2.), 1e-3);
  EXPECT_NEAR(line2.getX(1), -2*sqrt(2.), 1e-3);
  EXPECT_NEAR(line2.getY(1), 2*sqrt(2.), 1e-3);
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
  EXPECT_EQ(cells.getCellWherePoint(F2CPoint(5, 5)).area(), 0);
  EXPECT_EQ(cells.getCellWherePoint(F2CPoint(1, 1)).area(), 4);
  EXPECT_EQ(cells.getCellWherePoint(F2CPoint(0, 0)).area(), 4);
  EXPECT_EQ(cells.getCellWherePoint(F2CPoint(11, 11)).area(), 6);
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
  EXPECT_EQ(cells.area(), 0);
  EXPECT_EQ(cells.size(), 0);

  cells.setGeometry(0, cell2);
  EXPECT_EQ(cells.area(), 16);
  EXPECT_EQ(cells.size(), 1);

  cells.setGeometry(0, cell1);
  EXPECT_EQ(cells.area(), 4);
  EXPECT_EQ(cells.size(), 1);

  cells.setGeometry(1, F2CCell());
  EXPECT_EQ(cells.area(), 4);
  EXPECT_EQ(cells.size(), 2);

  cells.setGeometry(10, cell2);
  EXPECT_EQ(cells.area(), 20);
  EXPECT_EQ(cells.size(), 11);

  cells.setGeometry(0, cell3);
  EXPECT_EQ(cells.area(), 32);
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
  EXPECT_NEAR(split_cells.getGeometry(0).area(), 9, 1e-7);
  EXPECT_NEAR(split_cells.getGeometry(1).area(), 3, 1e-7);
  EXPECT_NEAR(split_cells.getGeometry(2).area(), 2, 1e-7);
  EXPECT_NEAR(split_cells.getGeometry(3).area(), 6, 1e-7);
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


