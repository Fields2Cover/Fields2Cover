//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
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


