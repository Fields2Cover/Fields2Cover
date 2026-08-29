//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

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



TEST(fields2cover_types_linearring, segments) {
  F2CLinearRing ring{F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(0,0)};

  EXPECT_EQ(ring.getSegment(0), F2CLineString(F2CPoint(0,0), F2CPoint(4,0)));
  EXPECT_EQ(ring.getLastSegment(), F2CLineString(F2CPoint(4,3), F2CPoint(0,0)));
  EXPECT_NEAR(ring.segmentLength(0), 4, 1e-7);
  EXPECT_NEAR(ring.segmentLength(1), 3, 1e-7);
  EXPECT_NEAR(ring.segmentLength(2), 5, 1e-7);
  EXPECT_NEAR(ring.segmentAng(0), 0, 1e-7);
  EXPECT_NEAR(ring.segmentAng(1), 0.5 * M_PI, 1e-7);
  EXPECT_NEAR(ring.segmentAng(2), M_PI + atan2(3, 4), 1e-7);
}

TEST(fields2cover_types_linearring, removePoint) {
  F2CLinearRing ring{F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(0,0)};

  EXPECT_EQ(ring.removePoint(1).size(), 3);
  EXPECT_EQ(ring.getGeometry(1), F2CPoint(4,3));
}

TEST(fields2cover_types_linearring, getParallelLine) {
  F2CLinearRing ring{F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,4),
                     F2CPoint(0,4), F2CPoint(0,0)};

  EXPECT_THROW(ring.getParallelLine({1, 1}), std::invalid_argument);
  // On a counterclockwise ring, a positive offset goes inwards.
  EXPECT_NEAR(ring.getParallelLine(1).area(), 4, 1e-7);
  EXPECT_NEAR(ring.getParallelLine(-1).area(), 36, 1e-7);
  EXPECT_NEAR(ring.getParallelLine(0).area(), ring.area(), 1e-7);
}

TEST(fields2cover_types_linearring, getParallelLinePerSegment) {
  F2CLinearRing ring{F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,4),
                     F2CPoint(0,4), F2CPoint(0,0)};

  // Only the bottom edge moves up, so a 4x4 square becomes a 4x3 rectangle.
  EXPECT_NEAR(ring.getParallelLine({1, 0, 0, 0}).area(), 12, 1e-7);
  EXPECT_NEAR(ring.getParallelLine({1, 1, 0, 0}).area(), 9, 1e-7);
}

TEST(fields2cover_types_linearring, bufferOutwardsAndInwards) {
  F2CLinearRing ring{F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,4),
                     F2CPoint(0,4), F2CPoint(0,0)};
  // clone(), not a copy: copies share the underlying OGR geometry.
  F2CLinearRing rev_ring {ring.clone()};
  rev_ring.reversePoints();
  EXPECT_NE(ring.isClockwise(), rev_ring.isClockwise());

  EXPECT_THROW(ring.bufferInwards({1, 1}), std::invalid_argument);
  EXPECT_THROW(ring.bufferOutwards({1, 1}), std::invalid_argument);

  // The direction does not depend on how the ring is wound.
  auto in_ring = ring.clone(), in_rev_ring = rev_ring.clone();
  EXPECT_NEAR(in_ring.bufferInwards({1, 1, 1, 1}).area(), 4, 1e-7);
  EXPECT_NEAR(in_rev_ring.bufferInwards({1, 1, 1, 1}).area(), 4, 1e-7);

  auto out_ring = ring.clone(), out_rev_ring = rev_ring.clone();
  EXPECT_NEAR(out_ring.bufferOutwards({1, 1, 1, 1}).area(), 36, 1e-7);
  EXPECT_NEAR(out_rev_ring.bufferOutwards({1, 1, 1, 1}).area(), 36, 1e-7);

  // A negative offset is the other buffer.
  auto neg_ring = ring.clone();
  EXPECT_NEAR(neg_ring.bufferInwards({-1, -1, -1, -1}).area(), 36, 1e-7);
}

TEST(fields2cover_types_linearring, filterSelfIntersections) {
  F2CLinearRing square{F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,4),
                       F2CPoint(0,4), F2CPoint(0,0)};
  auto filtered_square = square.clone();
  EXPECT_EQ(filtered_square.filterSelfIntersections(), square);

  F2CLinearRing tiny{F2CPoint(0,0), F2CPoint(4,0), F2CPoint(0,0)};
  auto filtered_tiny = tiny.clone();
  EXPECT_EQ(filtered_tiny.filterSelfIntersections(), tiny);

  // Crossed edges are cut at the crossing point.
  F2CLinearRing bowtie{F2CPoint(0,0), F2CPoint(4,0), F2CPoint(0,4),
                       F2CPoint(4,4), F2CPoint(0,0)};
  bowtie.filterSelfIntersections();
  EXPECT_EQ(bowtie.size(), 4);
  EXPECT_EQ(bowtie.getGeometry(2), F2CPoint(2,2));
  EXPECT_NEAR(bowtie.area(), 4, 1e-7);

  // A spur that doubles back on the previous edge is dropped.
  F2CLinearRing spur{F2CPoint(0,0), F2CPoint(4,0), F2CPoint(2,0),
                     F2CPoint(2,3), F2CPoint(0,3), F2CPoint(0,0)};
  spur.filterSelfIntersections();
  EXPECT_EQ(spur.size(), 5);
  EXPECT_NEAR(spur.area(), 6, 1e-7);
}
