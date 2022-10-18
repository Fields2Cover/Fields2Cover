//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <utility>
#include "fields2cover/headland_generator/constant_headland.h"

namespace f2c::hg {

F2CCells ConstHL::generateHeadlands(
    const F2CCells& field, double dist_headland) {
  F2CCells red_field = field.clone();
  for (auto&& poly : field) {
    for (auto&& ring : poly) {
      auto lines = F2CMultiLineString::getLineSegments(F2CLineString(ring));
      for (auto&& line : lines) {
        red_field = red_field.Difference(
              F2CCells::Buffer(line, dist_headland));
      }
    }
  }
  return red_field;
}


}  // namespace f2c::hg

