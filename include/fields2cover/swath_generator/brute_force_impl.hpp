//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_SWATH_GENERATOR_BRUTE_FORCE_IMPL_HPP_
#define FIELDS2COVER_SWATH_GENERATOR_BRUTE_FORCE_IMPL_HPP_

#include <algorithm>
#ifdef ALLOW_PARALLELIZATION
#include <execution>
#endif
#include <vector>
#include <utility>
#include <limits>

namespace f2c::sg {

template <typename T>
F2CSwaths BruteForce<T>::generateBestSwaths(
    double op_width, const F2CCell& poly) {
  int n = static_cast<int>(
      boost::math::constants::two_pi<double>() / step_angle);
  std::vector<double> costs(n);
  std::vector<int> ids(n);
  std::iota(ids.begin(), ids.end(), 0);

  auto getCostSwaths = [this, op_width, &poly] (const int& i) {
    return this->computeCostWithMinimizingSign(poly,
      BruteForce<T>::generateSwaths(i * step_angle,
        op_width, poly));
  };

  #ifdef ALLOW_PARALLELIZATION
    std::transform(std::execution::par_unseq, ids.begin(), ids.end(),
        costs.begin(), getCostSwaths);
  #else
    std::transform(ids.begin(), ids.end(), costs.begin(), getCostSwaths);
  #endif

  int min_cost_pos = std::min_element(
      costs.begin(), costs.end()) - costs.begin();
  this->best_angle = ids[min_cost_pos] * step_angle;

  return BruteForce<T>::generateSwaths(this->best_angle, op_width, poly);
}

template <typename T>
F2CSwathsByCells BruteForce<T>::generateBestSwaths(
    double op_width, const F2CCells& polys) {
  F2CSwathsByCells swaths;
  for (const auto& poly : polys) {
    swaths.emplace_back(generateBestSwaths(op_width, poly));
  }
  return swaths;
}


}  // namespace f2c::sg

#endif  // FIELDS2COVER_SWATH_GENERATOR_BRUTE_FORCE_IMPL_HPP_
