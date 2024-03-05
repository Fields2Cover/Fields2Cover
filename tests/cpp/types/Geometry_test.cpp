//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

template <class T>
class fields2cover_types_geometry : public testing::Test {};

typedef ::testing::Types<f2c::types::Geometry<OGRPoint, wkbPoint>, F2CPoint, f2c::types::Geometry<OGRLinearRing, wkbLinearRing>, F2CLinearRing, f2c::types::Geometry<OGRLineString, wkbLineString>, F2CLineString, f2c::types::Geometry<OGRMultiPoint, wkbMultiPoint>, F2CMultiPoint, f2c::types::Geometry<OGRMultiLineString, wkbMultiLineString>, F2CMultiLineString, f2c::types::Geometry<OGRPolygon, wkbPolygon>, F2CCell, f2c::types::Geometry<OGRMultiPolygon, wkbMultiPolygon>, F2CCells> Implementations;
TYPED_TEST_CASE(fields2cover_types_geometry, Implementations);

TYPED_TEST(fields2cover_types_geometry, getAngContinuity) {
  std::vector<double> v;
  for (double d = -50.0; d < 50.0; d += 0.1) {
    v.push_back(TypeParam::mod_2pi(d));
    EXPECT_NEAR(TypeParam::getAngContinuity(d - 0.1, TypeParam::mod_2pi(d)), d, 1e-5);
  }
  for (double d = 50.0; d > -50.0; d -= 0.1) {
    v.push_back(TypeParam::mod_2pi(d));
    EXPECT_NEAR(TypeParam::getAngContinuity(d + 0.1, TypeParam::mod_2pi(d)), d, 1e-5);
  }
  auto restored_v = TypeParam::getAngContinuity(v);
  for (int i = 1; i < restored_v.size(); ++i) {
    EXPECT_NEAR(fabs(restored_v[i] - restored_v[i - 1]), 0.1, 1e-5);
  }
}

TYPED_TEST(fields2cover_types_geometry, getAngleAvg) {
  EXPECT_NEAR(TypeParam::getAngleAvg(0, M_PI / 2.0), M_PI / 4.0, 1e-7);
  EXPECT_NEAR(TypeParam::getAngleAvg(0, -M_PI / 2.0), 7.0 * M_PI / 4.0, 1e-7);
  EXPECT_NEAR(TypeParam::getAngleAvg(M_PI, -M_PI / 2.0), 5.0 * M_PI / 4.0, 1e-7);
  EXPECT_NEAR(TypeParam::getAngleAvg(5. * M_PI / 4., 3. * M_PI / 4.), M_PI, 1e-7);
  EXPECT_NEAR(TypeParam::getAngleAvg(3. * M_PI / 4., 5. * M_PI / 4.), M_PI, 1e-7);
}

TYPED_TEST(fields2cover_types_geometry, getAngleDiff) {
  EXPECT_NEAR(TypeParam::getAngleDiffAbs(M_PI*0.25, M_PI), 0.75*M_PI, 1e-5);
  EXPECT_NEAR(TypeParam::getAngleDiffAbs(-M_PI*0.25, M_PI), 0.75*M_PI, 1e-5);
  EXPECT_NEAR(TypeParam::getAngleDiffAbs(-M_PI*0.25, 0), 0.25*M_PI, 1e-5);
  EXPECT_NEAR(TypeParam::getAngleDiffAbs(-M_PI*0.25, M_PI*0.5), 0.75*M_PI, 1e-5);
}

TYPED_TEST(fields2cover_types_geometry, mod) {
  EXPECT_EQ(TypeParam::mod(2.0, 6.5), 2.0);
  EXPECT_EQ(TypeParam::mod(20.0, 7.0), 6.0);
  EXPECT_EQ(TypeParam::mod(-1.0, 4.0), 3.0);
}
TYPED_TEST(fields2cover_types_geometry, init) {
  TypeParam t;
  TypeParam t2 = TypeParam(t.get());
  auto p_t3 = t->clone();
  TypeParam t3 = TypeParam(p_t3);
  OGRGeometryFactory::destroyGeometry(p_t3);
  TypeParam t4 {t};
  TypeParam t5 = TypeParam(t.operator->());
  TypeParam t6 = TypeParam(static_cast<OGRGeometry*>(t.get()));
  TypeParam t7 = TypeParam(static_cast<OGRGeometry const*>(t.get()));
  TypeParam t8;
  t8 = t;
  EXPECT_FALSE(t != t2);
  EXPECT_FALSE(t != t3);
  EXPECT_TRUE(t == t2);
  EXPECT_TRUE(t == t3);
  EXPECT_TRUE(t == t4);
  EXPECT_TRUE(t == t5);
  EXPECT_TRUE(t == t6);
  EXPECT_TRUE(t == t7);
  EXPECT_TRUE(t == t8);
}

TEST(fields2cover_types_geometry, intersects) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_FALSE(p_t.intersects(p_n));
  EXPECT_TRUE(p_t.intersects(ps));
  EXPECT_TRUE(p_t.intersects(line));
  EXPECT_TRUE(p_t.intersects(lines));
  EXPECT_FALSE(p_t.intersects(ring1));
  EXPECT_TRUE(p_t.intersects(cell1));
  EXPECT_TRUE(p_t.intersects(cells1));
  EXPECT_FALSE(p_n.intersects(ring1));
  EXPECT_FALSE(p_n.intersects(cell1));
  EXPECT_FALSE(p_n.intersects(cells1));

  EXPECT_TRUE(ps.intersects(p_n));
  EXPECT_TRUE(ps.intersects(ps));
  EXPECT_TRUE(ps.intersects(line));
  EXPECT_TRUE(ps.intersects(lines));
  EXPECT_FALSE(ps.intersects(ring1));
  EXPECT_TRUE(ps.intersects(cell1));
  EXPECT_TRUE(ps.intersects(cells1));

  EXPECT_TRUE(line.intersects(p_t));
  EXPECT_TRUE(line.intersects(ps));
  EXPECT_TRUE(line.intersects(line));
  EXPECT_TRUE(line.intersects(lines));
  EXPECT_FALSE(line.intersects(ring1));
  EXPECT_TRUE(line.intersects(cell1));
  EXPECT_TRUE(line.intersects(cells1));

  EXPECT_TRUE(lines.intersects(p_t));
  EXPECT_TRUE(lines.intersects(ps));
  EXPECT_TRUE(lines.intersects(line));
  EXPECT_TRUE(lines.intersects(lines));
  EXPECT_FALSE(lines.intersects(ring1));
  EXPECT_TRUE(lines.intersects(cell1));
  EXPECT_TRUE(lines.intersects(cells1));

  EXPECT_FALSE(ring1.intersects(p_t));
  EXPECT_FALSE(ring1.intersects(ps));
  EXPECT_FALSE(ring1.intersects(line));
  EXPECT_FALSE(ring1.intersects(lines));
  EXPECT_FALSE(ring1.intersects(ring1));
  EXPECT_FALSE(ring1.intersects(cell1));
  EXPECT_FALSE(ring1.intersects(cells1));

  EXPECT_TRUE(cell1.intersects(p_t));
  EXPECT_TRUE(cell1.intersects(ps));
  EXPECT_TRUE(cell1.intersects(line));
  EXPECT_TRUE(cell1.intersects(lines));
  EXPECT_FALSE(cell1.intersects(ring1));
  EXPECT_TRUE(cell1.intersects(cell1));
  EXPECT_TRUE(cell1.intersects(cells1));

  EXPECT_TRUE(cells1.intersects(p_t));
  EXPECT_TRUE(cells1.intersects(ps));
  EXPECT_TRUE(cells1.intersects(line));
  EXPECT_TRUE(cells1.intersects(lines));
  EXPECT_FALSE(cells1.intersects(ring1));
  EXPECT_TRUE(cells1.intersects(cell1));
  EXPECT_TRUE(cells1.intersects(cells1));
}

TEST(fields2cover_types_geometry, touches) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};

  EXPECT_FALSE(p_t.touches(p_n));
  EXPECT_FALSE(p_t.touches(ps));
  EXPECT_TRUE(p_t.touches(line));
  EXPECT_TRUE(p_t.touches(lines));
  EXPECT_FALSE(p_t.touches(ring1));
  EXPECT_FALSE(p_t.touches(cell1));
  EXPECT_FALSE(p_t.touches(cells1));
  EXPECT_FALSE(p_n.touches(ring1));
  EXPECT_FALSE(p_n.touches(cell1));
  EXPECT_FALSE(p_n.touches(cells1));

  EXPECT_FALSE(ps.touches(p_n));
  EXPECT_FALSE(ps.touches(ps));
  EXPECT_TRUE(ps.touches(line));
  EXPECT_TRUE(ps.touches(lines));
  EXPECT_FALSE(ps.touches(ring1));
  EXPECT_FALSE(ps.touches(cell1));
  EXPECT_FALSE(ps.touches(cells1));

  EXPECT_TRUE(line.touches(p_t));
  EXPECT_TRUE(line.touches(ps));
  EXPECT_FALSE(line.touches(line));
  EXPECT_FALSE(line.touches(lines));
  EXPECT_FALSE(line.touches(ring1));
  EXPECT_FALSE(line.touches(cell1));
  EXPECT_FALSE(line.touches(cells1));

  EXPECT_TRUE(lines.touches(p_t));
  EXPECT_TRUE(lines.touches(ps));
  EXPECT_FALSE(lines.touches(line));
  EXPECT_FALSE(lines.touches(lines));
  EXPECT_FALSE(lines.touches(ring1));
  EXPECT_FALSE(lines.touches(cell1));
  EXPECT_FALSE(lines.touches(cells1));

  EXPECT_FALSE(ring1.touches(p_t));
  EXPECT_FALSE(ring1.touches(ps));
  EXPECT_FALSE(ring1.touches(line));
  EXPECT_FALSE(ring1.touches(lines));
  EXPECT_FALSE(ring1.touches(ring1));
  EXPECT_FALSE(ring1.touches(cell1));
  EXPECT_FALSE(ring1.touches(cells1));

  EXPECT_FALSE(cell1.touches(p_t));
  EXPECT_FALSE(cell1.touches(ps));
  EXPECT_FALSE(cell1.touches(line));
  EXPECT_FALSE(cell1.touches(lines));
  EXPECT_FALSE(cell1.touches(ring1));
  EXPECT_FALSE(cell1.touches(cell1));
  EXPECT_FALSE(cell1.touches(cells1));

  EXPECT_FALSE(cells1.touches(p_t));
  EXPECT_FALSE(cells1.touches(ps));
  EXPECT_FALSE(cells1.touches(line));
  EXPECT_FALSE(cells1.touches(lines));
  EXPECT_FALSE(cells1.touches(ring1));
  EXPECT_FALSE(cells1.touches(cell1));
  EXPECT_FALSE(cells1.touches(cells1));
}

TEST(fields2cover_types_geometry, disjoint) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};



  EXPECT_TRUE(p_t.disjoint(p_n));
  EXPECT_FALSE(p_t.disjoint(ps));
  EXPECT_FALSE(p_t.disjoint(line));
  EXPECT_FALSE(p_t.disjoint(lines));
  EXPECT_FALSE(p_t.disjoint(ring1));
  EXPECT_FALSE(p_t.disjoint(cell1));
  EXPECT_FALSE(p_t.disjoint(cells1));
  EXPECT_FALSE(p_n.disjoint(ring1));
  EXPECT_TRUE(p_n.disjoint(cell1));
  EXPECT_TRUE(p_n.disjoint(cells1));

  EXPECT_FALSE(ps.disjoint(p_n));
  EXPECT_FALSE(ps.disjoint(ps));
  EXPECT_FALSE(ps.disjoint(line));
  EXPECT_FALSE(ps.disjoint(lines));
  EXPECT_FALSE(ps.disjoint(ring1));
  EXPECT_FALSE(ps.disjoint(cell1));
  EXPECT_FALSE(ps.disjoint(cells1));

  EXPECT_FALSE(line.disjoint(p_t));
  EXPECT_FALSE(line.disjoint(ps));
  EXPECT_FALSE(line.disjoint(line));
  EXPECT_FALSE(line.disjoint(lines));
  EXPECT_FALSE(line.disjoint(ring1));
  EXPECT_FALSE(line.disjoint(cell1));
  EXPECT_FALSE(line.disjoint(cells1));

  EXPECT_FALSE(lines.disjoint(p_t));
  EXPECT_FALSE(lines.disjoint(ps));
  EXPECT_FALSE(lines.disjoint(line));
  EXPECT_FALSE(lines.disjoint(lines));
  EXPECT_FALSE(lines.disjoint(ring1));
  EXPECT_FALSE(lines.disjoint(cell1));
  EXPECT_FALSE(lines.disjoint(cells1));

  EXPECT_FALSE(ring1.disjoint(p_t));
  EXPECT_FALSE(ring1.disjoint(ps));
  EXPECT_FALSE(ring1.disjoint(line));
  EXPECT_FALSE(ring1.disjoint(lines));
  EXPECT_FALSE(ring1.disjoint(ring1));
  EXPECT_FALSE(ring1.disjoint(cell1));
  EXPECT_FALSE(ring1.disjoint(cells1));

  EXPECT_FALSE(cell1.disjoint(p_t));
  EXPECT_FALSE(cell1.disjoint(ps));
  EXPECT_FALSE(cell1.disjoint(line));
  EXPECT_FALSE(cell1.disjoint(lines));
  EXPECT_FALSE(cell1.disjoint(ring1));
  EXPECT_FALSE(cell1.disjoint(cell1));
  EXPECT_FALSE(cell1.disjoint(cells1));

  EXPECT_FALSE(cells1.disjoint(p_t));
  EXPECT_FALSE(cells1.disjoint(ps));
  EXPECT_FALSE(cells1.disjoint(line));
  EXPECT_FALSE(cells1.disjoint(lines));
  EXPECT_FALSE(cells1.disjoint(ring1));
  EXPECT_FALSE(cells1.disjoint(cell1));
  EXPECT_FALSE(cells1.disjoint(cells1));
}

TEST(fields2cover_types_geometry, within) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};


  EXPECT_FALSE(p_t.within(p_n));
  EXPECT_TRUE(p_t.within(ps));
  EXPECT_FALSE(p_t.within(line));
  EXPECT_FALSE(p_t.within(lines));
  EXPECT_FALSE(p_t.within(ring1));
  EXPECT_TRUE(p_t.within(cell1));
  EXPECT_TRUE(p_t.within(cells1));
  EXPECT_FALSE(p_n.within(ring1));
  EXPECT_FALSE(p_n.within(cell1));
  EXPECT_FALSE(p_n.within(cells1));

  EXPECT_FALSE(ps.within(p_n));
  EXPECT_TRUE(ps.within(ps));
  EXPECT_FALSE(ps.within(line));
  EXPECT_FALSE(ps.within(lines));
  EXPECT_FALSE(ps.within(ring1));
  EXPECT_FALSE(ps.within(cell1));
  EXPECT_FALSE(ps.within(cells1));

  EXPECT_FALSE(line.within(p_t));
  EXPECT_FALSE(line.within(ps));
  EXPECT_TRUE(line.within(line));
  EXPECT_TRUE(line.within(lines));
  EXPECT_FALSE(line.within(ring1));
  EXPECT_FALSE(line.within(cell1));
  EXPECT_FALSE(line.within(cells1));

  EXPECT_FALSE(lines.within(p_t));
  EXPECT_FALSE(lines.within(ps));
  EXPECT_TRUE(lines.within(line));
  EXPECT_TRUE(lines.within(lines));
  EXPECT_FALSE(lines.within(ring1));
  EXPECT_FALSE(lines.within(cell1));
  EXPECT_FALSE(lines.within(cells1));

  EXPECT_FALSE(ring1.within(p_t));
  EXPECT_FALSE(ring1.within(ps));
  EXPECT_FALSE(ring1.within(line));
  EXPECT_FALSE(ring1.within(lines));
  EXPECT_FALSE(ring1.within(ring1));
  EXPECT_FALSE(ring1.within(cell1));
  EXPECT_FALSE(ring1.within(cells1));

  EXPECT_FALSE(cell1.within(p_t));
  EXPECT_FALSE(cell1.within(ps));
  EXPECT_FALSE(cell1.within(line));
  EXPECT_FALSE(cell1.within(lines));
  EXPECT_FALSE(cell1.within(ring1));
  EXPECT_TRUE(cell1.within(cell1));
  EXPECT_TRUE(cell1.within(cells1));

  EXPECT_FALSE(cells1.within(p_t));
  EXPECT_FALSE(cells1.within(ps));
  EXPECT_FALSE(cells1.within(line));
  EXPECT_FALSE(cells1.within(lines));
  EXPECT_FALSE(cells1.within(ring1));
  EXPECT_TRUE(cells1.within(cell1));
  EXPECT_TRUE(cells1.within(cells1));
}

TEST(fields2cover_types_geometry, crosses) {
  F2CPoint p_t(0.5, 0.5), p_n(-0.5, -0.5);
  F2CLineString line {p_t, p_n};
  F2CMultiLineString lines {line};
  F2CMultiPoint ps {p_t, p_n};
  F2CLinearRing ring1 {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,3), F2CPoint(3,3), F2CPoint(3,1), F2CPoint(1,1), F2CPoint(1,3), F2CPoint(0,3), F2CPoint(0,0)};
  F2CCell cell1 {ring1};
  F2CCells cells1 {cell1};


  EXPECT_FALSE(p_t.crosses(p_n));
  EXPECT_FALSE(p_t.crosses(ps));
  EXPECT_FALSE(p_t.crosses(line));
  EXPECT_FALSE(p_t.crosses(lines));
  EXPECT_FALSE(p_t.crosses(ring1));
  EXPECT_FALSE(p_t.crosses(cell1));
  EXPECT_FALSE(p_t.crosses(cells1));
  EXPECT_FALSE(p_n.crosses(ring1));
  EXPECT_FALSE(p_n.crosses(cell1));
  EXPECT_FALSE(p_n.crosses(cells1));

  EXPECT_FALSE(ps.crosses(p_n));
  EXPECT_FALSE(ps.crosses(ps));
  EXPECT_FALSE(ps.crosses(line));
  EXPECT_FALSE(ps.crosses(lines));
  EXPECT_FALSE(ps.crosses(ring1));
  EXPECT_TRUE(ps.crosses(cell1));
  EXPECT_TRUE(ps.crosses(cells1));

  EXPECT_FALSE(line.crosses(p_t));
  EXPECT_FALSE(line.crosses(ps));
  EXPECT_FALSE(line.crosses(line));
  EXPECT_FALSE(line.crosses(lines));
  EXPECT_FALSE(line.crosses(ring1));
  EXPECT_TRUE(line.crosses(cell1));
  EXPECT_TRUE(line.crosses(cells1));

  EXPECT_FALSE(lines.crosses(p_t));
  EXPECT_FALSE(lines.crosses(ps));
  EXPECT_FALSE(lines.crosses(line));
  EXPECT_FALSE(lines.crosses(lines));
  EXPECT_FALSE(lines.crosses(ring1));
  EXPECT_TRUE(lines.crosses(cell1));
  EXPECT_TRUE(lines.crosses(cells1));

  EXPECT_FALSE(ring1.crosses(p_t));
  EXPECT_FALSE(ring1.crosses(ps));
  EXPECT_FALSE(ring1.crosses(line));
  EXPECT_FALSE(ring1.crosses(lines));
  EXPECT_FALSE(ring1.crosses(ring1));
  EXPECT_FALSE(ring1.crosses(cell1));
  EXPECT_FALSE(ring1.crosses(cells1));

  EXPECT_FALSE(cell1.crosses(p_t));
  EXPECT_TRUE(cell1.crosses(ps));
  EXPECT_TRUE(cell1.crosses(line));
  EXPECT_TRUE(cell1.crosses(lines));
  EXPECT_FALSE(cell1.crosses(ring1));
  EXPECT_FALSE(cell1.crosses(cell1));
  EXPECT_FALSE(cell1.crosses(cells1));

  EXPECT_FALSE(cells1.crosses(p_t));
  EXPECT_TRUE(cells1.crosses(ps));
  EXPECT_TRUE(cells1.crosses(line));
  EXPECT_TRUE(cells1.crosses(lines));
  EXPECT_FALSE(cells1.crosses(ring1));
  EXPECT_FALSE(cells1.crosses(cell1));
  EXPECT_FALSE(cells1.crosses(cells1));
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


  EXPECT_NEAR(p_t.distance(p_n), sqrt(2), 1e-7);
  EXPECT_NEAR(p_t.distance(ps), 0, 1e-7);
  EXPECT_NEAR(p_t.distance(line), 0, 1e-7);
  EXPECT_NEAR(p_t.distance(lines), 0, 1e-7);
  EXPECT_NEAR(p_t.distance(ring1), -1, 1e-7);
  EXPECT_NEAR(p_t.distance(cell1), 0, 1e-7);
  EXPECT_NEAR(p_t.distance(cells1), 0, 1e-7);
  EXPECT_NEAR(p_n.distance(ring1), -1, 1e-7);
  EXPECT_NEAR(p_n.distance(cell1), sqrt(2)/2.0, 1e-7);
  EXPECT_NEAR(p_n.distance(cells1), sqrt(2)/2.0, 1e-7);

  EXPECT_NEAR(ps.distance(p_n), 0, 1e-7);
  EXPECT_NEAR(ps.distance(ps), 0, 1e-7);
  EXPECT_NEAR(ps.distance(line), 0, 1e-7);
  EXPECT_NEAR(ps.distance(lines), 0, 1e-7);
  EXPECT_NEAR(ps.distance(ring1), -1, 1e-7);
  EXPECT_NEAR(ps.distance(cell1), 0, 1e-7);
  EXPECT_NEAR(ps.distance(cells1), 0, 1e-7);

  EXPECT_NEAR(line.distance(p_t), 0, 1e-7);
  EXPECT_NEAR(line.distance(ps), 0, 1e-7);
  EXPECT_NEAR(line.distance(line), 0, 1e-7);
  EXPECT_NEAR(line.distance(lines), 0, 1e-7);
  EXPECT_NEAR(line.distance(ring1), -1, 1e-7);
  EXPECT_NEAR(line.distance(cell1), 0, 1e-7);
  EXPECT_NEAR(line.distance(cells1), 0, 1e-7);

  EXPECT_NEAR(lines.distance(p_t), 0, 1e-7);
  EXPECT_NEAR(lines.distance(ps), 0, 1e-7);
  EXPECT_NEAR(lines.distance(line), 0, 1e-7);
  EXPECT_NEAR(lines.distance(lines), 0, 1e-7);
  EXPECT_NEAR(lines.distance(ring1), -1, 1e-7);
  EXPECT_NEAR(lines.distance(cell1), 0, 1e-7);
  EXPECT_NEAR(lines.distance(cells1), 0, 1e-7);

  EXPECT_NEAR(ring1.distance(p_t), -1, 1e-7);
  EXPECT_NEAR(ring1.distance(ps), -1, 1e-7);
  EXPECT_NEAR(ring1.distance(line), -1, 1e-7);
  EXPECT_NEAR(ring1.distance(lines), -1, 1e-7);
  EXPECT_NEAR(ring1.distance(ring1), -1, 1e-7);
  EXPECT_NEAR(ring1.distance(cell1), -1, 1e-7);
  EXPECT_NEAR(ring1.distance(cells1), -1, 1e-7);

  EXPECT_NEAR(cell1.distance(p_t), 0, 1e-7);
  EXPECT_NEAR(cell1.distance(ps), 0, 1e-7);
  EXPECT_NEAR(cell1.distance(line), 0, 1e-7);
  EXPECT_NEAR(cell1.distance(lines), 0, 1e-7);
  EXPECT_NEAR(cell1.distance(ring1), -1, 1e-7);
  EXPECT_NEAR(cell1.distance(cell1), 0, 1e-7);
  EXPECT_NEAR(cell1.distance(cells1), 0, 1e-7);

  EXPECT_NEAR(cells1.distance(p_t), 0, 1e-7);
  EXPECT_NEAR(cells1.distance(ps), 0, 1e-7);
  EXPECT_NEAR(cells1.distance(line), 0, 1e-7);
  EXPECT_NEAR(cells1.distance(lines), 0, 1e-7);
  EXPECT_NEAR(cells1.distance(ring1), -1, 1e-7);
  EXPECT_NEAR(cells1.distance(cell1), 0, 1e-7);
  EXPECT_NEAR(cells1.distance(cells1), 0, 1e-7);


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
  p_out = line.startPoint();
  EXPECT_NEAR(p_out.getX(), 9, 1e-7);
  EXPECT_NEAR(p_out.getY(), 10, 1e-7);
  p_out = line.endPoint();
  EXPECT_NEAR(p_out.getX(), 7, 1e-7);
  EXPECT_NEAR(p_out.getY(), 8, 1e-7);
}


