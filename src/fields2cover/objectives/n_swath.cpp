//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/n_swath.h"

namespace f2c {
namespace obj {

double NSwath::computeCost(const F2CSwath& s) const {
  return 1.0;
}

double NSwath::computeCost(const F2CSwaths& swaths) const {
  return swaths.size();
}

}  // namespace obj
}  // namespace f2c

