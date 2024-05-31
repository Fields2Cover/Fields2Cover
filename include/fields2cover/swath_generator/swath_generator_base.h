//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_H_
#define FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_H_

#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/objectives/sg_obj/sg_objective.h"

namespace f2c::sg {

class SwathGeneratorBase {
 public:
  bool getAllowOverlap() const;
  void setAllowOverlap(bool);

  virtual F2CSwaths generateBestSwaths(f2c::obj::SGObjective& obj,
      double op_width, const F2CCell& poly);

  virtual F2CSwathsByCells generateBestSwaths(f2c::obj::SGObjective& obj,
      double op_width, const F2CCells& polys);

  virtual F2CSwaths generateSwaths(double angle,
      double op_width, const F2CCell& poly);

  virtual F2CSwathsByCells generateSwaths(double angle,
      double op_width, const F2CCells& polys);

  virtual double computeCostOfAngle(f2c::obj::SGObjective& obj,
      double ang, double op_width, const F2CCell& poly);

  virtual double computeBestAngle(f2c::obj::SGObjective& obj,
      double op_width, const F2CCell& poly) = 0;

  virtual ~SwathGeneratorBase() = default;

 protected:
  bool allow_overlap {false};
};

}  // namespace f2c::sg


#endif  // FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_H_
