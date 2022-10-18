//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/route_planning/snake_order.h"

namespace f2c::rp {

void SnakeOrder::sortSwaths(F2CSwaths& swaths) const {
  size_t i;
  for (i = 1; i < (swaths.size() - 1) / 2 + 1; ++i) {
    std::rotate(swaths.begin() + i, swaths.begin() + i + 1, swaths.end());
  }
  std::reverse(swaths.begin() + i + 1, swaths.end());
  if (swaths.size() % 2 == 1) {
    std::rotate(swaths.begin() + i, swaths.begin() + i + 1, swaths.end());
  }
}


}  // namespace f2c::rp
