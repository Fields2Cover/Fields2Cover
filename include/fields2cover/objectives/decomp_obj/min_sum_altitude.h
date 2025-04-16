//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_MIN_SUM_ALTUTIDE_H_
#define FIELDS2COVER_OBJECTIVES_MIN_SUM_ALTITUDE_H_

#include "fields2cover/types.h"
#include "fields2cover/objectives/decomp_obj/decomp_objective.h"

namespace f2c::obj {

/// Compute the sum of minimum altitude of polygons \cite{huang2001optimal}.
class MinSumAltitude : public DecompObjective {
 public:
  using DecompObjective::computeCost;
  double computeCost(const F2CLinearRing& ring) const override;
};

}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_MIN_SUM_ALTITUDE_H_
