//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/route_planning/snake_order.h"

namespace f2c::rp {

SnakeOrder::SnakeOrder(F2CSwaths& swaths) : SingleCellSwathsOrderBase(swaths) {
}

void SnakeOrder::sortSwaths() {
  this->changeStartPoint();

  size_t i;
  for (i = 1; i < (swaths_.size() - 1) / 2 + 1; ++i) {
    std::rotate(swaths_.begin() + i, swaths_.begin() + i + 1, swaths_.end());
  }
  std::reverse(swaths_.begin() + i + 1, swaths_.end());
  if (swaths_.size() % 2 == 1) {
    std::rotate(swaths_.begin() + i, swaths_.begin() + i + 1, swaths_.end());
  }
}


}  // namespace f2c::rp

