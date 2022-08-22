//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_cell, constructor) {
  F2CLinearRing line{
    F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  F2CCell cell {line};
  F2CCell cell_clone{cell.get()->clone()};

  EXPECT_EQ(cell.getArea(), 4);
  EXPECT_EQ(cell_clone.getArea(), 4);
  EXPECT_THROW(F2CCell(line.get()->clone()), std::invalid_argument);
}

TEST(fields2cover_types_cell, getArea) {
  F2CCell cell(F2CLinearRing({
    F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)}));
  EXPECT_EQ(cell.getArea(), 4);
}


TEST(fields2cover_types_cell, addRing) {
  F2CCell cell1;
  F2CLinearRing ring1{
    F2CPoint(0, 0), F2CPoint(2, 0),F2CPoint(2, 2), F2CPoint(0, 2), F2CPoint(0, 0)};

  F2CLinearRing ring2{
    F2CPoint(0.5, 0.5), F2CPoint(1.5, 0.5), F2CPoint(1.5, 1.5), F2CPoint(0.5, 1.5), F2CPoint(0.5, 0.5)};

  EXPECT_EQ(ring1.size(), 5);
  EXPECT_EQ(ring2.size(), 5);

  cell1.addRing(ring1);
  EXPECT_EQ(cell1.getArea(), 4);

  cell1.addRing(ring2);
  EXPECT_EQ(cell1.getArea(), 3);
  EXPECT_EQ(cell1.size(), 2);

  auto cell_ring1 = cell1.getGeometry(0);
  EXPECT_EQ(cell_ring1.size(), 5);
  EXPECT_EQ(cell_ring1.getLength(), 8);
  auto cell_ring2 = cell1.getGeometry(1);
  EXPECT_EQ(cell_ring2.size(), 5);
  EXPECT_EQ(cell_ring2.getLength(), 4);

  F2CLinearRing cell_ring3, cell_ring4;
  cell1.getGeometry(0, cell_ring3);
  EXPECT_EQ(cell_ring3.size(), 5);
  EXPECT_EQ(cell_ring3.getLength(), 8);
  cell1.getGeometry(1, cell_ring4);
  EXPECT_EQ(cell_ring4.size(), 5);
  EXPECT_EQ(cell_ring4.getLength(), 4);

  const F2CCell cell2 = cell1.clone();

  F2CLinearRing cell_ring5, cell_ring6;
  cell2.getGeometry(0, cell_ring5);
  EXPECT_EQ(cell_ring5.size(), 5);
  EXPECT_EQ(cell_ring5.getLength(), 8);
  cell2.getGeometry(1, cell_ring6);
  EXPECT_EQ(cell_ring6.size(), 5);
  EXPECT_EQ(cell_ring6.getLength(), 4);

  auto cell_ring7 = cell2.getGeometry(0);
  EXPECT_EQ(cell_ring7.size(), 5);
  EXPECT_EQ(cell_ring7.getLength(), 8);
  auto cell_ring8 = cell2.getGeometry(1);
  EXPECT_EQ(cell_ring8.size(), 5);
  EXPECT_EQ(cell_ring8.getLength(), 4);
}

TEST(fields2cover_types_cell, createLineUntilBorder) {
  F2CCell cell;
  F2CLinearRing ring{
    F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  cell.addRing(ring);

  auto line = cell.createLineUntilBorder(F2CPoint(1, 1), 0);

  EXPECT_EQ(line.size(), 2);
  if (line.size() == 2) {
    EXPECT_EQ(line.getX(0), 1);
    EXPECT_EQ(line.getY(0), 1);
    EXPECT_EQ(line.getX(1), 2);
    EXPECT_EQ(line.getY(1), 1);
  }
}

TEST(fields2cover_types_cell, isPointInBorder) {
  F2CCell cell;
  F2CLinearRing ring{
    F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  cell.addRing(ring);

  EXPECT_TRUE(cell.isPointInBorder(F2CPoint(2,2)));
  EXPECT_TRUE(cell.isPointInBorder(F2CPoint(1,2)));
  EXPECT_FALSE(cell.isPointInBorder(F2CPoint(1,1)));
  EXPECT_FALSE(cell.isPointInBorder(F2CPoint(10,10)));
}

TEST(fields2cover_types_cell, Buffer) {
  F2CCell cell(F2CLinearRing({
    F2CPoint(0,0), F2CPoint(1,0),F2CPoint(1,1),F2CPoint(0,1), F2CPoint(0,0)}));
  EXPECT_EQ(cell.getArea(), 1);
  
  auto cell_buffer = F2CCell::Buffer(cell, 2.0);
  EXPECT_EQ(cell.getArea(), 1);
  EXPECT_NEAR(cell_buffer.getArea(), 1 + 4 * 2 + 2 * 2 * M_PI, 1e-2);

  F2CPoint p {1, 2};
  auto point_buffer = F2CCell::Buffer(p, 3.0);
  EXPECT_NEAR(point_buffer.getArea(), 3 * 3 * M_PI, 1e-1);
  EXPECT_EQ(p.getX(), 1);
  EXPECT_EQ(p.getY(), 2);

  F2CLineString line {F2CPoint(3,4), F2CPoint(3,6)};
  auto line_buffer = F2CCell::Buffer(line, 4.0);
  EXPECT_NEAR(line_buffer.getArea(), 2 * 2 * 4 + 4 * 4 * M_PI, 1e-1);
}
