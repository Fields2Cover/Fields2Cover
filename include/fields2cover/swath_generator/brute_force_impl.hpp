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

namespace f2c {
namespace sg {

template <typename T>
F2CSwaths BruteForce<T>::generateBestSwaths(
    double _op_width, const F2CCell& _poly) {
  F2CSwaths best_swaths;
  double cost {0.0}, best_cost {std::numeric_limits<double>::max()};
  for (double i = 0.0; i < boost::math::constants::two_pi<double>();
      i += step_angle) {
    auto swaths = BruteForce<T>::generateSwaths(i, _op_width, _poly);
    cost = this->computeCostWithMinimizingSign(_poly, swaths);
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
    double _op_width, const F2CCells& _polys) {
  F2CSwathsByCells swaths;
  for (const auto& poly : _polys) {
    swaths.emplace_back(generateBestSwaths(_op_width, poly));
  }
  return swaths;
}


}  // namespace sg
}  // namespace f2c

#endif  // FIELDS2COVER_SWATH_GENERATOR_BRUTE_FORCE_IMPL_HPP_
