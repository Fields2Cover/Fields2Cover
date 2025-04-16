//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
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

double BruteForce::getStepAngle() const {
  return this->step_angle;
}

void BruteForce::setStepAngle(double d) {
  this->step_angle = d;
}

double BruteForce::computeBestAngle(const f2c::obj::SGObjective& obj,
    double op_width, const F2CCell& poly, SwathOverlapType dist) const {
  int n = static_cast<int>(
      boost::math::constants::two_pi<double>() / step_angle);
  std::vector<double> costs(n);
  std::vector<int> ids(n);
  std::iota(ids.begin(), ids.end(), 0);

  auto getCostSwaths = [this, op_width, &poly, &obj, dist] (const int& i) {
    return computeCostOfAngle(obj, i * step_angle, op_width, poly, dist);
  };

  #ifdef ALLOW_PARALLELIZATION
    std::transform(std::execution::par_unseq, ids.begin(), ids.end(),
        costs.begin(), getCostSwaths);
  #else
    std::transform(ids.begin(), ids.end(), costs.begin(), getCostSwaths);
  #endif

  return ids[std::min_element(
      costs.begin(), costs.end()) - costs.begin()] * step_angle;
}

double BruteForce::computeBestAngle(const f2c::obj::SGObjective& obj,
    const std::vector<double>& widths, const F2CCell& poly) const {
  int n = static_cast<int>(
      boost::math::constants::two_pi<double>() / step_angle);
  std::vector<double> costs(n);
  std::vector<int> ids(n);
  std::iota(ids.begin(), ids.end(), 0);

  auto getCostSwaths = [this, widths, &poly, &obj] (const int& i) {
    return computeCostOfAngle(obj, i * step_angle, widths, poly);
  };

  #ifdef ALLOW_PARALLELIZATION
    std::transform(std::execution::par_unseq, ids.begin(), ids.end(),
        costs.begin(), getCostSwaths);
  #else
    std::transform(ids.begin(), ids.end(), costs.begin(), getCostSwaths);
  #endif

  return ids[std::min_element(
      costs.begin(), costs.end()) - costs.begin()] * step_angle;
}

}  // namespace f2c::sg

