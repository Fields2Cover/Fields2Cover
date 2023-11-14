//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/utils/random.h"
#include "fields2cover/types.h"

namespace f2c {

TEST(fields2cover_types_point, minus) {
  F2CPoint p1(1.1, 2.2, 3.3), p2(-0.5, 0.6, 1);
  auto p3 = p2 - p1;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(p1.getZ(), 3.3, 1e-7);
  EXPECT_NEAR(p2.getX(), -0.5, 1e-7);
  EXPECT_NEAR(p2.getY(), 0.6, 1e-7);
  EXPECT_NEAR(p2.getZ(), 1, 1e-7);
  EXPECT_NEAR(p3.getX(), -1.6, 1e-7);
  EXPECT_NEAR(p3.getY(), -1.6, 1e-7);
  EXPECT_NEAR(p3.getZ(), -2.3, 1e-7);

  auto p4 = p2 - p1;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(p1.getZ(), 3.3, 1e-7);
  EXPECT_NEAR(p2.getX(), -0.5, 1e-7);
  EXPECT_NEAR(p2.getY(), 0.6, 1e-7);
  EXPECT_NEAR(p2.getZ(), 1, 1e-7);
  EXPECT_NEAR(p4.getX(), -1.6, 1e-7);
  EXPECT_NEAR(p4.getY(), -1.6, 1e-7);
  EXPECT_NEAR(p4.getZ(), -2.3, 1e-7);

  OGRPoint op2(-0.5, 0.6, 1);
  auto op4 = op2 - p1;
  EXPECT_NEAR(op4.getX(), -1.6, 1e-7);
  EXPECT_NEAR(op4.getY(), -1.6, 1e-7);
  EXPECT_NEAR(op4.getZ(), -2.3, 1e-7);
}

TEST(fields2cover_types_point, plus) {
  F2CPoint p1(1.1, 2.2, 3.3), p2(-0.5, 0.6, 1);
  auto p3 = p2 + p1;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(p1.getZ(), 3.3, 1e-7);
  EXPECT_NEAR(p2.getX(), -0.5, 1e-7);
  EXPECT_NEAR(p2.getY(), 0.6, 1e-7);
  EXPECT_NEAR(p2.getZ(), 1, 1e-7);
  EXPECT_NEAR(p3.getX(), 0.6, 1e-7);
  EXPECT_NEAR(p3.getY(), 2.8, 1e-7);
  EXPECT_NEAR(p3.getZ(), 4.3, 1e-7);

  OGRPoint op2(-0.5, 0.6, 1);
  auto p4 = op2 + p1;
  EXPECT_NEAR(p4.getX(), 0.6, 1e-7);
  EXPECT_NEAR(p4.getY(), 2.8, 1e-7);
  EXPECT_NEAR(p4.getZ(), 4.3, 1e-7);
}

TEST(fields2cover_types_point, mult_equal) {
  F2CPoint p1(1.1, 2.2, 3);
  p1 *= 1.5;
  EXPECT_NEAR(p1.getX(), 1.65, 1e-7);
  EXPECT_NEAR(p1.getY(), 3.3, 1e-7);
  EXPECT_NEAR(p1.getZ(), 4.5, 1e-7);
}

TEST(fields2cover_types_point, multiply) {
  F2CPoint p1(1.1, 2.2, 3);
  auto p2 = p1 * 1.5;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(p1.getZ(), 3, 1e-7);
  EXPECT_NEAR(p2.getX(), 1.65, 1e-7);
  EXPECT_NEAR(p2.getY(), 3.3, 1e-7);
  EXPECT_NEAR(p2.getZ(), 4.5, 1e-7);
}

TEST(fields2cover_types_point, plus_v) {
  F2CPoint p1(1.1, 2.2);
  std::vector<F2CPoint> v {F2CPoint(0.0, 0.0), F2CPoint(1.0, 2.0)};
  auto p2 = v + p1;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(v[0].getX(), 0, 1e-7);
  EXPECT_NEAR(v[0].getY(), 0, 1e-7);
  EXPECT_NEAR(v[1].getX(), 1, 1e-7);
  EXPECT_NEAR(v[1].getY(), 2, 1e-7);
  EXPECT_NEAR(p2[0].getX(), 1.1, 1e-7);
  EXPECT_NEAR(p2[0].getY(), 2.2, 1e-7);
  EXPECT_NEAR(p2[1].getX(), 2.1, 1e-7);
  EXPECT_NEAR(p2[1].getY(), 4.2, 1e-7);
}

TEST(fields2cover_types_point, minus_v) {
  F2CPoint p1(1.1, 2.2);
  std::vector<F2CPoint> v {F2CPoint(0.0, 0.0), F2CPoint(1.0, 2.0)};
  auto p2 = v - p1;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(v[0].getX(), 0, 1e-7);
  EXPECT_NEAR(v[0].getY(), 0, 1e-7);
  EXPECT_NEAR(v[1].getX(), 1, 1e-7);
  EXPECT_NEAR(v[1].getY(), 2, 1e-7);
  EXPECT_NEAR(p2[0].getX(), -1.1, 1e-7);
  EXPECT_NEAR(p2[0].getY(), -2.2, 1e-7);
  EXPECT_NEAR(p2[1].getX(), -0.1, 1e-7);
  EXPECT_NEAR(p2[1].getY(), -0.2, 1e-7);

  auto p3 = p1 - v;
  EXPECT_NEAR(p1.getX(), 1.1, 1e-7);
  EXPECT_NEAR(p1.getY(), 2.2, 1e-7);
  EXPECT_NEAR(v[0].getX(), 0, 1e-7);
  EXPECT_NEAR(v[0].getY(), 0, 1e-7);
  EXPECT_NEAR(v[1].getX(), 1, 1e-7);
  EXPECT_NEAR(v[1].getY(), 2, 1e-7);
  EXPECT_NEAR(p3[0].getX(), 1.1, 1e-7);
  EXPECT_NEAR(p3[0].getY(), 2.2, 1e-7);
  EXPECT_NEAR(p3[1].getX(), 0.1, 1e-7);
  EXPECT_NEAR(p3[1].getY(), 0.2, 1e-7);
}

TEST(fields2cover_types_point, clone) {
  F2CPoint a(2, 3);
  F2CPoint b = a.clone();
  a.setX(10);
  EXPECT_EQ(b.getX(), 2);
}

TEST(fields2cover_types_point, set) {
  F2CPoint a(1, 2);
  EXPECT_EQ(a, F2CPoint(1, 2, 0));
  a.setPoint(3, 4, 5);
  EXPECT_EQ(a, F2CPoint(3, 4, 5));
  a.setPoint(OGRPoint(6, 7));
  EXPECT_EQ(a, F2CPoint(6, 7, 0));
  a.setPoint(F2CPoint(8, 9, 10));
  EXPECT_EQ(a, F2CPoint(8, 9, 10));
}

TEST(fields2cover_types_point, print) {
  F2CPoint a(1, 2);
  std::ostringstream ss_a;
  ss_a << a;
  EXPECT_EQ(ss_a.str(), "Point(1, 2, 0)");
  a.setZ(-5);
  std::ostringstream ss_a2;
  ss_a2 << a;
  EXPECT_EQ(ss_a2.str(), "Point(1, 2, -5)");
}

TEST(fields2cover_types_point, rotateFromPoint) {
  F2CPoint p0(0, 0), p1(0, 1), p2(2, 0);
  F2CMultiPoint ps {p1, p2};
  F2CLineString ls {p1, p2};
  F2CLineString ls2 {p1};
  F2CMultiLineString mls {ls, ls2};
  std::vector<F2CPoint> vp {p1, p2};
  std::vector<F2CMultiPoint> pss {ps};
  std::vector<F2CLineString> vls {ls, ls2};
  std::vector<F2CMultiLineString> vmls {mls};
  auto r_ls = p0.rotateFromPoint(0.5*boost::math::constants::half_pi<double>(), ls);
  EXPECT_NEAR(r_ls.getGeometry(0).getX(), -1./sqrt(2), 1e-7);
  EXPECT_NEAR(r_ls.getGeometry(0).getY(), 1./sqrt(2), 1e-7);
  EXPECT_NEAR(r_ls.getGeometry(1).getX(), sqrt(2), 1e-7);
  EXPECT_NEAR(r_ls.getGeometry(1).getY(), sqrt(2), 1e-7);

  auto r_mls = p0.rotateFromPoint(0.5*boost::math::constants::half_pi<double>(), mls);
  EXPECT_EQ(r_mls.size(), 2);
  auto r_ls3 = r_mls.getGeometry(0);
  EXPECT_NEAR(r_ls3.getGeometry(0).getX(), -1./sqrt(2), 1e-7);
  EXPECT_NEAR(r_ls3.getGeometry(0).getY(), 1./sqrt(2), 1e-7);
  EXPECT_NEAR(r_ls3.getGeometry(1).getX(), sqrt(2), 1e-7);
  EXPECT_NEAR(r_ls3.getGeometry(1).getY(), sqrt(2), 1e-7);

  auto r_vmls = p0.rotateFromPoint(0.5*boost::math::constants::half_pi<double>(), vmls);
  EXPECT_EQ(r_vmls.size(), 1);
  EXPECT_EQ(r_vmls[0].size(), 2);
  auto r_ls4 = r_vmls[0].getGeometry(0);
  EXPECT_NEAR(r_ls4.getGeometry(0).getX(), -1./sqrt(2), 1e-7);
  EXPECT_NEAR(r_ls4.getGeometry(0).getY(), 1./sqrt(2), 1e-7);
  EXPECT_NEAR(r_ls4.getGeometry(1).getX(), sqrt(2), 1e-7);
  EXPECT_NEAR(r_ls4.getGeometry(1).getY(), sqrt(2), 1e-7);

  auto r_vls = p0.rotateFromPoint(0.5*boost::math::constants::half_pi<double>(), vls);
  EXPECT_EQ(r_vls.size(), 2);
  auto r_ls5 = r_vls[0];
  EXPECT_NEAR(r_ls5.getGeometry(0).getX(), -1./sqrt(2), 1e-7);
  EXPECT_NEAR(r_ls5.getGeometry(0).getY(), 1./sqrt(2), 1e-7);
  EXPECT_NEAR(r_ls5.getGeometry(1).getX(), sqrt(2), 1e-7);
  EXPECT_NEAR(r_ls5.getGeometry(1).getY(), sqrt(2), 1e-7);

  auto r_p1 = p0.rotateFromPoint(boost::math::constants::half_pi<double>(), p1);
  EXPECT_NEAR(r_p1.getX(), -1, 1e-7);
  EXPECT_NEAR(r_p1.getY(), 0, 1e-7);
  auto r_p2 = p0.rotateFromPoint(boost::math::constants::half_pi<double>(), p2);
  EXPECT_NEAR(r_p2.getX(), 0, 1e-7);
  EXPECT_NEAR(r_p2.getY(), 2, 1e-7);
  auto r_ps = p0.rotateFromPoint(boost::math::constants::half_pi<double>(), ps);
  EXPECT_NEAR(r_ps.getGeometry(1).getX(), 0, 1e-7);
  EXPECT_NEAR(r_ps.getGeometry(1).getY(), 2, 1e-7);
  auto r_pss = p0.rotateFromPoint(boost::math::constants::half_pi<double>(), pss);
  EXPECT_NEAR(r_pss[0].getGeometry(1).getX(), 0, 1e-7);
  EXPECT_NEAR(r_pss[0].getGeometry(1).getY(), 2, 1e-7);
  auto r_vp = p0.rotateFromPoint(boost::math::constants::half_pi<double>(), vp);
  EXPECT_NEAR(r_vp[1].getX(), 0, 1e-7);
  EXPECT_NEAR(r_vp[1].getY(), 2, 1e-7);

  f2c::Random rand(42);
  auto field = rand.generateRandField( 3, 1e4);
  EXPECT_NEAR(field.field.getArea(), 1e4, 1e-7);
  F2CCells cells = field.field;
  EXPECT_NEAR(cells.getArea(), 1e4, 1e-7);
  auto r_cells = p0.rotateFromPoint(boost::math::constants::pi<double>(), cells);
  F2CPoint start = cells.getGeometry(0).getGeometry(0).getGeometry(1);
  F2CPoint r_start = r_cells.getGeometry(0).getGeometry(0).getGeometry(1);
  EXPECT_NEAR(cells.getArea(), 1e4, 1e-7);
  EXPECT_NEAR(r_cells.getArea(), 1e4, 1e-7);
  EXPECT_NEAR(start.getX(), -r_start.getX(), 1e-7);
  EXPECT_NEAR(start.getY(), -r_start.getY(), 1e-7);

  auto r_cell = p0.rotateFromPoint(boost::math::constants::pi<double>(),
      cells.getGeometry(0));
  F2CPoint r_start2 = r_cell.getGeometry(0).getGeometry(1);
  EXPECT_NEAR(start.getX(), -r_start2.getX(), 1e-7);
  EXPECT_NEAR(start.getY(), -r_start2.getY(), 1e-7);

  std::vector<F2CLinearRing> v_ring {cells.getGeometry(0).getGeometry(0)};
  auto r_v_ring = p0.rotateFromPoint(boost::math::constants::pi<double>(),
      v_ring);
  F2CPoint r_v_start6 = r_v_ring[0].getGeometry(1);
  EXPECT_NEAR(start.getX(), -r_v_start6.getX(), 1e-7);
  EXPECT_NEAR(start.getY(), -r_v_start6.getY(), 1e-7);

  std::vector<F2CCell> v_cell {cells.getGeometry(0)};
  auto r_v_cell = p0.rotateFromPoint(boost::math::constants::pi<double>(),
      v_cell);
  F2CPoint r_v_start2 = r_v_cell[0].getGeometry(0).getGeometry(1);
  EXPECT_NEAR(start.getX(), -r_v_start2.getX(), 1e-7);
  EXPECT_NEAR(start.getY(), -r_v_start2.getY(), 1e-7);

  std::vector<F2CCells> v_cells {cells};
  auto r_v_cells = p0.rotateFromPoint(boost::math::constants::pi<double>(),
      v_cells);
  F2CPoint r_v_start3 = r_v_cells[0].getGeometry(0).getGeometry(0).getGeometry(1);
  EXPECT_NEAR(start.getX(), -r_v_start3.getX(), 1e-7);
  EXPECT_NEAR(start.getY(), -r_v_start3.getY(), 1e-7);
}


}  // namespace f2c

