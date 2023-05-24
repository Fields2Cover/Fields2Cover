//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_H_
#define FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_H_

#include <memory>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <numeric>
#include "fields2cover/types.h"
#include "fields2cover/objectives/sg_objective.h"

namespace f2c::sg {

template <typename T>
class SwathGeneratorBase {
 public:
   bool allow_overlap {false};

 public:
  virtual F2CSwaths generateBestSwaths(f2c::obj::SGObjective& obj,
      double op_width, const F2CCell& poly) = 0;

  virtual F2CSwathsByCells generateBestSwaths(f2c::obj::SGObjective& obj,
      double op_width, const F2CCells& polys);

  virtual F2CSwaths generateSwaths(double angle,
      double op_width, const F2CCell& poly);

  virtual F2CSwathsByCells generateSwaths(double angle,
      double op_width, const F2CCells& polys);

  void setAllowOverlap(bool value) {allow_overlap = value;}
};

}  // namespace f2c::sg

#include "fields2cover/swath_generator/swath_generator_base_impl.hpp"


#endif  // FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_H_
