//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_FIELD_COVERAGE_H_
#define FIELDS2COVER_OBJECTIVES_FIELD_COVERAGE_H_

#include <utility>
#include <memory>
#include "fields2cover/types.h"
#include "fields2cover/objectives/global_objective.h"

namespace f2c {
namespace obj {

// Works only if all the swaths have same width
/// Global cost function as the percentage of the field covered
class FieldCoverage : public GlobalObjective {
 public:
  using GlobalObjective::computeCost;
  double computeCost(const F2CCell& poly, const F2CSwaths& swaths)
    const override;
  double computeCost(const F2CCells& poly, const F2CSwaths& swaths)
    const override;

 public:
  bool isMinimizing(void) const override;
};

}  // namespace obj
}  // namespace f2c

#endif  // FIELDS2COVER_OBJECTIVES_FIELD_COVERAGE_H_
