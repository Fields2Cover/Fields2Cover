//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <fstream>
#include "fields2cover/decomposition/trapezoidal_decomp.h"
#include "fields2cover/objectives/n_swath.h"
#include "fields2cover/utils/random.h"
#include "fields2cover/types.h"
#include "fields2cover/utils/visualizer.h"

TEST(fields2cover_decomp_trapezoidal, decompose) {
  f2c::Random rand(42);
  auto non_convex_field = rand.genNonConvexField(1e3);
  auto convex_field = rand.genNonConvexField(1e2);
  F2CCells cells = non_convex_field.field.Difference(convex_field.field);

  f2c::decomp::TrapezoidalDecomp decomp;
  auto decomp_lines = decomp.genSplitLines(cells);
  auto decomp_field = decomp.decompose(cells);
  EXPECT_EQ(decomp_field.size(), 18);
  EXPECT_NEAR(decomp_field.getArea(), cells.getArea(), 1e-3);

  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(decomp_field);
  f2c::Visualizer::plot(decomp_lines, "--r");
  f2c::Visualizer::show(false);
  */
}

