//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"
#include "fields2cover/objectives/pp_objective.h"

TEST(fields2cover_obj_pp_obj, isMinimizing) {
  f2c::obj::PPObjective obj;
  EXPECT_TRUE(obj.isMinimizing());
  EXPECT_FALSE(obj.isMaximizing());
}

