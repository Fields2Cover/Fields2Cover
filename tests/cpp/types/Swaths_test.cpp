//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <random>
#include "fields2cover/types.h"

std::vector<F2CLinearRing> create_polygon_border(void) {
  std::vector<F2CLinearRing> v_line;
  F2CLinearRing line;
  line.addPoint(-10, 0);
  line.addPoint(-10, 20);
  line.addPoint(10, 20);
  line.addPoint(10, 0);
  line.addPoint(-10, 0);
  v_line.emplace_back(line);
  return v_line;
}

TEST(fields2cover_types_swaths, append) {
  auto rings = create_polygon_border();
  F2CCells polys;
  F2CCell polygon;
  polygon.addRing(rings[0]);
  polys.addGeometry(polygon);
  F2CLineString line;
  line.addPoint(-20, -10);
  line.addPoint(-5, 5);

  F2CSwaths swaths;
  EXPECT_EQ(swaths.size(), 0);
  swaths.append(line, polys, 1.0);
  EXPECT_EQ(swaths.size(), 1);
  swaths.append(line, polys, 2.0);
  EXPECT_EQ(swaths.size(), 2);
  line.addPoint(-19.9, -10);
  line.addPoint(-5, 5.1);
  swaths.append(line, polys, 3.0);
  EXPECT_EQ(swaths.size(), 4);
  F2CMultiLineString lines;
  lines.addGeometry(line);
  lines.addGeometry(F2CLineString({F2CPoint(-10,0),F2CPoint(10,20)}));
  swaths.append(lines, polygon, 4);
  EXPECT_EQ(swaths.size(), 8);
  swaths.append(lines, polys, 5);
  EXPECT_EQ(swaths.size(), 12);
  lines.addGeometry(F2CLineString({F2CPoint(1e5, 1e5),F2CPoint(1e5+1, 1e5+1)}));
  swaths.append(lines, polys, 6);
  EXPECT_EQ(swaths.size(), 16);
}


TEST(fields2cover_types_swaths, sort) {
  const int n = 5;
  F2CSwaths swaths;
  for (int i = n - 1; i >= 1; --i) {
    swaths.emplace_back(F2CLineString({F2CPoint(i, i), F2CPoint(i, i + 1)}), i, i);
  }
  auto rng = std::default_random_engine {};
  std::shuffle(swaths.begin(), swaths.end(), rng);

  swaths.sort();
  for (int i = 1; i < n; ++i) {
    EXPECT_EQ(swaths[i-1].getWidth(), i);
  }
}


TEST(fields2cover_types_swaths, reverseDirOddSwaths) {
  const int n = 10;
  F2CSwaths swaths;
  for (int i = 1; i <= n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(i, i), F2CPoint(i, i + 1)}), i);
  }
  swaths.reverseDirOddSwaths();
  for (int i = 1; i < n; ++i) {
    EXPECT_FALSE(swaths[i].hasSameDir(swaths[i-1]));
  }
}


TEST(fields2cover_types_swaths, push_back) {
  const int n = 10;
  F2CSwaths swaths1;
  std::vector<F2CSwath> v_swath;
  for (int i = 1; i <= n; ++i) {
    F2CSwath swath {F2CLineString({F2CPoint(i, i), F2CPoint(i, i + 1)}), 1.0*i, i};
    swaths1.push_back(swath);
    v_swath.push_back(swath);
  }

  F2CSwaths swaths2{v_swath};

  for (int i = 1; i < n; ++i) {
    EXPECT_EQ(swaths1.at(i).getId(), i+1);
    EXPECT_EQ(swaths2.at(i).getId(), i+1);
  }
}


TEST(fields2cover_types_swaths, move_to) {
  const int n = 10;
  F2CSwaths swaths;
  for (int i = 1; i <= n; ++i) {
    F2CSwath swath {F2CLineString({F2CPoint(i, i), F2CPoint(i, i + 1)}), 1.0*i, i};
    swaths.push_back(swath);
  }

  F2CSwaths init_swaths = swaths.clone();
  F2CPoint pt(1.0, 2.0);
  swaths.moveTo(pt);

  for (int i = 1; i < n; ++i) {
    EXPECT_EQ(swaths.at(i).startPoint(), init_swaths.at(i).startPoint() + pt);
  }
}



