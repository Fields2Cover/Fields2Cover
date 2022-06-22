//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <algorithm>
#include "fields2cover/route_planning/single_cell_swaths_order_base.h"

namespace f2c::rp {

SingleCellSwathsOrderBase::SingleCellSwathsOrderBase() = default;

SingleCellSwathsOrderBase::SingleCellSwathsOrderBase(F2CSwaths& swaths) {
  setSwaths(swaths);
}

SingleCellSwathsOrderBase::~SingleCellSwathsOrderBase() = default;

F2CSwaths& SingleCellSwathsOrderBase::genSortedSwaths() {
  // Always sort swaths to work with them in the same direction.
  swaths_.sort();
  sortSwaths();
  swaths_.reverseDirOddSwaths();
  ++counter_;
  return swaths_;
}

void SingleCellSwathsOrderBase::setSwaths(F2CSwaths& swaths) {
  swaths_ = swaths;
}

void SingleCellSwathsOrderBase::changeStartPoint() {
  if (counter_ & 1) {
    swaths_.reverse();
  }
  if (counter_ & 2) {
    swaths_[0].reverse();
  }
}



}  // namespace f2c::rp

