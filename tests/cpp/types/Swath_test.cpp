//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_swath, hasSameDir) {
  F2CSwath swath1(F2CLineString({F2CPoint(0, 0), F2CPoint(1, 1)}), 1.0);
  F2CSwath swath2 = swath1.clone();
  swath2.reverse();
  EXPECT_TRUE(swath1.hasSameDir(swath1));
  EXPECT_FALSE(swath1.hasSameDir(swath2));
  EXPECT_FALSE(swath2.hasSameDir(swath1));
  swath1.targetSameDirAs(swath2);
  EXPECT_TRUE(swath1.hasSameDir(swath2));
  EXPECT_EQ(swath1.getPoint(0), F2CPoint(1, 1));
}

TEST(fields2cover_types_swath, comparison) {
  F2CSwath swath1(F2CLineString({F2CPoint(0, 0), F2CPoint(1, 1)}), 1.0, 1);
  F2CSwath swath2(F2CLineString({F2CPoint(0, 0), F2CPoint(2, 2)}), 1.0, 2);
  F2CSwath swath3(F2CLineString({F2CPoint(1, 1), F2CPoint(0, 0)}), 1.0, 3);
  F2CSwath swath4(F2CLineString({F2CPoint(0, 0), F2CPoint(1, 1)}), 2.0, 4);
  F2CSwath swath5(F2CLineString({F2CPoint(1, 0), F2CPoint(2, 2)}), 1.0, 5);
  F2CSwath swath6(F2CLineString({F2CPoint(0, 1), F2CPoint(2, 2)}), 1.0, 6);

  EXPECT_FALSE(swath1 != swath1);
  EXPECT_TRUE(swath1 == swath1);
  EXPECT_TRUE(swath1 >= swath1);
  EXPECT_TRUE(swath1 <= swath1);
  EXPECT_FALSE(swath1 < swath1);
  EXPECT_FALSE(swath1 > swath1);
  EXPECT_TRUE(swath1 != swath3);
  EXPECT_FALSE(swath1 == swath3);
  EXPECT_TRUE(swath1 != swath4);
  EXPECT_FALSE(swath1 == swath4);
  EXPECT_FALSE(swath1 > swath4);
  EXPECT_TRUE(swath1 < swath4);
  EXPECT_FALSE(swath1 >= swath4);
  EXPECT_TRUE(swath1 <= swath4);
  EXPECT_TRUE(swath1 < swath5);
  EXPECT_FALSE(swath1 > swath5);
  EXPECT_TRUE(swath1 <= swath5);
  EXPECT_FALSE(swath1 >= swath5);
  EXPECT_TRUE(swath5 > swath1);
  EXPECT_FALSE(swath5 < swath1);
  EXPECT_TRUE(swath5 >= swath1);
  EXPECT_FALSE(swath5 <= swath1);
  EXPECT_TRUE(swath1 < swath6);
  EXPECT_FALSE(swath1 > swath6);
  EXPECT_TRUE(swath1 <= swath6);
  EXPECT_FALSE(swath1 >= swath6);
  EXPECT_TRUE(swath6 > swath1);
  EXPECT_FALSE(swath6 < swath1);
  EXPECT_TRUE(swath6 >= swath1);
  EXPECT_FALSE(swath6 <= swath1);
}



TEST(fields2cover_types_swath, length) {
  F2CLineString path1, path2, path3;
  path1->addPoint( 0.0, 1.0);
  path1->addPoint( 4.0, 1.0);
  path2->addPoint( 0.0, 3.0);
  path2->addPoint( 4.0, 3.0);
  path3->addPoint( 0.0, 0.0);
  path3->addPoint( 1.0, 1.0);
  F2CSwath swath1(path1);
  F2CSwath swath2(path2);
  F2CSwath swath3(path3);
  F2CSwath swath4;

  EXPECT_EQ(swath1.length(), 4);
  EXPECT_EQ(swath2.length(), 4);
  EXPECT_NEAR(std::pow(swath3.length(),2), 2, 1e-7);

  swath4.setPath(path2);
  EXPECT_EQ(swath4.length(), 4);
}
TEST(fields2cover_types_swath, set_id) {
  F2CSwath swath;
  swath.setId(400);
  EXPECT_EQ(swath.getId(), 400);
}

TEST(fields2cover_types_swath, width) {
  F2CSwath swath(5.0);
  EXPECT_EQ(swath.getWidth(), 5.0);

  F2CSwath swath2(0.2);
  EXPECT_EQ(swath2.getWidth(), 0.2);

  F2CLineString path;
  path->addPoint(0.0, 1.0);
  path->addPoint(4.0, 1.0);
  EXPECT_THROW(F2CSwath(path, -10), std::invalid_argument);
  EXPECT_THROW(F2CSwath(0), std::invalid_argument);
  EXPECT_THROW(F2CSwath(-10), std::invalid_argument);
}


TEST(fields2cover_types_swath, area) {
  F2CSwath swath_empty(F2CLineString(), 1.0);
  EXPECT_TRUE(swath_empty.areaCovered().isEmpty());

  F2CLineString line {F2CPoint(0, 1), F2CPoint(4, 1)};
  F2CSwath swath(line, 2.0);
  auto cell = swath.areaCovered();
  EXPECT_EQ(swath.getWidth() * swath.length(), cell.getGeometry(0).area());
  EXPECT_EQ(swath.getWidth() * swath.length(), swath.area());

  F2CCells cells = F2CCells::buffer(line, 1.0);
  EXPECT_NEAR(swath.area(cells), cells.area(), 1e-5);
}


TEST(fields2cover_types_swath, angles) {
  F2CLineString path1, path2, path3;
  path1->addPoint( 0.0, 1.0);
  path1->addPoint( 4.0, 1.0);
  path2->addPoint( 3.0, 1.0);
  path2->addPoint( 3.0, 4.0);
  path3->addPoint( 0.0, 0.0);
  path3->addPoint( 1.0, 1.0);
  F2CSwath swath_empty;
  F2CSwath swath1(path1);
  F2CSwath swath2(path2);
  F2CSwath swath3(path3);

  EXPECT_EQ(swath_empty.getInAngle(), -1.0);
  EXPECT_EQ(swath_empty.getOutAngle(), -1.0);
  auto empty_p1 = swath_empty.startPoint();
  auto empty_p2 = swath_empty.endPoint();
  EXPECT_TRUE(empty_p1.isEmpty());
  EXPECT_TRUE(empty_p2.isEmpty());
  EXPECT_NEAR(fmod(swath1.getInAngle(),boost::math::constants::two_pi<double>()), 0.0, 1e-5);
  EXPECT_NEAR(fmod(swath2.getInAngle(),boost::math::constants::two_pi<double>()), boost::math::constants::half_pi<double>(), 1e-5);
  EXPECT_NEAR(fmod(swath3.getInAngle(),boost::math::constants::two_pi<double>()), boost::math::constants::pi<double>() / 4.0, 1e-5);
  EXPECT_NEAR(fmod(swath1.getInAngle(),boost::math::constants::two_pi<double>()), fmod(swath1.getOutAngle(),boost::math::constants::two_pi<double>()), 1e-5);
  EXPECT_NEAR(fmod(swath2.getInAngle(),boost::math::constants::two_pi<double>()), fmod(swath2.getOutAngle(),boost::math::constants::two_pi<double>()), 1e-5);
  EXPECT_NEAR(fmod(swath3.getInAngle(),boost::math::constants::two_pi<double>()), fmod(swath3.getOutAngle(),boost::math::constants::two_pi<double>()), 1e-5);
}

TEST(fields2cover_types_swath, start_and_end_points) {
  F2CLineString path1;
  path1->addPoint( -100.0, 30.0);
  path1->addPoint( 4.0, 24.2);
  path1->addPoint( 50.0, -34.2);
  F2CSwath swath1(path1);

  EXPECT_NEAR(swath1.startPoint().getX(), -100.0, 1e-5);
  EXPECT_NEAR(swath1.startPoint().getY(), 30.0, 1e-5);
  EXPECT_NEAR(swath1.endPoint().getX(), 50.0, 1e-5);
  EXPECT_NEAR(swath1.endPoint().getY(), -34.2, 1e-5);
}

