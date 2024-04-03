//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_multilinestring, init) {
  F2CMultiLineString lines;
  for (int i = 0; i < 3; ++i) {
    F2CLineString line;
    for (int j = i; j < 5; ++j) {
      line.addPoint(F2CPoint(i, j, -i * j));
    }
    lines.addGeometry(line);
  }

  int i = 0;
  for (const F2CLineString& l : lines) {
    for (int j = 0; j < 5 - i; ++j) {
      EXPECT_EQ(l.getX(j), i);
      EXPECT_EQ(l.getY(j), j+i);
      EXPECT_EQ(l.getZ(j), -i * (j + i));
    }
    ++i;
  }

  lines *= 2.0;

  i = 0;
  for (auto&& l : lines) {
    for (int j = 0; j < 5-i; ++j) {
      EXPECT_EQ(l.getX(j), 2*i);
      EXPECT_EQ(l.getY(j), 2*(j+i));
      EXPECT_EQ(l.getZ(j), -2*i * (j+i));
    }
    ++i;
  }
  const auto const_lines = lines;
  i = 0;
  for (auto&& l : const_lines) {
    for (int j = 0; j < 5-i; ++j) {
      EXPECT_EQ(l.getX(j), 2*i);
      EXPECT_EQ(l.getY(j), 2*(j+i));
      EXPECT_EQ(l.getZ(j), -2*i * (j+i));
    }
    ++i;
  }
  i = 0;
  for (F2CLineString& l : lines) {
    for (int j = 0; j < 5-i; ++j) {
      EXPECT_EQ(l.getX(j), 2*i);
      EXPECT_EQ(l.getY(j), 2*(j+i));
      EXPECT_EQ(l.getZ(j), -2*i * (j+i));
    }
    ++i;
  }

  EXPECT_EQ(lines.size(), 3);
  F2CMultiLineString lines2 = lines.clone();
  EXPECT_EQ(lines.size(), 3);
  EXPECT_EQ(lines2.size(), 3);

  lines.addGeometry(lines2);
  EXPECT_EQ(lines.size(), 6);
  EXPECT_EQ(lines2.size(), 3);
}

TEST(fields2cover_types_multilinestring, getGeometry) {
  F2CMultiLineString lines;
  for (int i = 0; i < 3; ++i) {
    F2CLineString line;
    for (int j = i; j < 5; ++j) {
      line.addPoint(F2CPoint(0, j));
    }
    lines.addGeometry(line);
  }

  const auto const_lines = lines.clone();
  for (int i = 0; i < lines.size(); ++i) {
    EXPECT_EQ(lines.getGeometry(i).size(), 5-i);
    EXPECT_EQ(lines.getGeometry(i).length(), 5-i-1);
    EXPECT_EQ(const_lines.getGeometry(i).size(), 5-i);
    EXPECT_EQ(const_lines.getGeometry(i).length(), 5-i-1);
  }


  F2CLineString l_error;
  EXPECT_THROW(lines.getGeometry(30, l_error), std::out_of_range);
  EXPECT_THROW(const_lines.getGeometry(30, l_error), std::out_of_range);
  EXPECT_THROW(lines.getGeometry(30), std::out_of_range);
  EXPECT_THROW(const_lines.getGeometry(30), std::out_of_range);
}

TEST(fields2cover_types_multilinestring, append) {
  F2CLinearRing ring1 {
    F2CPoint(0,0), F2CPoint(1,0), F2CPoint(1,1), F2CPoint(0,1), F2CPoint(0,0)};
  F2CLinearRing ring2 = ring1.clone();
  ring2 *= 2.0;
  ring1 = ring1 + F2CPoint(0.5, 0.5);
  F2CCell cell {ring2};
  cell.addRing(ring1);

  F2CMultiLineString lines;
  lines.append(cell.get());
  EXPECT_EQ(lines.size(), 2);
  lines.append(F2CCells(cell).get());
  EXPECT_EQ(lines.size(), 4);
}

TEST(fields2cover_types_multilinestring, setGeometry) {
  F2CLineString line {
    F2CPoint(0,0), F2CPoint(2,0), F2CPoint(2,2), F2CPoint(0,2)};
  F2CLineString line2 {
    F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,4), F2CPoint(0,4)};
  F2CMultiLineString lines;
  EXPECT_EQ(lines.length(), 0);
  EXPECT_EQ(lines.size(), 0);

  lines.setGeometry(0, line2);
  EXPECT_EQ(lines.length(), 12);
  EXPECT_EQ(lines.size(), 1);

  lines.setGeometry(0, line);
  EXPECT_EQ(lines.length(), 6);
  EXPECT_EQ(lines.size(), 1);

  lines.setGeometry(1, F2CLineString());
  EXPECT_EQ(lines.length(), 6);
  EXPECT_EQ(lines.size(), 2);

  lines.setGeometry(10, line);
  EXPECT_EQ(lines.length(), 12);
  EXPECT_EQ(lines.size(), 11);

  lines.setGeometry(0, line2);
  EXPECT_EQ(lines.length(), 18);
  EXPECT_EQ(lines.size(), 11);
}

TEST(fields2cover_types_multilinestring, getLineSegments) {
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(1,0), F2CPoint(1,1), F2CPoint(0,1), F2CPoint(0,0)};
  F2CMultiLineString lines = F2CMultiLineString::getLineSegments(ring1);
  EXPECT_EQ(lines.size(), 4);
  EXPECT_EQ(lines.size(), ring1.size() - 1);
  EXPECT_NEAR(lines.length(), 4.0, 1e-7);
}

