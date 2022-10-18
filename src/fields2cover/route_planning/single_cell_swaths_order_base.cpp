//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <algorithm>
#include "fields2cover/route_planning/single_cell_swaths_order_base.h"

namespace f2c::rp {

F2CSwaths SingleCellSwathsOrderBase::genSortedSwaths(
    const F2CSwaths& swaths, uint32_t variant) const {
  // Always sort swaths to work with them in the same direction.
  F2CSwaths new_swaths = swaths.clone();
  if (new_swaths.size() > 0) {
    new_swaths.sort();
    this->changeStartPoint(new_swaths, variant);
    this->sortSwaths(new_swaths);
    new_swaths.reverseDirOddSwaths();
  }
  return new_swaths;
}

void SingleCellSwathsOrderBase::changeStartPoint(
    F2CSwaths& swaths, uint32_t variant) const {
  if (variant & 1) {
    swaths.reverse();
  }
  if (variant & 2) {
    swaths[0].reverse();
  }
}



}  // namespace f2c::rp

