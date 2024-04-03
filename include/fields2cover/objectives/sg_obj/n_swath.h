//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_N_SWATH_H_
#define FIELDS2COVER_OBJECTIVES_N_SWATH_H_

#include "fields2cover/types.h"
#include "fields2cover/objectives/sg_obj/sg_objective.h"

namespace f2c::obj {

/// @brief SG cost function as the number of swaths.
///
/// This cost function assumes that turn are slower than going through the
/// swaths. Then, less swaths means less turns and faster paths.
class NSwath : public SGObjective {
 public:
  using SGObjective::computeCost;
  double computeCost(const F2CSwath& s) override;
  double computeCost(const F2CSwaths& swaths) override;
};

}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_N_SWATH_H_
