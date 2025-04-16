//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/decomp_obj/min_sum_altitude.h"

namespace f2c::obj {

double MinSumAltitude::computeCost(const F2CLinearRing& ring) const {
  return ring.minAltitude();
}

}  // namespace f2c::obj

