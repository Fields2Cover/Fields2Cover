//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <utility>
#include "fields2cover/headland_generator/constant_headland.h"

namespace f2c::hg {

F2CCells ConstHL::generateHeadlands(
    const F2CCells& field, double dist_headland) const {
  return field.buffer(-dist_headland);
}

F2CCells ConstHL::generateHeadlandArea(
    const F2CCells& field, double swath_width, int n_swaths) const {
  return field.buffer(-swath_width * n_swaths);
}

std::vector<F2CMultiLineString> ConstHL::generateHeadlandSwaths(
    const F2CCells& field, double swath_width,
    int n_swaths, bool dir_out2in) const {
  std::vector<F2CCells> hl;
  if (dir_out2in) {
    for (int i = 0; i < n_swaths; ++i) {
      hl.emplace_back(field.buffer(-swath_width * (i + 0.5)));
    }
  } else {
    hl.emplace_back(field.buffer(-swath_width * (n_swaths - 0.5)));
    for (int i = 1; i < n_swaths; ++i) {
      hl.emplace_back(hl.back().buffer(swath_width));
    }
  }
  std::vector<F2CMultiLineString> hl_tracks;
  for (auto&& cells : hl) {
    hl_tracks.emplace_back(cells.getLineSections());
  }
  return hl_tracks;
}


}  // namespace f2c::hg

