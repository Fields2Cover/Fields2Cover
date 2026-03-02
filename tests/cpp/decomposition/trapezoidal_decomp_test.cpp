//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <fstream>
#include "fields2cover/decomposition/trapezoidal_decomp.h"
#include "fields2cover/objectives/decomp_obj/min_sum_altitude.h"
#include "fields2cover/utils/random.h"
#include "fields2cover/types.h"

TEST(fields2cover_decomp_trapezoidal, decompose) {
  f2c::Random rand(42);
  F2CCells non_convex_field {rand.genNonConvexCell(1e3)};
  F2CCells convex_field {rand.genConvexCell(1e2)};
  F2CCells cells = non_convex_field.difference(convex_field);
  double cells_area = cells.area();

  f2c::decomp::TrapezoidalDecomp decomp;

  auto decomp_lines = decomp.genSplitLines(cells, 0.5*M_PI);
  auto decomp_field = decomp.decompose(cells, 0.5*M_PI);
  EXPECT_EQ(decomp_field.size(), 13);
  EXPECT_NEAR(decomp_field.area(), cells.area(), 1e-3);

  f2c::obj::MinSumAltitude obj;
  auto decomp_field2 = decomp.decompose(cells, obj);
  EXPECT_EQ(decomp_field2.size(), 13);
  EXPECT_NEAR(decomp_field2.area(), cells.area(), 1e-3);

  EXPECT_DOUBLE_EQ(decomp.decompose(F2CCells(), obj).area(), 0.0);
}

