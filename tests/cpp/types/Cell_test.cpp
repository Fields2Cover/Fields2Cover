//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_cell, constructor) {
  F2CLinearRing line{
    F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  F2CCell cell {line};
  F2CCell cell_clone{cell.get()};
  F2CCell cell2 {cell};
  F2CCell cell3 {static_cast<OGRGeometry*>(cell.get())};

  EXPECT_EQ(cell.area(), 4);
  EXPECT_EQ(cell2.area(), 4);
  EXPECT_EQ(cell3.area(), 4);
  EXPECT_EQ(cell_clone.area(), 4);
  EXPECT_EQ(F2CCell(cell.get()).area(), 4);
  EXPECT_EQ(F2CCell(cell).area(), 4);
  EXPECT_THROW(F2CCell(line.get()), std::invalid_argument);
}

TEST(fields2cover_types_cell, setGeometry) {
  F2CLinearRing line {
    F2CPoint(0,0), F2CPoint(2,0), F2CPoint(2,2), F2CPoint(0,2), F2CPoint(0,0)};
  F2CLinearRing line2 {
    F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,4), F2CPoint(0,4), F2CPoint(0,0)};
  F2CCell cell;
  EXPECT_EQ(cell.area(), 0);
  EXPECT_EQ(cell.size(), 0);

  cell.setGeometry(0, line2);
  EXPECT_EQ(cell.area(), 16);
  EXPECT_EQ(cell.size(), 1);

  cell.setGeometry(0, line);
  EXPECT_EQ(cell.area(), 4);
  EXPECT_EQ(cell.size(), 1);

  cell.setGeometry(1, F2CLinearRing());
  EXPECT_EQ(cell.area(), 4);
  EXPECT_EQ(cell.size(), 2);

  cell.setGeometry(10, line);
  EXPECT_EQ(cell.area(), 0);
  EXPECT_EQ(cell.size(), 11);

  cell.setGeometry(0, line2);
  EXPECT_EQ(cell.area(), 12);
  EXPECT_EQ(cell.size(), 11);
}

TEST(fields2cover_types_cell, area) {
  F2CCell cell(F2CLinearRing({
    F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)}));
  EXPECT_EQ(cell.area(), 4);
}

TEST(fields2cover_types_cell, eq_mult_operator) {
  F2CCell cell(F2CLinearRing({
    F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)}));
  cell *= 10.0;
  EXPECT_EQ(cell.area(), 400);
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
  EXPECT_EQ(cell1.area(), 4);

  cell1.addRing(ring2);
  EXPECT_EQ(cell1.area(), 3);
  EXPECT_EQ(cell1.size(), 2);

  auto cell_ring1 = cell1.getGeometry(0);
  EXPECT_EQ(cell_ring1.size(), 5);
  EXPECT_EQ(cell_ring1.length(), 8);
  auto cell_ring2 = cell1.getGeometry(1);
  EXPECT_EQ(cell_ring2.size(), 5);
  EXPECT_EQ(cell_ring2.length(), 4);

  F2CLinearRing cell_ring3, cell_ring4;
  cell1.getGeometry(0, cell_ring3);
  EXPECT_EQ(cell_ring3.size(), 5);
  EXPECT_EQ(cell_ring3.length(), 8);
  cell1.getGeometry(1, cell_ring4);
  EXPECT_EQ(cell_ring4.size(), 5);
  EXPECT_EQ(cell_ring4.length(), 4);

  const F2CCell cell2 = cell1.clone();

  F2CLinearRing cell_ring5, cell_ring6;
  cell2.getGeometry(0, cell_ring5);
  EXPECT_EQ(cell_ring5.size(), 5);
  EXPECT_EQ(cell_ring5.length(), 8);
  cell2.getGeometry(1, cell_ring6);
  EXPECT_EQ(cell_ring6.size(), 5);
  EXPECT_EQ(cell_ring6.length(), 4);

  auto cell_ring7 = cell2.getGeometry(0);
  EXPECT_EQ(cell_ring7.size(), 5);
  EXPECT_EQ(cell_ring7.length(), 8);
  auto cell_ring8 = cell2.getGeometry(1);
  EXPECT_EQ(cell_ring8.size(), 5);
  EXPECT_EQ(cell_ring8.length(), 4);

  F2CLinearRing error_ring;
  EXPECT_THROW(cell1.getGeometry(100, error_ring), std::out_of_range);
  EXPECT_THROW(cell2.getGeometry(100, error_ring), std::out_of_range);
  EXPECT_THROW(cell1.getGeometry(100), std::out_of_range);
  EXPECT_THROW(cell2.getGeometry(100), std::out_of_range);
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

  auto line2 = cell.createLineUntilBorder(F2CPoint(-1, 1), 0);
  EXPECT_EQ(line2.size(), 2);
  EXPECT_EQ(line2.getX(0), -1);
  EXPECT_EQ(line2.getY(0), 1);
  EXPECT_EQ(line2.getX(1), 0);
  EXPECT_EQ(line2.getY(1), 1);
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

TEST(fields2cover_types_cell, isPointIn) {
  F2CCell cell;
  F2CLinearRing ring{
    F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)};
  cell.addRing(ring);

  EXPECT_TRUE(cell.isPointIn(F2CPoint(1,1)));
  EXPECT_FALSE(cell.isPointIn(F2CPoint(2,2)));
  EXPECT_FALSE(cell.isPointIn(F2CPoint(-1,1)));
  EXPECT_FALSE(cell.isPointIn(F2CPoint(10,-10)));
}

TEST(fields2cover_types_cell, convexHull) {
  F2CLinearRing ring {
    F2CPoint(0,0), F2CPoint(1,0),F2CPoint(1,1),F2CPoint(0.5, 0.5), F2CPoint(0,1), F2CPoint(0,0)};
  F2CCell cell(ring);
  EXPECT_EQ(cell.area(), 0.75);
  EXPECT_EQ(cell.convexHull().area(), 1.0);
}


TEST(fields2cover_types_cell, buffer) {
  F2CLinearRing ring {
    F2CPoint(0,0), F2CPoint(1,0),F2CPoint(1,1),F2CPoint(0,1), F2CPoint(0,0)};
  F2CCell cell(ring);
  EXPECT_EQ(cell.area(), 1);

  auto ring_buffer = F2CCell::buffer(ring, 0.01);
  EXPECT_NEAR(ring_buffer.area(), 0.08, 1e-5);

  auto cell_buffer = F2CCell::buffer(cell, 2.0);
  EXPECT_EQ(cell.area(), 1);
  EXPECT_NEAR(cell_buffer.area(), 5*5, 1e-2);

  F2CPoint p {1, 2};
  auto point_buffer = F2CCell::buffer(p, 3.0);
  EXPECT_NEAR(point_buffer.area(), 9*3.141593, 1e-1);
  EXPECT_EQ(p.getX(), 1);
  EXPECT_EQ(p.getY(), 2);

  F2CLineString line {F2CPoint(3,4), F2CPoint(3,6)};
  auto line_buffer = F2CCell::buffer(line, 4.0);
  EXPECT_NEAR(line_buffer.area(), 2 * 2 * 4, 1e-1);
}

TEST(fields2cover_types_sstr, NumberStringConcat) {
  auto result = f2c::types::sstr("word", 10, "another word", 20, 30);
  EXPECT_EQ(result, "word10another word2030");

  std::stringstream ss;
  ss << std::hex << 10 << f2c::types::sstr(10, 0xA, "10", "a");
  result = ss.str();
  EXPECT_EQ(result, "a101010a");

  result = f2c::types::sstr(std::hex, 10, 10, std::dec, 10, 10);
  EXPECT_EQ(result, "aa1010");
}



