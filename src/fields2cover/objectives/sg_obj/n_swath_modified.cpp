//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/sg_obj/n_swath_modified.h"

namespace f2c::obj {

double NSwathModified::computeCost(
    double ang, double op_width, const F2CCell& cell) {
  if (cell.isEmpty() || op_width <= 0.0) {
    return 0.0;
  }
  double n_turns {0.0};
  for (auto&& ring : cell) {
    for (size_t i = 0; i < ring.size(); ++i) {
      auto p = ring.getGeometry(i);
      auto p_1 = ring.getGeometry((i - 1) % ring.size());
      n_turns += p.distance(p_1) *
        fabs(sin(ang - (p - p_1).getAngleFromPoint()));
    }
  }
  return n_turns / (2.0 * op_width);
}

}  // namespace f2c::obj

