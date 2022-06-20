//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_SWATH_GENERATOR_BRUTE_FORCE_H_
#define FIELDS2COVER_SWATH_GENERATOR_BRUTE_FORCE_H_

#include <limits>
#include <utility>
#include <numeric>
#include <memory>
#include "fields2cover/types.h"
#include "fields2cover/swath_generator/swath_generator_base.h"

namespace f2c {
namespace sg {

template <typename T>
class BruteForce : public SwathGeneratorBase<T> {
 public:
  F2CSwaths generateBestSwaths(double op_width, const F2CCell& poly) override;
  F2CSwathsByCells generateBestSwaths(
      double op_width, const F2CCells& poly) override;

 public:
  double step_angle {boost::math::constants::degree<double>()};  // radians
};


}  // namespace sg
}  // namespace f2c

#include "fields2cover/swath_generator/brute_force_impl.hpp"


#endif  // FIELDS2COVER_SWATH_GENERATOR_BRUTE_FORCE_H_
