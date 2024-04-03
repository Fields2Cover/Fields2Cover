//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_SWATH_LENGTH_H_
#define FIELDS2COVER_OBJECTIVES_SWATH_LENGTH_H_

#include <numeric>
#include "fields2cover/types.h"
#include "fields2cover/objectives/sg_obj/sg_objective.h"

namespace f2c::obj {

/// Global cost function as the length of the swaths
class SwathLength : public SGObjective {
 public:
  using SGObjective::computeCost;
  double computeCost(const F2CSwath& s) override;
};

}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_SWATH_LENGTH_H_
