//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <algorithm>
#ifdef ALLOW_PARALLELIZATION
#include <execution>
#endif
#include <vector>
#include <utility>
#include <limits>
#include "fields2cover/swath_generator/brute_force.h"

namespace f2c::sg {

F2CSwaths BruteForce::generateBestSwaths(f2c::obj::SGObjective& obj,
    double op_width, const F2CCell& poly) {
  int n = static_cast<int>(
      boost::math::constants::two_pi<double>() / step_angle);
  std::vector<double> costs(n);
  std::vector<int> ids(n);
  std::iota(ids.begin(), ids.end(), 0);

  auto getCostSwaths = [this, op_width, &poly, &obj] (const int& i) {
    auto s = generateSwaths(i * step_angle, op_width, poly);
    return obj.computeCostWithMinimizingSign(poly, s);
  };

  #ifdef ALLOW_PARALLELIZATION
    std::transform(std::execution::par_unseq, ids.begin(), ids.end(),
        costs.begin(), getCostSwaths);
  #else
    std::transform(ids.begin(), ids.end(), costs.begin(), getCostSwaths);
  #endif

  int min_cost_pos = std::min_element(
      costs.begin(), costs.end()) - costs.begin();
  return generateSwaths(ids[min_cost_pos] * step_angle, op_width, poly);
}

}  // namespace f2c::sg

