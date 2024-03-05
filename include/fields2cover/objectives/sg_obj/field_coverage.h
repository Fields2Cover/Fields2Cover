//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_FIELD_COVERAGE_H_
#define FIELDS2COVER_OBJECTIVES_FIELD_COVERAGE_H_

#include <utility>
#include <memory>
#include "fields2cover/types.h"
#include "fields2cover/objectives/sg_obj/sg_objective.h"

namespace f2c::obj {

// Works only if all the swaths have same width
/// SG objective function as the percentage of the field covered
class FieldCoverage : public SGObjective {
 public:
  using SGObjective::computeCost;
  double computeCost(const F2CCell& poly, const F2CSwaths& swaths) override;
  double computeCost(const F2CCells& poly, const F2CSwaths& swaths) override;

 public:
  bool isMinimizing(void) const override;
};

}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_FIELD_COVERAGE_H_
