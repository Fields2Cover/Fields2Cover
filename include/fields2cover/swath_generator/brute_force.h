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

namespace f2c::sg {

class BruteForce : public SwathGeneratorBase<BruteForce> {
 public:
  F2CSwaths generateBestSwaths(f2c::obj::SGObjective& obj,
      double op_width, const F2CCell& poly) override;

 public:
  double step_angle {boost::math::constants::degree<double>()};  // radians
};


}  // namespace f2c::sg



#endif  // FIELDS2COVER_SWATH_GENERATOR_BRUTE_FORCE_H_
