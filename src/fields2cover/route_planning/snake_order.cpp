//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/route_planning/snake_order.h"

namespace f2c::rp {

void SnakeOrder::sortSwaths(F2CSwaths& swaths) const {
  // A cell with one swath has nothing to reorder, and the arithmetic below
  // does not survive it: (size() - 1) / 2 + 1 leaves i == 1, so the reverse
  // starts one past the end. With no swath at all, size() - 1 wraps around.
  if (swaths.size() < 2) {
    return;
  }
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
