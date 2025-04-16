//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "../test_helpers/path_planning_checker.hpp"

F2CPathStateSimp state_checker(F2CPathStateSimp ps,
    double theta, double kurv, double sigma, double len,
    double end_x, double end_y, double end_ang, double end_kurv) {
  ps.start.p = F2CPoint(1, 1);
  ps.len = len;
  ps.start.theta = theta;
  ps.start.kurv = kurv;
  ps.sigma = sigma;
  EXPECT_NEAR(ps.atEnd().p.getX(), end_x, 1e-4);
  EXPECT_NEAR(ps.atEnd().p.getY(), end_y, 1e-4);
  EXPECT_NEAR(F2CPoint::mod_2pi(ps.atEnd().theta), F2CPoint::mod_2pi(end_ang), 1e-4);
  EXPECT_NEAR(ps.atEnd().kurv, end_kurv, 1e-4);
  return ps;
}

TEST(fields2cover_types_pathSimpState, isDirFront) {
  F2CPathStateSimp ps;
  ps.len = 10.0;
  EXPECT_TRUE(ps.isDirFront());
  ps.len = -10.0;
  EXPECT_FALSE(ps.isDirFront());
}


TEST(fields2cover_types_pathSimpState, atEnd_STRAIGHT) {
  F2CPathStateSimp ps;
  ps.type = f2c::types::SectionSimp::STRAIGHT;
  state_checker(ps, M_PI * 0.5, 0, 0,  2,  1,  3, M_PI/2, 0);
  state_checker(ps, M_PI      , 0, 0,  3, -2,  1, M_PI, 0);
  state_checker(ps, M_PI * 0.5, 0, 0, -2,  1, -1, M_PI/2, 0);
  state_checker(ps, M_PI      , 0, 0, -3,  4,  1, M_PI, 0);
}


TEST(fields2cover_types_pathSimpState, atEnd_CIRCLE) {
  F2CPathStateSimp ps;
  ps.type = f2c::types::SectionSimp::CIRCLE;
  state_checker(ps, M_PI/2.,  1./4., 0,  4.*M_PI/2., -3,  5, M_PI,  1./4.);
  state_checker(ps, M_PI/2.,  1./4., 0, -4.*M_PI/2., -3, -3, 0,     1./4.);
  state_checker(ps, M_PI/2., -1./4., 0,  4.*M_PI/2.,  5,  5, 0,    -1./4.);
  state_checker(ps, M_PI/2., -1./4., 0, -4.*M_PI/2.,  5, -3, M_PI, -1./4.);
}


TEST(fields2cover_types_pathSimpState, atEnd_CLOTHOID) {
  F2CPathStateSimp ps;
  ps.type = f2c::types::SectionSimp::CLOTHOID;
  auto cloth_checker = [&ps] (double ang, double kurv, double sigma, double len,
      double x_end_pos, double y_end_pos, double x_end_neg, double y_end_neg,
      double ang_end1, double ang_end2,
      double k_end) {
    state_checker(ps, ang, kurv, sigma,  len,
        x_end_neg, y_end_pos, ang_end1, k_end);
    state_checker(ps, ang, kurv, sigma,  -len,
        x_end_pos, y_end_neg, ang_end1, -k_end);
    state_checker(ps, ang, kurv, -sigma,  len,
        x_end_pos, y_end_pos, ang_end2, -k_end);
    state_checker(ps, ang, kurv, -sigma,  -len,
        x_end_neg, y_end_neg, ang_end2, k_end);
  };
  cloth_checker(M_PI/2., 0.0, 0.1, 2*M_PI,
      4.11692, 5.23897, -2.11692, -3.23898,
      3.54471, 5.88006, 0.628318);
}


TEST(fields2cover_types_pathSimpState, atEnd_BUFF_CLOTHOID) {
  F2CPathStateSimp ps;
  ps.type = f2c::types::SectionSimp::BUFF_CLOTHOID;

  auto cloth_checker = [&ps] (double ang, double kurv, double sigma, double len,
      double x_end_pos, double y_end_pos, double x_end_neg, double y_end_neg,
      double ang_end1, double ang_end2,
      double k_end) {
    state_checker(ps, ang, kurv, sigma,  len,
        x_end_neg, y_end_pos, ang_end1, k_end);
    state_checker(ps, ang, kurv, sigma,  -len,
        x_end_pos, y_end_neg, ang_end1, -k_end);
    state_checker(ps, ang, kurv, -sigma,  len,
        x_end_pos, y_end_pos, ang_end2, -k_end);
    state_checker(ps, ang, kurv, -sigma,  -len,
        x_end_neg, y_end_neg, ang_end2, k_end);
  };
  ps.dist_cloth = 0.0;
  cloth_checker(M_PI/2., 0.0, 0.1, 2*M_PI,
      4.11692, 5.23897, -2.11692, -3.23898,
      3.54471, 5.88006, 0.628318);

  ps.dist_cloth = 2.0;
  cloth_checker(M_PI/2., 0.0, 0.1, 2*M_PI,
      6.90151, 7.07865, -4.90151, -5.07866,
      3.54471, 5.88006, 0.27843);
}

TEST(fields2cover_types_pathSimpState, reversePath) {
  F2CPathStateSimp ps;
  ps.type = f2c::types::SectionSimp::BUFF_CLOTHOID;
  ps.start.p = F2CPoint(1, 1);
  ps.len = -10.0;
  ps.start.theta = M_PI / 6.0;
  ps.start.kurv = -1.0;
  ps.sigma = 0.2;
  ps.dist_cloth = 0.2;

  ps.reversePath();
  EXPECT_EQ(ps.atEnd().p, F2CPoint(1, 1));
  EXPECT_NEAR(ps.atEnd().theta, M_PI + M_PI / 6.0, 1e-7);
  EXPECT_NEAR(ps.atEnd().kurv, 1.0, 1e-7);
  EXPECT_EQ(ps.sigma, 0.2);
  EXPECT_EQ(ps.len, -10);
}
