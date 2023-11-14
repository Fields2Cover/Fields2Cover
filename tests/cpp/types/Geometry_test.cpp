//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_geometry, getAngContinuity) {
  std::vector<double> v;
  for (double d = -50.0; d < 50.0; d += 0.1) {
    v.push_back(F2CPoint::mod_2pi(d));
    EXPECT_NEAR(F2CPoint::getAngContinuity(d - 0.1, F2CPoint::mod_2pi(d)), d, 1e-5);
  }
  for (double d = 50.0; d > -50.0; d -= 0.1) {
    v.push_back(F2CPoint::mod_2pi(d));
    EXPECT_NEAR(F2CPoint::getAngContinuity(d + 0.1, F2CPoint::mod_2pi(d)), d, 1e-5);
    EXPECT_NEAR(F2CLinearRing::getAngContinuity(d - 0.1, F2CLinearRing::mod_2pi(d)), d, 1e-5);
    EXPECT_NEAR(F2CLineString::getAngContinuity(d + 0.5, F2CLineString::mod_2pi(d)), d, 1e-5);
    EXPECT_NEAR(F2CMultiLineString::getAngContinuity(d - 0.5, F2CMultiLineString::mod_2pi(d)), d, 1e-5);
    EXPECT_NEAR(F2CMultiPoint::getAngContinuity(d + 1.0, F2CMultiPoint::mod_2pi(d)), d, 1e-5);
    EXPECT_NEAR(F2CCell::getAngContinuity(d - 1.0, F2CCell::mod_2pi(d)), d, 1e-5);
    EXPECT_NEAR(F2CCells::getAngContinuity(d + 0.0, F2CCells::mod_2pi(d)), d, 1e-5);
  }
  auto restored_v = F2CPoint::getAngContinuity(v);
  for (int i = 1; i < restored_v.size(); ++i) {
    EXPECT_NEAR(fabs(restored_v[i] - restored_v[i - 1]), 0.1, 1e-5);
  }
}

TEST(fields2cover_types_geometry, intersects) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_FALSE(p_t.Intersects(p_n));
  EXPECT_TRUE(p_t.Intersects(ps));
  EXPECT_TRUE(p_t.Intersects(line));
  EXPECT_TRUE(p_t.Intersects(lines));
  EXPECT_FALSE(p_t.Intersects(ring1));
  EXPECT_TRUE(p_t.Intersects(cell1));
  EXPECT_TRUE(p_t.Intersects(cells1));
  EXPECT_FALSE(p_n.Intersects(ring1));
  EXPECT_FALSE(p_n.Intersects(cell1));
  EXPECT_FALSE(p_n.Intersects(cells1));

  EXPECT_TRUE(ps.Intersects(p_n));
  EXPECT_TRUE(ps.Intersects(ps));
  EXPECT_TRUE(ps.Intersects(line));
  EXPECT_TRUE(ps.Intersects(lines));
  EXPECT_FALSE(ps.Intersects(ring1));
  EXPECT_TRUE(ps.Intersects(cell1));
  EXPECT_TRUE(ps.Intersects(cells1));

  EXPECT_TRUE(line.Intersects(p_t));
  EXPECT_TRUE(line.Intersects(ps));
  EXPECT_TRUE(line.Intersects(line));
  EXPECT_TRUE(line.Intersects(lines));
  EXPECT_FALSE(line.Intersects(ring1));
  EXPECT_TRUE(line.Intersects(cell1));
  EXPECT_TRUE(line.Intersects(cells1));

  EXPECT_TRUE(lines.Intersects(p_t));
  EXPECT_TRUE(lines.Intersects(ps));
  EXPECT_TRUE(lines.Intersects(line));
  EXPECT_TRUE(lines.Intersects(lines));
  EXPECT_FALSE(lines.Intersects(ring1));
  EXPECT_TRUE(lines.Intersects(cell1));
  EXPECT_TRUE(lines.Intersects(cells1));

  EXPECT_FALSE(ring1.Intersects(p_t));
  EXPECT_FALSE(ring1.Intersects(ps));
  EXPECT_FALSE(ring1.Intersects(line));
  EXPECT_FALSE(ring1.Intersects(lines));
  EXPECT_FALSE(ring1.Intersects(ring1));
  EXPECT_FALSE(ring1.Intersects(cell1));
  EXPECT_FALSE(ring1.Intersects(cells1));

  EXPECT_TRUE(cell1.Intersects(p_t));
  EXPECT_TRUE(cell1.Intersects(ps));
  EXPECT_TRUE(cell1.Intersects(line));
  EXPECT_TRUE(cell1.Intersects(lines));
  EXPECT_FALSE(cell1.Intersects(ring1));
  EXPECT_TRUE(cell1.Intersects(cell1));
  EXPECT_TRUE(cell1.Intersects(cells1));

  EXPECT_TRUE(cells1.Intersects(p_t));
  EXPECT_TRUE(cells1.Intersects(ps));
  EXPECT_TRUE(cells1.Intersects(line));
  EXPECT_TRUE(cells1.Intersects(lines));
  EXPECT_FALSE(cells1.Intersects(ring1));
  EXPECT_TRUE(cells1.Intersects(cell1));
  EXPECT_TRUE(cells1.Intersects(cells1));
}

TEST(fields2cover_types_geometry, touches) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_FALSE(p_t.Touches(p_n));
  EXPECT_FALSE(p_t.Touches(ps));
  EXPECT_TRUE(p_t.Touches(line));
  EXPECT_TRUE(p_t.Touches(lines));
  EXPECT_FALSE(p_t.Touches(ring1));
  EXPECT_FALSE(p_t.Touches(cell1));
  EXPECT_FALSE(p_t.Touches(cells1));
  EXPECT_FALSE(p_n.Touches(ring1));
  EXPECT_FALSE(p_n.Touches(cell1));
  EXPECT_FALSE(p_n.Touches(cells1));

  EXPECT_FALSE(ps.Touches(p_n));
  EXPECT_FALSE(ps.Touches(ps));
  EXPECT_TRUE(ps.Touches(line));
  EXPECT_TRUE(ps.Touches(lines));
  EXPECT_FALSE(ps.Touches(ring1));
  EXPECT_FALSE(ps.Touches(cell1));
  EXPECT_FALSE(ps.Touches(cells1));

  EXPECT_TRUE(line.Touches(p_t));
  EXPECT_TRUE(line.Touches(ps));
  EXPECT_FALSE(line.Touches(line));
  EXPECT_FALSE(line.Touches(lines));
  EXPECT_FALSE(line.Touches(ring1));
  EXPECT_FALSE(line.Touches(cell1));
  EXPECT_FALSE(line.Touches(cells1));

  EXPECT_TRUE(lines.Touches(p_t));
  EXPECT_TRUE(lines.Touches(ps));
  EXPECT_FALSE(lines.Touches(line));
  EXPECT_FALSE(lines.Touches(lines));
  EXPECT_FALSE(lines.Touches(ring1));
  EXPECT_FALSE(lines.Touches(cell1));
  EXPECT_FALSE(lines.Touches(cells1));

  EXPECT_FALSE(ring1.Touches(p_t));
  EXPECT_FALSE(ring1.Touches(ps));
  EXPECT_FALSE(ring1.Touches(line));
  EXPECT_FALSE(ring1.Touches(lines));
  EXPECT_FALSE(ring1.Touches(ring1));
  EXPECT_FALSE(ring1.Touches(cell1));
  EXPECT_FALSE(ring1.Touches(cells1));

  EXPECT_FALSE(cell1.Touches(p_t));
  EXPECT_FALSE(cell1.Touches(ps));
  EXPECT_FALSE(cell1.Touches(line));
  EXPECT_FALSE(cell1.Touches(lines));
  EXPECT_FALSE(cell1.Touches(ring1));
  EXPECT_FALSE(cell1.Touches(cell1));
  EXPECT_FALSE(cell1.Touches(cells1));

  EXPECT_FALSE(cells1.Touches(p_t));
  EXPECT_FALSE(cells1.Touches(ps));
  EXPECT_FALSE(cells1.Touches(line));
  EXPECT_FALSE(cells1.Touches(lines));
  EXPECT_FALSE(cells1.Touches(ring1));
  EXPECT_FALSE(cells1.Touches(cell1));
  EXPECT_FALSE(cells1.Touches(cells1));
}

TEST(fields2cover_types_geometry, disjoint) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};



  EXPECT_TRUE(p_t.Disjoint(p_n));
  EXPECT_FALSE(p_t.Disjoint(ps));
  EXPECT_FALSE(p_t.Disjoint(line));
  EXPECT_FALSE(p_t.Disjoint(lines));
  EXPECT_FALSE(p_t.Disjoint(ring1));
  EXPECT_FALSE(p_t.Disjoint(cell1));
  EXPECT_FALSE(p_t.Disjoint(cells1));
  EXPECT_FALSE(p_n.Disjoint(ring1));
  EXPECT_TRUE(p_n.Disjoint(cell1));
  EXPECT_TRUE(p_n.Disjoint(cells1));

  EXPECT_FALSE(ps.Disjoint(p_n));
  EXPECT_FALSE(ps.Disjoint(ps));
  EXPECT_FALSE(ps.Disjoint(line));
  EXPECT_FALSE(ps.Disjoint(lines));
  EXPECT_FALSE(ps.Disjoint(ring1));
  EXPECT_FALSE(ps.Disjoint(cell1));
  EXPECT_FALSE(ps.Disjoint(cells1));

  EXPECT_FALSE(line.Disjoint(p_t));
  EXPECT_FALSE(line.Disjoint(ps));
  EXPECT_FALSE(line.Disjoint(line));
  EXPECT_FALSE(line.Disjoint(lines));
  EXPECT_FALSE(line.Disjoint(ring1));
  EXPECT_FALSE(line.Disjoint(cell1));
  EXPECT_FALSE(line.Disjoint(cells1));

  EXPECT_FALSE(lines.Disjoint(p_t));
  EXPECT_FALSE(lines.Disjoint(ps));
  EXPECT_FALSE(lines.Disjoint(line));
  EXPECT_FALSE(lines.Disjoint(lines));
  EXPECT_FALSE(lines.Disjoint(ring1));
  EXPECT_FALSE(lines.Disjoint(cell1));
  EXPECT_FALSE(lines.Disjoint(cells1));

  EXPECT_FALSE(ring1.Disjoint(p_t));
  EXPECT_FALSE(ring1.Disjoint(ps));
  EXPECT_FALSE(ring1.Disjoint(line));
  EXPECT_FALSE(ring1.Disjoint(lines));
  EXPECT_FALSE(ring1.Disjoint(ring1));
  EXPECT_FALSE(ring1.Disjoint(cell1));
  EXPECT_FALSE(ring1.Disjoint(cells1));

  EXPECT_FALSE(cell1.Disjoint(p_t));
  EXPECT_FALSE(cell1.Disjoint(ps));
  EXPECT_FALSE(cell1.Disjoint(line));
  EXPECT_FALSE(cell1.Disjoint(lines));
  EXPECT_FALSE(cell1.Disjoint(ring1));
  EXPECT_FALSE(cell1.Disjoint(cell1));
  EXPECT_FALSE(cell1.Disjoint(cells1));

  EXPECT_FALSE(cells1.Disjoint(p_t));
  EXPECT_FALSE(cells1.Disjoint(ps));
  EXPECT_FALSE(cells1.Disjoint(line));
  EXPECT_FALSE(cells1.Disjoint(lines));
  EXPECT_FALSE(cells1.Disjoint(ring1));
  EXPECT_FALSE(cells1.Disjoint(cell1));
  EXPECT_FALSE(cells1.Disjoint(cells1));
}

TEST(fields2cover_types_geometry, within) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};


  EXPECT_FALSE(p_t.Within(p_n));
  EXPECT_TRUE(p_t.Within(ps));
  EXPECT_FALSE(p_t.Within(line));
  EXPECT_FALSE(p_t.Within(lines));
  EXPECT_FALSE(p_t.Within(ring1));
  EXPECT_TRUE(p_t.Within(cell1));
  EXPECT_TRUE(p_t.Within(cells1));
  EXPECT_FALSE(p_n.Within(ring1));
  EXPECT_FALSE(p_n.Within(cell1));
  EXPECT_FALSE(p_n.Within(cells1));

  EXPECT_FALSE(ps.Within(p_n));
  EXPECT_TRUE(ps.Within(ps));
  EXPECT_FALSE(ps.Within(line));
  EXPECT_FALSE(ps.Within(lines));
  EXPECT_FALSE(ps.Within(ring1));
  EXPECT_FALSE(ps.Within(cell1));
  EXPECT_FALSE(ps.Within(cells1));

  EXPECT_FALSE(line.Within(p_t));
  EXPECT_FALSE(line.Within(ps));
  EXPECT_TRUE(line.Within(line));
  EXPECT_TRUE(line.Within(lines));
  EXPECT_FALSE(line.Within(ring1));
  EXPECT_FALSE(line.Within(cell1));
  EXPECT_FALSE(line.Within(cells1));

  EXPECT_FALSE(lines.Within(p_t));
  EXPECT_FALSE(lines.Within(ps));
  EXPECT_TRUE(lines.Within(line));
  EXPECT_TRUE(lines.Within(lines));
  EXPECT_FALSE(lines.Within(ring1));
  EXPECT_FALSE(lines.Within(cell1));
  EXPECT_FALSE(lines.Within(cells1));

  EXPECT_FALSE(ring1.Within(p_t));
  EXPECT_FALSE(ring1.Within(ps));
  EXPECT_FALSE(ring1.Within(line));
  EXPECT_FALSE(ring1.Within(lines));
  EXPECT_FALSE(ring1.Within(ring1));
  EXPECT_FALSE(ring1.Within(cell1));
  EXPECT_FALSE(ring1.Within(cells1));

  EXPECT_FALSE(cell1.Within(p_t));
  EXPECT_FALSE(cell1.Within(ps));
  EXPECT_FALSE(cell1.Within(line));
  EXPECT_FALSE(cell1.Within(lines));
  EXPECT_FALSE(cell1.Within(ring1));
  EXPECT_TRUE(cell1.Within(cell1));
  EXPECT_TRUE(cell1.Within(cells1));

  EXPECT_FALSE(cells1.Within(p_t));
  EXPECT_FALSE(cells1.Within(ps));
  EXPECT_FALSE(cells1.Within(line));
  EXPECT_FALSE(cells1.Within(lines));
  EXPECT_FALSE(cells1.Within(ring1));
  EXPECT_TRUE(cells1.Within(cell1));
  EXPECT_TRUE(cells1.Within(cells1));
}

TEST(fields2cover_types_geometry, crosses) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};


  EXPECT_FALSE(p_t.Crosses(p_n));
  EXPECT_FALSE(p_t.Crosses(ps));
  EXPECT_FALSE(p_t.Crosses(line));
  EXPECT_FALSE(p_t.Crosses(lines));
  EXPECT_FALSE(p_t.Crosses(ring1));
  EXPECT_FALSE(p_t.Crosses(cell1));
  EXPECT_FALSE(p_t.Crosses(cells1));
  EXPECT_FALSE(p_n.Crosses(ring1));
  EXPECT_FALSE(p_n.Crosses(cell1));
  EXPECT_FALSE(p_n.Crosses(cells1));

  EXPECT_FALSE(ps.Crosses(p_n));
  EXPECT_FALSE(ps.Crosses(ps));
  EXPECT_FALSE(ps.Crosses(line));
  EXPECT_FALSE(ps.Crosses(lines));
  EXPECT_FALSE(ps.Crosses(ring1));
  EXPECT_TRUE(ps.Crosses(cell1));
  EXPECT_TRUE(ps.Crosses(cells1));

  EXPECT_FALSE(line.Crosses(p_t));
  EXPECT_FALSE(line.Crosses(ps));
  EXPECT_FALSE(line.Crosses(line));
  EXPECT_FALSE(line.Crosses(lines));
  EXPECT_FALSE(line.Crosses(ring1));
  EXPECT_TRUE(line.Crosses(cell1));
  EXPECT_TRUE(line.Crosses(cells1));

  EXPECT_FALSE(lines.Crosses(p_t));
  EXPECT_FALSE(lines.Crosses(ps));
  EXPECT_FALSE(lines.Crosses(line));
  EXPECT_FALSE(lines.Crosses(lines));
  EXPECT_FALSE(lines.Crosses(ring1));
  EXPECT_TRUE(lines.Crosses(cell1));
  EXPECT_TRUE(lines.Crosses(cells1));

  EXPECT_FALSE(ring1.Crosses(p_t));
  EXPECT_FALSE(ring1.Crosses(ps));
  EXPECT_FALSE(ring1.Crosses(line));
  EXPECT_FALSE(ring1.Crosses(lines));
  EXPECT_FALSE(ring1.Crosses(ring1));
  EXPECT_FALSE(ring1.Crosses(cell1));
  EXPECT_FALSE(ring1.Crosses(cells1));

  EXPECT_FALSE(cell1.Crosses(p_t));
  EXPECT_TRUE(cell1.Crosses(ps));
  EXPECT_TRUE(cell1.Crosses(line));
  EXPECT_TRUE(cell1.Crosses(lines));
  EXPECT_FALSE(cell1.Crosses(ring1));
  EXPECT_FALSE(cell1.Crosses(cell1));
  EXPECT_FALSE(cell1.Crosses(cells1));

  EXPECT_FALSE(cells1.Crosses(p_t));
  EXPECT_TRUE(cells1.Crosses(ps));
  EXPECT_TRUE(cells1.Crosses(line));
  EXPECT_TRUE(cells1.Crosses(lines));
  EXPECT_FALSE(cells1.Crosses(ring1));
  EXPECT_FALSE(cells1.Crosses(cell1));
  EXPECT_FALSE(cells1.Crosses(cells1));
}

TEST(fields2cover_types_geometry, distance) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  ring1.reversePoints();
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};


  EXPECT_NEAR(p_t.Distance(p_n), sqrt(2), 1e-7);
  EXPECT_NEAR(p_t.Distance(ps), 0, 1e-7);
  EXPECT_NEAR(p_t.Distance(line), 0, 1e-7);
  EXPECT_NEAR(p_t.Distance(lines), 0, 1e-7);
  EXPECT_NEAR(p_t.Distance(ring1), -1, 1e-7);
  EXPECT_NEAR(p_t.Distance(cell1), 0, 1e-7);
  EXPECT_NEAR(p_t.Distance(cells1), 0, 1e-7);
  EXPECT_NEAR(p_n.Distance(ring1), -1, 1e-7);
  EXPECT_NEAR(p_n.Distance(cell1), sqrt(2)/2.0, 1e-7);
  EXPECT_NEAR(p_n.Distance(cells1), sqrt(2)/2.0, 1e-7);

  EXPECT_NEAR(ps.Distance(p_n), 0, 1e-7);
  EXPECT_NEAR(ps.Distance(ps), 0, 1e-7);
  EXPECT_NEAR(ps.Distance(line), 0, 1e-7);
  EXPECT_NEAR(ps.Distance(lines), 0, 1e-7);
  EXPECT_NEAR(ps.Distance(ring1), -1, 1e-7);
  EXPECT_NEAR(ps.Distance(cell1), 0, 1e-7);
  EXPECT_NEAR(ps.Distance(cells1), 0, 1e-7);

  EXPECT_NEAR(line.Distance(p_t), 0, 1e-7);
  EXPECT_NEAR(line.Distance(ps), 0, 1e-7);
  EXPECT_NEAR(line.Distance(line), 0, 1e-7);
  EXPECT_NEAR(line.Distance(lines), 0, 1e-7);
  EXPECT_NEAR(line.Distance(ring1), -1, 1e-7);
  EXPECT_NEAR(line.Distance(cell1), 0, 1e-7);
  EXPECT_NEAR(line.Distance(cells1), 0, 1e-7);

  EXPECT_NEAR(lines.Distance(p_t), 0, 1e-7);
  EXPECT_NEAR(lines.Distance(ps), 0, 1e-7);
  EXPECT_NEAR(lines.Distance(line), 0, 1e-7);
  EXPECT_NEAR(lines.Distance(lines), 0, 1e-7);
  EXPECT_NEAR(lines.Distance(ring1), -1, 1e-7);
  EXPECT_NEAR(lines.Distance(cell1), 0, 1e-7);
  EXPECT_NEAR(lines.Distance(cells1), 0, 1e-7);

  EXPECT_NEAR(ring1.Distance(p_t), -1, 1e-7);
  EXPECT_NEAR(ring1.Distance(ps), -1, 1e-7);
  EXPECT_NEAR(ring1.Distance(line), -1, 1e-7);
  EXPECT_NEAR(ring1.Distance(lines), -1, 1e-7);
  EXPECT_NEAR(ring1.Distance(ring1), -1, 1e-7);
  EXPECT_NEAR(ring1.Distance(cell1), -1, 1e-7);
  EXPECT_NEAR(ring1.Distance(cells1), -1, 1e-7);

  EXPECT_NEAR(cell1.Distance(p_t), 0, 1e-7);
  EXPECT_NEAR(cell1.Distance(ps), 0, 1e-7);
  EXPECT_NEAR(cell1.Distance(line), 0, 1e-7);
  EXPECT_NEAR(cell1.Distance(lines), 0, 1e-7);
  EXPECT_NEAR(cell1.Distance(ring1), -1, 1e-7);
  EXPECT_NEAR(cell1.Distance(cell1), 0, 1e-7);
  EXPECT_NEAR(cell1.Distance(cells1), 0, 1e-7);

  EXPECT_NEAR(cells1.Distance(p_t), 0, 1e-7);
  EXPECT_NEAR(cells1.Distance(ps), 0, 1e-7);
  EXPECT_NEAR(cells1.Distance(line), 0, 1e-7);
  EXPECT_NEAR(cells1.Distance(lines), 0, 1e-7);
  EXPECT_NEAR(cells1.Distance(ring1), -1, 1e-7);
  EXPECT_NEAR(cells1.Distance(cell1), 0, 1e-7);
  EXPECT_NEAR(cells1.Distance(cells1), 0, 1e-7);


}

TEST(fields2cover_types_geometry, getDimMinX) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_NEAR(p_t.getDimMinX(), 0.5, 1e-7);
  EXPECT_NEAR(ps.getDimMinX(), -0.5, 1e-7);
  EXPECT_NEAR(ring1.getDimMinX(), 0, 1e-7);
  EXPECT_NEAR(line.getDimMinX(), -0.5, 1e-7);
  EXPECT_NEAR(lines.getDimMinX(), -0.5, 1e-7);
  EXPECT_NEAR(cell1.getDimMinX(), 0, 1e-7);
  EXPECT_NEAR(cells1.getDimMinX(), 0, 1e-7);
}

TEST(fields2cover_types_geometry, getDimMinY) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_NEAR(p_t.getDimMinY(), 0.5, 1e-7);
  EXPECT_NEAR(ps.getDimMinY(), -0.5, 1e-7);
  EXPECT_NEAR(ring1.getDimMinY(), 0, 1e-7);
  EXPECT_NEAR(line.getDimMinY(), -0.5, 1e-7);
  EXPECT_NEAR(lines.getDimMinY(), -0.5, 1e-7);
  EXPECT_NEAR(cell1.getDimMinY(), 0, 1e-7);
  EXPECT_NEAR(cells1.getDimMinY(), 0, 1e-7);
}

TEST(fields2cover_types_geometry, getDimMaxX) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_NEAR(p_t.getDimMaxX(), 0.5, 1e-7);
  EXPECT_NEAR(ps.getDimMaxX(), 0.5, 1e-7);
  EXPECT_NEAR(ring1.getDimMaxX(), 4, 1e-7);
  EXPECT_NEAR(line.getDimMaxX(), 0.5, 1e-7);
  EXPECT_NEAR(lines.getDimMaxX(), 0.5, 1e-7);
  EXPECT_NEAR(cell1.getDimMaxX(), 4, 1e-7);
  EXPECT_NEAR(cells1.getDimMaxX(), 4, 1e-7);
}

TEST(fields2cover_types_geometry, getDimMaxY) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_NEAR(p_t.getDimMaxY(), 0.5, 1e-7);
  EXPECT_NEAR(ps.getDimMaxY(), 0.5, 1e-7);
  EXPECT_NEAR(ring1.getDimMaxY(), 3, 1e-7);
  EXPECT_NEAR(line.getDimMaxY(), 0.5, 1e-7);
  EXPECT_NEAR(lines.getDimMaxY(), 0.5, 1e-7);
  EXPECT_NEAR(cell1.getDimMaxY(), 3, 1e-7);
  EXPECT_NEAR(cells1.getDimMaxY(), 3, 1e-7);
}

TEST(fields2cover_types_geometry, getHeight) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_NEAR(p_t.getHeight(), 0, 1e-7);
  EXPECT_NEAR(ps.getHeight(), 1, 1e-7);
  EXPECT_NEAR(ring1.getHeight(), 3, 1e-7);
  EXPECT_NEAR(line.getHeight(), 1, 1e-7);
  EXPECT_NEAR(lines.getHeight(), 1, 1e-7);
  EXPECT_NEAR(cell1.getHeight(), 3, 1e-7);
  EXPECT_NEAR(cells1.getHeight(), 3, 1e-7);
}

TEST(fields2cover_types_geometry, getWidth) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_NEAR(p_t.getWidth(), 0, 1e-7);
  EXPECT_NEAR(ps.getWidth(), 1, 1e-7);
  EXPECT_NEAR(ring1.getWidth(), 4, 1e-7);
  EXPECT_NEAR(line.getWidth(), 1, 1e-7);
  EXPECT_NEAR(lines.getWidth(), 1, 1e-7);
  EXPECT_NEAR(cell1.getWidth(), 4, 1e-7);
  EXPECT_NEAR(cells1.getWidth(), 4, 1e-7);
}

TEST(fields2cover_types_geometry, getMinSafeLength) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_NEAR(p_t.getMinSafeLength(), 0, 1e-7);
  EXPECT_NEAR(ps.getMinSafeLength(), 2, 1e-7);
  EXPECT_NEAR(ring1.getMinSafeLength(), 7, 1e-7);
  EXPECT_NEAR(line.getMinSafeLength(), 2, 1e-7);
  EXPECT_NEAR(lines.getMinSafeLength(), 2, 1e-7);
  EXPECT_NEAR(cell1.getMinSafeLength(), 7, 1e-7);
  EXPECT_NEAR(cells1.getMinSafeLength(), 7, 1e-7);
}


TEST(fields2cover_types_geometry, equality) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_TRUE(p_t == p_t);
  EXPECT_TRUE(ps == ps);
  EXPECT_TRUE(line == line);
  EXPECT_TRUE(lines == lines);
  EXPECT_TRUE(ring1 == ring1);
  EXPECT_TRUE(cell1 == cell1);
  EXPECT_TRUE(cells1 == cells1);

  EXPECT_FALSE(p_t != p_t);
  EXPECT_FALSE(ps != ps);
  EXPECT_FALSE(line != line);
  EXPECT_FALSE(lines != lines);
  EXPECT_FALSE(ring1 != ring1);
  EXPECT_FALSE(cell1 != cell1);
  EXPECT_FALSE(cells1 != cells1);
}

TEST(fields2cover_types_geometry, exportToGML) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_EQ(p_t.exportToGML(), "<gml:Point><gml:coordinates>0.5,0.5,0</gml:coordinates></gml:Point>");
  EXPECT_EQ(ps.exportToGML(), "<gml:MultiPoint><gml:pointMember><gml:Point><gml:coordinates>0.5,0.5,0</gml:coordinates></gml:Point></gml:pointMember><gml:pointMember><gml:Point><gml:coordinates>-0.5,-0.5,0</gml:coordinates></gml:Point></gml:pointMember></gml:MultiPoint>");
  EXPECT_EQ(ring1.exportToGML(), "<gml:LinearRing><gml:coordinates>0,0,0 4,0,0 4,3,0 0,0,0</gml:coordinates></gml:LinearRing>");
  EXPECT_EQ(line.exportToGML(), "<gml:LineString><gml:coordinates>0.5,0.5,0 -0.5,-0.5,0</gml:coordinates></gml:LineString>");
  EXPECT_EQ(lines.exportToGML(), "<gml:MultiLineString><gml:lineStringMember><gml:LineString><gml:coordinates>0.5,0.5,0 -0.5,-0.5,0</gml:coordinates></gml:LineString></gml:lineStringMember></gml:MultiLineString>");
  EXPECT_EQ(cell1.exportToGML(), "<gml:Polygon><gml:outerBoundaryIs><gml:LinearRing><gml:coordinates>0,0,0 4,0,0 4,3,0 0,0,0</gml:coordinates></gml:LinearRing></gml:outerBoundaryIs></gml:Polygon>");
  EXPECT_EQ(cells1.exportToGML(), "<gml:MultiPolygon><gml:polygonMember><gml:Polygon><gml:outerBoundaryIs><gml:LinearRing><gml:coordinates>0,0,0 4,0,0 4,3,0 0,0,0</gml:coordinates></gml:LinearRing></gml:outerBoundaryIs></gml:Polygon></gml:polygonMember></gml:MultiPolygon>");
}

TEST(fields2cover_types_geometry, exportToKML) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_EQ(p_t.exportToKML(), "<Point><coordinates>0.5,0.5,0</coordinates></Point>");
  EXPECT_EQ(ps.exportToKML(), "<MultiGeometry><Point><coordinates>0.5,0.5,0</coordinates></Point><Point><coordinates>-0.5,-0.5,0</coordinates></Point></MultiGeometry>");
  EXPECT_EQ(ring1.exportToKML(), "<LinearRing><coordinates>0,0,0 4,0,0 4,3,0 0,0,0</coordinates></LinearRing>");
  EXPECT_EQ(line.exportToKML(), "<LineString><coordinates>0.5,0.5,0 -0.5,-0.5,0</coordinates></LineString>");
  EXPECT_EQ(lines.exportToKML(), "<MultiGeometry><LineString><coordinates>0.5,0.5,0 -0.5,-0.5,0</coordinates></LineString></MultiGeometry>");
  EXPECT_EQ(cell1.exportToKML(), "<Polygon><outerBoundaryIs><LinearRing><coordinates>0,0,0 4,0,0 4,3,0 0,0,0</coordinates></LinearRing></outerBoundaryIs></Polygon>");
  EXPECT_EQ(cells1.exportToKML(), "<MultiGeometry><Polygon><outerBoundaryIs><LinearRing><coordinates>0,0,0 4,0,0 4,3,0 0,0,0</coordinates></LinearRing></outerBoundaryIs></Polygon></MultiGeometry>");
}

TEST(fields2cover_types_geometry, exportToJson) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_EQ(p_t.exportToJson(), "{ \"type\": \"Point\", \"coordinates\": [ 0.5, 0.5, 0.0 ] }");
  EXPECT_EQ(ps.exportToJson(), "{ \"type\": \"MultiPoint\", \"coordinates\": [ [ 0.5, 0.5, 0.0 ], [ -0.5, -0.5, 0.0 ] ] }");
  EXPECT_EQ(ring1.exportToJson(), "{ \"type\": \"LineString\", \"coordinates\": [ [ 0.0, 0.0, 0.0 ], [ 4.0, 0.0, 0.0 ], [ 4.0, 3.0, 0.0 ], [ 0.0, 0.0, 0.0 ] ] }");
  EXPECT_EQ(line.exportToJson(), "{ \"type\": \"LineString\", \"coordinates\": [ [ 0.5, 0.5, 0.0 ], [ -0.5, -0.5, 0.0 ] ] }");
  EXPECT_EQ(lines.exportToJson(), "{ \"type\": \"MultiLineString\", \"coordinates\": [ [ [ 0.5, 0.5, 0.0 ], [ -0.5, -0.5, 0.0 ] ] ] }");
  EXPECT_EQ(cell1.exportToJson(), "{ \"type\": \"Polygon\", \"coordinates\": [ [ [ 0.0, 0.0, 0.0 ], [ 4.0, 0.0, 0.0 ], [ 4.0, 3.0, 0.0 ], [ 0.0, 0.0, 0.0 ] ] ] }");
  EXPECT_EQ(cells1.exportToJson(), "{ \"type\": \"MultiPolygon\", \"coordinates\": [ [ [ [ 0.0, 0.0, 0.0 ], [ 4.0, 0.0, 0.0 ], [ 4.0, 3.0, 0.0 ], [ 0.0, 0.0, 0.0 ] ] ] ] }");
}

TEST(fields2cover_types_geometry, rotateFromPoint) {
  F2CPoint p_in(1, 0), p_out(1, 0);
  double pi = boost::math::constants::pi<double>();
  p_out = F2CPoint(0,0).rotateFromPoint(pi/2.0, p_out);
  EXPECT_NEAR(p_out.getX(), 0, 1e-7);
  EXPECT_NEAR(p_out.getY(), 1, 1e-7);
  p_out = F2CPoint(0,0).rotateFromPoint(pi/2.0, p_out);
  EXPECT_NEAR(p_out.getX(), -1, 1e-7);
  EXPECT_NEAR(p_out.getY(), 0, 1e-7);
  p_out = F2CPoint(0,0).rotateFromPoint(pi/2.0, p_out);
  EXPECT_NEAR(p_out.getX(), 0, 1e-7);
  EXPECT_NEAR(p_out.getY(), -1, 1e-7);
  p_out = F2CPoint(0,0).rotateFromPoint(pi/2.0, p_out);
  EXPECT_NEAR(p_out.getX(), 1, 1e-7);
  EXPECT_NEAR(p_out.getY(), 0, 1e-7);
  p_out = p_in.rotateFromPoint(pi/2.0, p_out);
  EXPECT_NEAR(p_out.getX(), 1, 1e-7);
  EXPECT_NEAR(p_out.getY(), 0, 1e-7);
  p_out = F2CPoint(0.5, 0).rotateFromPoint(pi/2.0, p_out);
  EXPECT_NEAR(p_out.getX(), 0.5, 1e-7);
  EXPECT_NEAR(p_out.getY(), 0.5, 1e-7);

  F2CLineString line;
  line->addPoint(1, 2);
  line->addPoint(3, 4);
  line = F2CPoint(5, 6).rotateFromPoint(pi, line);
  p_out = line.StartPoint();
  EXPECT_NEAR(p_out.getX(), 9, 1e-7);
  EXPECT_NEAR(p_out.getY(), 10, 1e-7);
  p_out = line.EndPoint();
  EXPECT_NEAR(p_out.getX(), 7, 1e-7);
  EXPECT_NEAR(p_out.getY(), 8, 1e-7);
}


