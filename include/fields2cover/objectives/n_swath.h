//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_N_SWATH_H_
#define FIELDS2COVER_OBJECTIVES_N_SWATH_H_

#include "fields2cover/types.h"
#include "fields2cover/objectives/global_objective.h"

namespace f2c {
namespace obj {

/// @brief Global cost function as the number of swaths.
///
/// This cost function assumes that turn are slower than going through the
/// swaths. Then, less swaths means less turns and faster paths.
class NSwath : public GlobalObjective {
 public:
  using GlobalObjective::computeCost;
  double computeCost(const F2CSwath& s) const override;
  double computeCost(const F2CSwaths& swaths) const override;
};

}  // namespace obj
}  // namespace f2c

#endif  // FIELDS2COVER_OBJECTIVES_N_SWATH_H_
