//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/objectives/optimization_class.h"
#include "fields2cover/objectives/n_swath.h"
#include "fields2cover/types.h"

TEST(fields2cover_obj_optimization_class, computeCost_cost) { 
  F2CSwaths swaths50 {F2CSwaths(50)};

  f2c::obj::OptimizationClass<f2c::obj::NSwath> opt;

  EXPECT_EQ(opt.computeCost(swaths50), 50.0);
  swaths50.emplace_back(F2CSwath());
  EXPECT_EQ(opt.computeCost(swaths50), 51.0);
}
