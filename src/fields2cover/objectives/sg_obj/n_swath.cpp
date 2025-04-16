//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/sg_obj/n_swath.h"

namespace f2c::obj {

double NSwath::computeCost(const F2CSwath& s) const {
  return 1.0;
}

double NSwath::computeCost(const F2CSwaths& swaths) const {
  return swaths.size();
}

}  // namespace f2c::obj

