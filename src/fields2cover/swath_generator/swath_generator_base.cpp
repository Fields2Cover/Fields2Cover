//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/swath_generator/swath_generator_base.h"
#include "fields2cover/utils/visualizer.h"
#include <cmath>

namespace f2c::sg {

bool SwathGeneratorBase::getAllowOverlap() const {
  if (this->overlap_type == SwathOverlapType::NO_OVERLAP) {
    return false;
  }
  return true;
}

void SwathGeneratorBase::setAllowOverlap(bool value) {
  if (value) {
    this->overlap_type = SwathOverlapType::EVENLY_DISTRIBUTED_OVERLAP;
  } else {
    this->overlap_type = SwathOverlapType::NO_OVERLAP;
  }
}

SwathOverlapType SwathGeneratorBase::getOverlapType() const {
  return this->overlap_type;
}

void SwathGeneratorBase::setOverlapType(SwathOverlapType type) {
  this->overlap_type = type;
}

F2CSwaths SwathGeneratorBase::generateBestSwaths(
    f2c::obj::SGObjective& obj, double op_width, const F2CCell& poly) {
  return generateSwaths(computeBestAngle(obj, op_width, poly), op_width, poly);
}

F2CSwathsByCells SwathGeneratorBase::generateBestSwaths(
    f2c::obj::SGObjective& obj, double op_width, const F2CCells& polys) {
  F2CSwathsByCells swaths;
  for (auto&& p : polys) {
    swaths.emplace_back(generateBestSwaths(obj, op_width, p));
  }
  return swaths;
}

F2CSwathsByCells SwathGeneratorBase::generateSwaths(double angle,
    double op_width, const F2CCells& polys) {
  F2CSwathsByCells swaths;
  for (auto&& p : polys) {
    swaths.emplace_back(generateSwaths(angle, op_width, p));
  }
  return swaths;
}

F2CSwaths SwathGeneratorBase::generateSwaths(double angle,
    double op_width, const F2CCell& poly) {
  auto rot_poly {F2CPoint(0.0, 0.0).rotateFromPoint(-angle, poly)};

  double field_height {rot_poly.getHeight()};
  F2CPoint min_point(rot_poly.getDimMinX(), rot_poly.getDimMinY());
  auto seed_curve = rot_poly.createStraightLongLine(min_point, 0.0);

  F2CMultiLineString paths;

  double increment = op_width;
  int num_paths = std::floor(field_height / op_width);
  double beginning_offset = 0.5 * op_width;
  double end_offset = 0;

  switch (overlap_type)
  {
  case SwathOverlapType::NO_OVERLAP:
    break;
  case SwathOverlapType::END_OVERLAP:
    end_offset = field_height - (0.5 * op_width);
    break;
  case SwathOverlapType::MIDDLE_OVERLAP:
    num_paths = std::floor(field_height / op_width);
    end_offset = field_height - (0.5 * op_width);
    op_width = (field_height - op_width) / num_paths;
    increment = op_width;
    break;
  case SwathOverlapType::EVENLY_DISTRIBUTED_OVERLAP:
    num_paths = std::floor(field_height / op_width);
    op_width = (field_height - op_width) / num_paths;
    end_offset = field_height - (0.5 * op_width);
    increment = op_width;
    break;
  default:
    break;
  }

  for (int i = 0; i < num_paths; i++) {
    double path_y = beginning_offset + (i * increment);
    paths.addGeometry(F2CPoint(0.0, 0.0).rotateFromPoint(angle,
        seed_curve + F2CPoint(0.0, path_y)));
  }

  if (end_offset != 0) {
    paths.addGeometry(F2CPoint(0.0, 0.0).rotateFromPoint(angle,
        seed_curve + F2CPoint(0.0, end_offset)));
  }

  F2CSwaths swaths;
  swaths.append(paths, poly, op_width, f2c::types::SwathType::MAINLAND);
  return swaths;
}


double SwathGeneratorBase::computeCostOfAngle(f2c::obj::SGObjective& obj,
    double ang, double op_width, const F2CCell& poly) {
  if (obj.isFastCompAvailable()) {
    return obj.computeCostWithMinimizingSign(ang, op_width, poly);
  }
  return obj.computeCostWithMinimizingSign(poly,
      generateSwaths(ang, op_width, poly));
}

double SwathGeneratorBase::computeBestAngle(f2c::obj::SGObjective& obj,
    double width, const F2CCell& poly) {
  return computeBestAngle(obj, width, poly);
}

}  // namespace f2c::sg

