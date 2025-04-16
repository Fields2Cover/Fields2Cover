//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover.h"

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

TEST(fields2cover_types_linestring, removePoint) {
  F2CLineString line({
        F2CPoint(0,0), F2CPoint(2,0), F2CPoint(2,1), F2CPoint(1,1), F2CPoint(1,-1)});
  EXPECT_EQ(line.size(), 5);
  EXPECT_NEAR(line.length(), 6, 1e-3);
  EXPECT_EQ(line[0], F2CPoint(0, 0));
  EXPECT_EQ(line[3], F2CPoint(1, 1));

  line.removePoint(2);

  EXPECT_EQ(line.size(), 4);
  EXPECT_NEAR(line.length(), 4 + sqrt(2), 1e-3);
  EXPECT_EQ(line[0], F2CPoint(0,  0));
  EXPECT_EQ(line[3], F2CPoint(1, -1));

  line.removePoint(10);
  EXPECT_EQ(line.size(), 4);
}

TEST(fields2cover_types_linestring, filterSelfIntersections) {
  F2CLineString line({
        F2CPoint(0,0), F2CPoint(2,0), F2CPoint(2,1), F2CPoint(1,1), F2CPoint(1,-1)});
  EXPECT_EQ(line.size(), 5);
  EXPECT_NEAR(line.length(), 6, 1e-3);

  line.filterSelfIntersections();

  EXPECT_EQ(line.size(), 3);
  EXPECT_NEAR(line.length(), 2, 1e-3);
  EXPECT_EQ(line[0], F2CPoint(0, 0));
  EXPECT_EQ(line[1], F2CPoint(1, 0));
  EXPECT_EQ(line[2], F2CPoint(1, -1));

  line.filterSelfIntersections();
  EXPECT_EQ(line.size(), 3);
  EXPECT_NEAR(line.length(), 2, 1e-3);
}

TEST(fields2cover_types_linestring, getParallelLine) {
  F2CLineString line({
        F2CPoint(0,0), F2CPoint(2,0), F2CPoint(3,1),
        F2CPoint(4,0), F2CPoint(5, 0.1)});
  EXPECT_EQ(line.size(), 5);
  EXPECT_NEAR(line.length(), 5.83, 1e-1);

  auto p_line_p = line.getParallelLine(0.5);
  auto p_line_n = line.getParallelLine(-0.5);
  auto p_line2 = line.getParallelLine({-0.2, 0.3, -0.4, 0.5});

  EXPECT_EQ(p_line_p.size(), 5);
  EXPECT_EQ(p_line_n.size(), 5);
  EXPECT_NEAR(p_line_p.length(), line.length(), 1e-5);
  EXPECT_NEAR(p_line_n.length(), line.length(), 1e-5);

  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(line);
  f2c::Visualizer::plot(p_line_p);
  f2c::Visualizer::plot(p_line_n);
  f2c::Visualizer::plot(p_line2);
  f2c::Visualizer::show();
  */
}

TEST(fields2cover_types_linestring, getCurvedSwaths) {
  f2c::Random rand(420445);
  auto non_convex_field = rand.genNonConvexField(1e3);
  F2CCells cells = non_convex_field.getField();
  F2CLinearRing ring = cells[0][0];

  auto edge = ring.getLongestEdge(0.5);

  F2CSwaths swaths;
  double w = 1.0;
  auto prev_line = edge.getParallelLine(-w * 0.5);
  for (int i = 0; i < 50; ++i) {
    swaths.append(prev_line.getParallelLine(-i*w).extend(2*edge.length()), cells, w);
    //swaths.append(prev_line, cells, w);
    //prev_line = prev_line.getParallelLine(-w);
  }

  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(edge);
  f2c::Visualizer::plot(swaths);
  f2c::Visualizer::show();
  */
}


/*
TEST(fields2cover_types_linestring, moveLine) {
  //F2CLineString line1({F2CPoint(1, 2), F2CPoint(1, 4), F2CPoint(-2, 4)});
  F2CLineString line1({
        F2CPoint(0,0), F2CPoint(2,0), F2CPoint(2,2),
        F2CPoint(0,2), F2CPoint(0,0)});
  f2c::Visualizer::figure();
  for (int i = 1; i<=2; ++i){
    auto mov_line1 = line1.getParallelLine(-1. * i);
    f2c::Visualizer::plot(mov_line1);
    f2c::Visualizer::plot(line1);
  }
  f2c::Visualizer::show();
}
*/

TEST(fields2cover_types_linestring, extend) {
  F2CLineString line({F2CPoint(1, 2), F2CPoint(1, 4), F2CPoint(-2, 4)});
  auto ext_line = line.clone().extend(5);
  EXPECT_EQ(ext_line.size(), line.size());
  EXPECT_EQ(ext_line.length(), 5 * 2 + line.length());
  EXPECT_EQ(ext_line.startPoint(), F2CPoint(1,-3));
  EXPECT_EQ(ext_line.endPoint(), F2CPoint(-7, 4));
}






