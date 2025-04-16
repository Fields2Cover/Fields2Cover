//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover.h"

TEST(fields2cover_types_linearring, init) {
  F2CLinearRing ring1;
  EXPECT_EQ(ring1.size(), 0);

  ring1.addPoint(1,1);
  ring1.addPoint(2,1);
  EXPECT_EQ(ring1.size(), 2);

  F2CLinearRing ring2{F2CPoint(1,2), F2CPoint(3,2), F2CPoint(1,3)};
  EXPECT_EQ(ring2.size(), 3);


  F2CPoint error_p;
  const F2CLinearRing c_line = ring1.clone();
  EXPECT_THROW(ring1.getGeometry(100, error_p), std::out_of_range);
  EXPECT_THROW(c_line.getGeometry(100, error_p), std::out_of_range);
  EXPECT_THROW(ring1.getGeometry(100), std::out_of_range);
  EXPECT_THROW(c_line.getGeometry(100), std::out_of_range);
}

TEST(fields2cover_types_linearring, area) {
  std::vector<F2CPoint> ps{F2CPoint(-1,2), F2CPoint(3,2), F2CPoint(-1,3), F2CPoint(-1,2)};
  F2CLinearRing ring{ps};
  EXPECT_EQ(ring.area(), 2);
}

TEST(fields2cover_types_linearring, mult_equal) {
  std::vector<F2CPoint> ps{F2CPoint(1,2), F2CPoint(3,2), F2CPoint(1,3)};
  F2CLinearRing ring{ps};
  ring *= 10.0;
  EXPECT_EQ(ring.size(), 3);
  EXPECT_EQ(ring.getGeometry(0).getX(), 10);
  EXPECT_EQ(ring.getGeometry(1).getX(), 30);
  EXPECT_EQ(ring.getGeometry(2).getX(), 10);
  EXPECT_EQ(ring.getY(0), 20);
  EXPECT_EQ(ring.getY(1), 20);
  EXPECT_EQ(ring.getY(2), 30);
  EXPECT_EQ(ring.getZ(2), 0);
  ring *= -0.01;
  auto p = ring.endPoint();
  EXPECT_EQ(p.getX(), -0.1);
  EXPECT_EQ(p.getY(), -0.3);
  EXPECT_EQ(p.getZ(), 0);
  const auto ring2 = ring.clone();
  auto c_p = ring2.getGeometry(1);
  EXPECT_EQ(c_p.getX(), -0.3);
  EXPECT_EQ(c_p.getY(), -0.2);
}


TEST(fields2cover_types_linearring, loop) {
  F2CLinearRing line(std::vector<F2CPoint>({F2CPoint(1,2), F2CPoint(3,4), F2CPoint(5,6)}));
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
}

TEST(fields2cover_types_linearring, getLongestEdge) {
  F2CLinearRing ring({F2CPoint( 0,   0.0),
                      F2CPoint(10,  -0.1),
                      F2CPoint(20,   0.0),
                      F2CPoint(17,   5.0),
                      F2CPoint( 3,   5.0),
                      F2CPoint(-0.1, 0.0),
                      F2CPoint( 0,   0.0)});

  EXPECT_EQ(ring.size(), 7);

  auto edge_0 = ring.getLongestEdge();
  EXPECT_EQ(edge_0.size(), 2);
  EXPECT_NEAR(edge_0.length(), 14.0, 1e-1);
  EXPECT_EQ(edge_0[0], ring[3]);
  EXPECT_EQ(edge_0[1], ring[4]);

  auto edge_1 = ring.getLongestEdge(5.0 * M_PI / 180.0);
  EXPECT_EQ(edge_1.size(), 4);
  EXPECT_NEAR(edge_1.length(), 20.1, 1e-1);
  EXPECT_EQ(edge_1[0], ring[5]);
  EXPECT_EQ(edge_1[1], ring[0]);
  EXPECT_EQ(edge_1[2], ring[1]);
  EXPECT_EQ(edge_1[3], ring[2]);
}



TEST(fields2cover_types_linearring, closestPointTo) {
  F2CLinearRing ring({F2CPoint( 1, 1),
                      F2CPoint( 1, 5),
                      F2CPoint( 5, 5),
                      F2CPoint( 5, 1),
                      F2CPoint( 1, 1)});
  EXPECT_EQ(ring.closestPointTo(F2CPoint(2.5, 0)), F2CPoint(2.5, 1));
  EXPECT_EQ(ring.closestPointTo(F2CPoint(3.5, 4)), F2CPoint(3.5, 5));
  EXPECT_EQ(ring.closestPointTo(F2CPoint(  0, 0)), F2CPoint(  1, 1));
}

TEST(fields2cover_types_linearring, segmentLines) {
  F2CLinearRing ring({F2CPoint( 1, 1),
                      F2CPoint( 1, 5),
                      F2CPoint( 5, 5),
                      F2CPoint( 5, 1),
                      F2CPoint( 1, 1)});
  F2CLineString line = ring.getLastSegment();
  EXPECT_EQ(line.size(), 2);
  EXPECT_EQ(ring.segmentLength(3), line.length());
}

TEST(fields2cover_types_linearring, toLineString) {
  F2CLinearRing ring({F2CPoint( 1, 1),
                      F2CPoint( 1, 5),
                      F2CPoint( 5, 5),
                      F2CPoint( 5, 1),
                      F2CPoint( 1, 1)});
  EXPECT_EQ(ring.toLineString().length(), ring.length());
}

TEST(fields2cover_types_linearring, bufferInwards) {
  F2CLinearRing ring({F2CPoint( 1, 1),
                      F2CPoint( 1, 5),
                      F2CPoint( 5, 5),
                      F2CPoint( 5, 1),
                      F2CPoint( 1, 1)});
  F2CLinearRing rev_ring {ring};
  EXPECT_EQ(ring.isClockwise(), rev_ring.isClockwise());

  EXPECT_THROW(ring.bufferInwards({1, 2}), std::invalid_argument);
  auto b_ring = ring;
  auto b_neg_ring = ring;
  auto bv_ring = rev_ring;
  b_ring.bufferInwards({1, 1, 1, 1});
  bv_ring.bufferInwards({1, 1, 1, 1});
  EXPECT_EQ(b_ring, bv_ring);
  EXPECT_NEAR(b_ring.area(), 4, 0.1);

  b_neg_ring.bufferInwards({-1, -1, -1, -1});
  EXPECT_NEAR(b_neg_ring.area(), 36, 0.1);

  auto b_line = b_neg_ring.getParallelLine(1);
  EXPECT_NEAR(b_neg_ring.area(), 36, 0.1);
  EXPECT_NEAR(b_line.area(), 64, 0.1);
}

