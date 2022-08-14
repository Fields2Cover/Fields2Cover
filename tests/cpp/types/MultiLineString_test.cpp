//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
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
  for (const auto& l : lines) {
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
    EXPECT_EQ(lines.getGeometry(i).getLength(), 5-i-1);
    EXPECT_EQ(const_lines.getGeometry(i).size(), 5-i);
    EXPECT_EQ(const_lines.getGeometry(i).getLength(), 5-i-1);
  }
}

TEST(fields2cover_types_multilinestring, append) {
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(1,0), F2CPoint(1,1), F2CPoint(0,1), F2CPoint(0,0)};
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
