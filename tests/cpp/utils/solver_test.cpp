//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/utils/solver.h"
#include "fields2cover/types.h"


TEST(fields2cover_utils_Solver, siman_solve) {
  f2c::Random rand (42);
  std::vector<double> res = f2c::Solver::siman_solve<std::vector<double>>(rand, {15.5, 15.5},
      [] (const std::vector<double>& x) {
          return exp(-pow((x[0] * x[1] - 1.0), 2.0)) * sin(8* x[0] * x[1]);
      },
      [] (f2c::Random& r, const std::vector<double>& x, double step_size) {
          std::vector<double> new_x (x.size());
          for (size_t i = 0; i < x.size(); ++i) {
            new_x[i] = r.getRandomLinear(-step_size, step_size) + x[i];
          }
          return new_x;
      }, 10000);
  EXPECT_GT(res[0] * res[1], 1.0);
  EXPECT_LT(res[0] * res[1], 1.5);
}

TEST(fields2cover_utils_Solver, find_min_golden_selection) {
  auto f = [] (double x) {return pow(x - 0.5, 2.0);};
  double t = f2c::Solver::findMinGoldenSection(f, 0.0, 30.0, 1e-10);
  EXPECT_NEAR(t, 0.5, 1e-5);
}

