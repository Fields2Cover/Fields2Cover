//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/swath_generator/longest_edge.h"

namespace f2c::sg {

double LongestEdge::computeBestAngle(const f2c::obj::SGObjective& obj,
    double op_width, const F2CCell& poly, SwathOverlapType dist) const {
  return computeBestAngle(obj, {op_width}, poly);
}

double LongestEdge::computeBestAngle(const f2c::obj::SGObjective& obj,
    const std::vector<double>& widths, const F2CCell& poly) const {
  if (poly.size() == 0 || poly[0].size() < 2) {
    return 0.0;
  }
  auto edge = poly[0].getLongestEdge(0.0);
  return edge.size() == 0 ? 0.0 : edge.endAngle();
}



}  // namespace f2c::sg

