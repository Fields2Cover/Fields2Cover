//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/sg_obj/field_coverage.h"

namespace f2c::obj {

double FieldCoverage::computeCost(
    const F2CCell& poly, const F2CSwaths& swaths) {
  return computeCost(F2CCells(poly), swaths);
}

double FieldCoverage::computeCost(
    const F2CCells& poly, const F2CSwaths& swaths) {
  F2CMultiLineString lines;
  for (const auto& s : swaths) {
    lines.addGeometry(s.getPath());
  }

  double area_covered {poly.getCellsInside(
      F2CCells::buffer(lines, swaths[0].getWidth() / 2.0)).area()};
  return area_covered / poly.area();
}

bool FieldCoverage::isMinimizing() const {
  return false;
}

}  // namespace f2c::obj

