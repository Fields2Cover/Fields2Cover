//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_N_SWATH_MODIFIED_H_
#define FIELDS2COVER_OBJECTIVES_N_SWATH_MODIFIED_H_

#include "fields2cover/types.h"
#include "fields2cover/objectives/sg_obj/n_swath.h"

namespace f2c::obj {

/// @brief SG cost function as the number of swaths \cite jin2010optimal.
///
/// This cost function assumes that turn are slower than going through the
/// swaths. Then, less swaths means less turns and faster paths.
class NSwathModified : public NSwath {
 public:
  using SGObjective::computeCost;
  bool isFastCompAvailable() const override {return true;}
  double computeCost(double ang, double op_width, const F2CCell& cell) override;
};

}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_N_SWATH_MODIFIED_H_
