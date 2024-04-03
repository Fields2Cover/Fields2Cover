//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/decomposition/trapezoidal_decomp.h"

namespace f2c::decomp {

void TrapezoidalDecomp::setSplitAngle(double ang) {
  this->split_angle = ang;
}

double TrapezoidalDecomp::getSplitAngle() const {
  return this->split_angle;
}

F2CMultiLineString TrapezoidalDecomp::genSplitLines(
    const F2CCells& cells, const obj::DecompObjective& obj) {
  F2CMultiLineString lines;
  for (auto&& cell : cells) {
    for (auto&& ring : cell) {
      for (auto&& p : ring) {
        lines.addGeometry(cells.createLineUntilBorder(
            p, split_angle));
        lines.addGeometry(cells.createLineUntilBorder(
            p, split_angle + M_PI));
      }
    }
  }
  return lines;
}

}  // namespace f2c::decomp

