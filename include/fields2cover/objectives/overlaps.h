//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_OVERLAPS_H_
#define FIELDS2COVER_OBJECTIVES_OVERLAPS_H_

#include <utility>
#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/objectives/sg_objective.h"

namespace f2c::obj {

/// SG cost function as the percentage of the field overlapped
class Overlaps : public SGObjective {
 public:
  using SGObjective::computeCost;

  double computeCost(const F2CCell& poly, const F2CSwaths& swaths) override;
  double computeCost(const F2CCells& poly, const F2CSwaths& swaths) override;
};

}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_OVERLAPS_H_
