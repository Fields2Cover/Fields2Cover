//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/sg_obj/polygon_altitude.h"

namespace f2c::obj {

double PolygonAltitude::computeCost(
    double ang, double op_width, const F2CCell& cell) const {
  if (cell.isEmpty()) {
    return 0.0;
  }
  auto rot_cell = F2CPoint(0.0, 0.0).rotateFromPoint(-ang, cell);
  double height {rot_cell.getHeight()};
  for (size_t i = 0; i < rot_cell.size() - 1; ++i) {
    height += rot_cell.getInteriorRing(i).getHeight();
  }
  return height;
}

}  // namespace f2c::obj

