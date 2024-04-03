//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_geometries, constructor) {
  const int N = 10;
  F2CLineString line;
  for (int i =0; i < N; ++i) {
    line.addPoint(i,i);
  }

  F2CLineString line2 (line.get());

  for (int i =0; i < N; ++i) {
    EXPECT_EQ(line2.getX(i), i);
    EXPECT_EQ(line2.getY(i), i);
  }
  EXPECT_EQ(line2.size(), N);
}

TEST(fields2cover_types_geometries, access_iterators) {
  const int N = 10;
  F2CLineString line;

  for (int i =0; i < N; ++i) {
    line.addPoint(i,i);
  }
  for (int i =0; i < N; ++i) {
    EXPECT_EQ(line.getX(i), i);
    EXPECT_EQ(line.getY(i), i);
  }
  EXPECT_EQ(line.size(), N);

  int i = 0;
  for (auto p : line) {
    EXPECT_EQ(p.getX(), i);
    EXPECT_EQ(p.getY(), i);
    ++i;
  }

  i = 0;
  for (auto&& p : line) {
    EXPECT_EQ(p.getX(), i);
    EXPECT_EQ(p.getY(), i);
    ++i;
  }

  i = 0;
  for (auto& p : line) {
    EXPECT_EQ(p.getX(), i);
    EXPECT_EQ(p.getY(), i);
    ++i;
  }

  i = 0;
  for (const auto& p : line) {
    EXPECT_EQ(p.getX(), i);
    EXPECT_EQ(p.getY(), i);
    ++i;
  }
}


TEST(fields2cover_types_geometries, mult_iterators) {
  const int N = 2;
  F2CLinearRing line;

  for (int i =0; i < N; ++i) {
    line.addPoint(i,i);
  }
  for (int i =0; i < N; ++i) {
    EXPECT_EQ(line.getX(i), i);
    EXPECT_EQ(line.getY(i), i);
  }
  EXPECT_EQ(line.size(), N);

  int i = 0;
  for (auto&& p : line) {
    EXPECT_EQ(p.getX(), i);
    EXPECT_EQ(p.getY(), i);
    p *= 10.0;
    EXPECT_EQ(p.getX(), 1e1 * i);
    EXPECT_EQ(p.getY(), 1e1 * i);
    ++i;
  }

  i = 0;
  for (auto&& p : line) {
    EXPECT_EQ(p.getX(), 1e1 * i);
    EXPECT_EQ(p.getY(), 1e1 * i);
    ++i;
  }
  i = 0;
  for (auto& p : line) {
    EXPECT_EQ(p.getX(), 1e1 * i);
    EXPECT_EQ(p.getY(), 1e1 * i);
    p *= 10.0;
    EXPECT_EQ(p.getX(), 1e2 * i);
    EXPECT_EQ(p.getY(), 1e2 * i);
    ++i;
  }

  i = 0;
  for (auto&& p : line) {
    EXPECT_EQ(p.getX(), 1e2 * i);
    EXPECT_EQ(p.getY(), 1e2 * i);
    ++i;
  }
}

TEST(fields2cover_types_geometries, simplify) {
  F2CLinearRing ring1{
    F2CPoint(0,0), F2CPoint(1,0),F2CPoint(1, 1),F2CPoint(0, 1), F2CPoint(0, 0)};
  F2CLineString line {ring1};
  F2CMultiLineString lines {line};
  F2CCell cell {ring1};
  F2CCells cells {cell};
  EXPECT_NEAR(line.length(), line.simplify(1e-2).length(), 0.1);
  auto s_lines = lines.simplify(1e-2);
  EXPECT_NEAR(lines[0].length(), s_lines.getGeometry(0).length(), 0.1);
  EXPECT_NEAR(cell.area(), cell.simplify(1e-2).area(), 0.1);
  EXPECT_NEAR(cells.area(), cells.simplify(1e-2).area(), 0.1);
}

TEST(fields2cover_types_geometries, area) {
  F2CMultiPoint ps{
    F2CPoint(0,0), F2CPoint(1,0),F2CPoint(1, 1),F2CPoint(0, 1), F2CPoint(0, 0)};
  F2CLinearRing ring1{
    F2CPoint(0,0), F2CPoint(1,0),F2CPoint(1, 1),F2CPoint(0, 1), F2CPoint(0, 0)};
  F2CLineString line {ring1};
  F2CMultiLineString lines {line};
  F2CCell cell {ring1};
  F2CCells cells {cell};
  EXPECT_NEAR(ps.area(), 0, 1e-3);
  EXPECT_NEAR(ring1.area(), 1, 1e-3);
  EXPECT_NEAR(line.area(), 1, 1e-3);
  EXPECT_NEAR(lines.area(), 1, 1e-3);
  EXPECT_NEAR(cell.area(), 1, 1e-3);
  EXPECT_NEAR(cells.area(), 1, 1e-3);
  EXPECT_NEAR(cell.clone().area(), 1, 1e-3);
}




