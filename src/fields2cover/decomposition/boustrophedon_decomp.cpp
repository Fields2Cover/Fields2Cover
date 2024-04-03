//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/decomposition/boustrophedon_decomp.h"

namespace f2c::decomp {

F2CMultiLineString BoustrophedonDecomp::genSplitLines(
    const F2CCells& cells, const obj::DecompObjective& obj) {
  F2CMultiLineString lines;
  for (auto&& cell : cells) {
    for (auto&& ring : cell) {
      for (auto&& p : ring) {
        auto line1 = cells.createLineUntilBorder(p, split_angle);
        auto line2 = cells.createLineUntilBorder(p, split_angle + M_PI);
        if (line1.length() > 1e-5 && line2.length() > 1e-5) {
          lines.addGeometry(line1);
          lines.addGeometry(line2);
        }
      }
    }
  }
  return lines;
}

}  // namespace f2c::decomp

