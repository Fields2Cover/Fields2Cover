//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_linestring, init) {
  F2CLineString line1;
  line1.addPoint(1,1);
  line1.addPoint(2,1);
  EXPECT_EQ(line1.size(), 2);
  F2CLineString line2({F2CPoint(1,2), F2CPoint(3,2), F2CPoint(1,3)});
  EXPECT_EQ(line2.size(), 3);
  EXPECT_EQ(line2.getX(0), 1);
  EXPECT_EQ(line2.getX(1), 3);
  EXPECT_EQ(line2.X(1), 3);
  EXPECT_EQ(line2.Y(1), 2);
  EXPECT_EQ(line2.Z(1), 0);
  F2CLineString line3{F2CPoint(2,3)};
  EXPECT_EQ(line3.size(), 1);
  EXPECT_EQ(line3.getX(0), 2);
  const F2CPoint p {20, 30};
  F2CLineString line4{p};
  EXPECT_EQ(line4.size(), 1);
  auto p2 = line4.getGeometry(0);
  EXPECT_EQ(p2.getX(), 20);
  EXPECT_EQ(p2.getY(), 30);
  F2CLineString line5(std::vector<F2CPoint>({F2CPoint(1,2), F2CPoint(3,2), F2CPoint(1,3)}));
  EXPECT_EQ(line5.size(), 3);
  EXPECT_EQ(line5.getX(0), 1);
  EXPECT_EQ(line5.getX(1), 3);

  F2CLineString line6(static_cast<OGRGeometry*>(line5.get()), f2c::types::EmptyDestructor());
  EXPECT_EQ(line6.size(), 3);
  EXPECT_EQ(line6.getX(0), 1);
  EXPECT_EQ(line6.getX(1), 3);

  OGRLineString ogr_line = *(line5.get());
  F2CLineString line7(ogr_line);
  EXPECT_EQ(line7.size(), 3);
  EXPECT_EQ(line7.getX(0), 1);
  EXPECT_EQ(line7.getX(1), 3);

  F2CPoint error_p;
  const F2CLineString c_line = line7.clone();
  EXPECT_THROW(line7.getGeometry(100, error_p), std::out_of_range);
  EXPECT_THROW(c_line.getGeometry(100, error_p), std::out_of_range);
  EXPECT_THROW(line7.getGeometry(100), std::out_of_range);
  EXPECT_THROW(c_line.getGeometry(100), std::out_of_range);
}

TEST(fields2cover_types_linestring, loop) {
  F2CLineString line(std::vector<F2CPoint>({F2CPoint(1,2), F2CPoint(3,4), F2CPoint(5,6)}));
  int i = 1;
  for (const F2CPoint& p : line) {
    EXPECT_EQ(p, F2CPoint(i, i+1));
    i += 2;
  }
  i = 1;
  for (F2CPoint& p : line) {
    EXPECT_EQ(p, F2CPoint(i, i+1));
    i += 2;
  }
  i = 1;
  for (auto&& p : line) {
    EXPECT_EQ(p, F2CPoint(i, i+1));
    i += 2;
  }
  i = 1;
  for (auto&& p : line) {
    p *= -1;
    EXPECT_EQ(p, F2CPoint(-i, -i-1));
    i += 2;
  }

  EXPECT_EQ(line.getX(0), -1);
  EXPECT_EQ(line.getY(0), -2);
}


TEST(fields2cover_types_linestring, closestPointTo) {
  F2CLineString line(std::vector<F2CPoint>({F2CPoint(1,2), F2CPoint(3,2), F2CPoint(3,7)}));
  EXPECT_EQ(line.closestPointTo(F2CPoint(0, 0)), F2CPoint(1, 2));
  EXPECT_EQ(line.closestPointTo(F2CPoint(2, -5)), F2CPoint(2, 2));
  EXPECT_EQ(line.closestPointTo(F2CPoint(3, 5)), F2CPoint(3, 5));
  EXPECT_EQ(line.closestPointTo(F2CPoint(5, 4)), F2CPoint(3, 4));
}

TEST(fields2cover_types_linestring, startAngle_and_endAngle) {
  F2CLineString line(std::vector<F2CPoint>({
        F2CPoint(1,-1), F2CPoint(3,1), F2CPoint(4,2), F2CPoint(4,-2)}));
  EXPECT_NEAR(line.startAngle(), 0.25*M_PI, 1e-3);
  EXPECT_NEAR(line.endAngle(), 1.5*M_PI, 1e-3);
}


