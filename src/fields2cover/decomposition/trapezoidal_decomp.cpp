//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/decomposition/trapezoidal_decomp.h"

namespace f2c::decomp {

F2CMultiLineString TrapezoidalDecomp::genSplitLines(
    const F2CCells& cells, const obj::DecompObjective& obj) {
  F2CMultiLineString lines;
  for (auto&& cell : cells) {
    for (auto&& ring : cell) {
      for (auto&& p : ring) {
        lines.addGeometry(cells.createLineUntilBorder(
            p, split_angle + M_PI/2.0));
        lines.addGeometry(cells.createLineUntilBorder(
            p, split_angle - M_PI/2.0));
      }
    }
  }
  return lines;
}

}  // namespace f2c::decomp

