//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <vector>
#include "fields2cover/swath_generator/curved_swaths.h"
#include "fields2cover/utils/visualizer.h"

namespace f2c::sg {

double CurvedSwathsGen::getThresholdAngle() const {
  return this->threshold_angle;
}

void CurvedSwathsGen::setThresholdAngle(double ang) {
  this->threshold_angle = ang;
}

double CurvedSwathsGen::computeBestAngle(const f2c::obj::SGObjective& obj,
    double op_width, const F2CCell& poly, SwathOverlapType dist) const {
  return computeBestAngle(obj, {op_width}, poly);
}

double CurvedSwathsGen::computeBestAngle(const f2c::obj::SGObjective& obj,
    const std::vector<double>& widths, const F2CCell& poly) const {
  auto edge = poly[0].getLongestEdge(this->threshold_angle);
  return (edge.endPoint() - edge.startPoint()).getAngleFromPoint();
}


F2CSwaths CurvedSwathsGen::generateBestSwaths(const f2c::obj::SGObjective& obj,
    double op_width, const F2CCell& poly, SwathOverlapType dist) const {
  return this->generateSwaths(0, op_width, poly, dist);
}

F2CSwaths CurvedSwathsGen::generateSwaths(double angle,
    double op_width, const F2CCell& poly, SwathOverlapType dist) const {
  auto edge = poly[0].getLongestEdge(this->threshold_angle);
  edge.extend(100.0 * edge.length()).filterSelfIntersections();

  double safe_dist = poly.getMinSafeLength();
  int n = static_cast<int>(safe_dist / op_width);

  F2CSwaths swaths;
  for (double i = -n - 0.5; i < n + 0.5; ++i) {
    swaths.append(
        edge.clone().getParallelLine(i * op_width).filterSelfIntersections(),
        poly, op_width);
  }
  return swaths;
}

}  // namespace f2c::sg

