//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_SWATH_GENERATOR_BRUTE_FORCE_IMPL_HPP_
#define FIELDS2COVER_SWATH_GENERATOR_BRUTE_FORCE_IMPL_HPP_

#include <utility>
#include <limits>

namespace f2c::sg {

template <typename T>
F2CSwaths BruteForce<T>::generateBestSwaths(
    double op_width, const F2CCell& poly) {
  F2CSwaths best_swaths;
  double cost {0.0}, best_cost {std::numeric_limits<double>::max()};
  for (double i = 0.0; i < boost::math::constants::two_pi<double>();
      i += step_angle) {
    auto swaths = BruteForce<T>::generateSwaths(i, op_width, poly);
    cost = this->computeCostWithMinimizingSign(poly, swaths);
    if (cost < best_cost) {
      best_swaths = std::move(swaths);
      this->best_angle = i;
      best_cost = cost;
    }
  }
  return best_swaths;
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
