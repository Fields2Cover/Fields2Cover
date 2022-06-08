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
  }
  auto restored_v = F2CPoint::getAngContinuity(v);
  for (int i = 1; i < restored_v.size(); ++i) {
    EXPECT_NEAR(fabs(restored_v[i] - restored_v[i - 1]), 0.1, 1e-5);
  }
}




