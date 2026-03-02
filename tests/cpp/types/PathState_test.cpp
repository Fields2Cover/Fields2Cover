//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_pathState, atEnd) {
  F2CPathState ps;
  ps.point    = F2CPoint(1, 1);
  ps.angle    = 0.0;
  ps.velocity = 2.0;
  ps.len      = 4.0;
  ps.dir      = f2c::types::PathDirection::FORWARD;
  ps.type     = f2c::types::PathSectionType::SWATH;
  EXPECT_EQ(ps.atEnd(), F2CPoint(5,1));

  ps.angle    = M_PI*0.5;
  EXPECT_NEAR(ps.atEnd().getX(), 1, 1e-7);
  EXPECT_NEAR(ps.atEnd().getY(), 5, 1e-7);

  ps.dir      = f2c::types::PathDirection::BACKWARD;
  EXPECT_NEAR(ps.atEnd().getX(),  1, 1e-7);
  EXPECT_NEAR(ps.atEnd().getY(), -3, 1e-7);

  ps.len      = -4.0;
  EXPECT_NEAR(ps.atEnd().getX(),  1, 1e-7);
  EXPECT_NEAR(ps.atEnd().getY(),  5, 1e-7);
}

TEST(fields2cover_types_pathState, toPathState) {
  F2CPoint p_start(10.0, -10.0);
  F2CPoint p_end(20.0, 0.0);

  F2CPathState ps1 = F2CPathState::toPathState(p_start, p_end);

  EXPECT_EQ(ps1.point, p_start);
  EXPECT_EQ(ps1.atEnd(), p_end);
  EXPECT_NEAR(ps1.len, p_start.distance(p_end), 1e-7);
  EXPECT_NEAR(ps1.angle, M_PI/4, 1e-7);
}


