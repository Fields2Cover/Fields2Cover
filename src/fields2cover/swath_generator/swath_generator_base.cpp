//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/swath_generator/swath_generator_base.h"

namespace f2c::sg {

bool SwathGeneratorBase::getAllowOverlap() const {
  return this->dist_type != SwathOverlapType::NO_OVERLAP;
}

void SwathGeneratorBase::setAllowOverlap(bool value) {
  this->dist_type = value ?
    SwathOverlapType::MIDDLE_OVERLAP : SwathOverlapType::NO_OVERLAP;
}

SwathOverlapType SwathGeneratorBase::getOverlapType() const {
  return this->dist_type;
}

void SwathGeneratorBase::setOverlapType(SwathOverlapType dist) {
  this->dist_type = dist;
}

F2CSwaths SwathGeneratorBase::generateBestSwaths(
    const f2c::obj::SGObjective& obj, double op_width,
    const F2CCell& poly) const {
  return generateBestSwaths(obj, op_width, poly, this->dist_type);
}

std::vector<F2CSwaths> SwathGeneratorBase::generateBestSwaths(
    const f2c::obj::SGObjective& obj, const std::vector<double>& widths, const F2CCell& poly) const {
  return generateSwaths(computeBestAngle(obj, widths, poly), widths, poly);
}

F2CSwaths SwathGeneratorBase::generateBestSwaths(
    const f2c::obj::SGObjective& obj, double op_width, const F2CCell& poly,
    SwathOverlapType dist) const {
  return generateSwaths(
      computeBestAngle(obj, op_width, poly, dist), op_width, poly, dist);
}

F2CSwathsByCells SwathGeneratorBase::generateBestSwaths(
    const f2c::obj::SGObjective& obj, double op_width,
    const F2CCells& polys) const {
  return generateBestSwaths(obj, op_width, polys, this->dist_type);
}

std::vector<F2CSwathsByCells> SwathGeneratorBase::generateBestSwaths(
    const f2c::obj::SGObjective& obj, const std::vector<double>& widths, const F2CCells& polys) const {
  std::vector<F2CSwathsByCells> swaths(widths.size());
  for (auto&& p : polys) {
    std::vector<F2CSwaths> v_poly_swaths = generateBestSwaths(obj, widths, p);
    for (int i = 0; i < widths.size(); ++i) {
      swaths[i].emplace_back(v_poly_swaths[i]);
    }
  }
  return swaths;
}


F2CSwathsByCells SwathGeneratorBase::generateBestSwaths(
    const f2c::obj::SGObjective& obj, double op_width, const F2CCells& polys,
    SwathOverlapType dist) const {
  F2CSwathsByCells swaths;
  for (auto&& p : polys) {
    swaths.emplace_back(generateBestSwaths(obj, op_width, p, dist));
  }
  return swaths;
}

F2CSwathsByCells SwathGeneratorBase::generateSwaths(double angle,
    double op_width, const F2CCells& polys) const {
  return generateSwaths(angle, op_width, polys, this->dist_type);
}

F2CSwathsByCells SwathGeneratorBase::generateSwaths(double angle,
    double op_width, const F2CCells& polys,
    SwathOverlapType dist) const {
  return generateSwaths(std::vector<double>(polys.size(), angle),
      op_width, polys, dist);
}

F2CSwathsByCells SwathGeneratorBase::generateSwaths(
    const std::vector<double>& angles, double op_width, const F2CCells& polys,
    SwathOverlapType dist) const {
  F2CSwathsByCells swaths;
  if (polys.size() != angles.size()) {
    throw std::invalid_argument(
        "Error on SwathGeneratorBase::generateSwaths:"
        "angles should have the same size as polys");
  }
  for (size_t i = 0; i < polys.size(); ++i) {
    swaths.emplace_back(generateSwaths(angles[i], op_width, polys[i], dist));
  }
  return swaths;
}

std::vector<F2CSwathsByCells> SwathGeneratorBase::generateSwaths(double angle,
    const std::vector<double>& widths, const F2CCells& polys) const {
  std::vector<F2CSwathsByCells> swaths(widths.size());
  for (auto&& p : polys) {
    auto v_s = generateSwaths(angle, widths, p);
    for (int i = 0; i < v_s.size(); ++i) {
      swaths[i].emplace_back(v_s[i]);
    }
  }
  return swaths;
}

F2CSwaths SwathGeneratorBase::generateSwaths(double angle,
    double op_width, const F2CCell& poly) const {
  return generateSwaths(angle, op_width, poly, this->dist_type);
}

F2CSwaths SwathGeneratorBase::generateSwaths(double angle,
    double op_width, const F2CCell& poly, SwathOverlapType dist) const {
  auto rot_poly {F2CPoint(0.0, 0.0).rotateFromPoint(-angle + 0.5 * M_PI, poly)};

  double field_width {rot_poly.getWidth()};
  F2CPoint min_point(rot_poly.getDimMinX(), rot_poly.getDimMinY());
  auto seed_curve = rot_poly.createStraightLongLine(min_point, 0.5 * M_PI);

  F2CMultiLineString paths;
  auto dist_x = getSwathsDistribution(dist, field_width, op_width);

  for (double d : dist_x) {
    paths.addGeometry(seed_curve + F2CPoint(d, 0.0));
  }
  paths = F2CPoint(0.0, 0.0).rotateFromPoint(angle - 0.5 * M_PI, paths);

  F2CSwaths swaths;
  swaths.append(paths, poly, op_width, f2c::types::SwathType::MAINLAND);
  return swaths;
}

std::vector<F2CSwaths> SwathGeneratorBase::generateSwaths(double angle,
    const std::vector<double>& widths, const F2CCell& poly) const {
  auto rot_poly {F2CPoint(0.0, 0.0).rotateFromPoint(-angle, poly)};

  double field_height {rot_poly.getHeight()};
  F2CPoint min_point(rot_poly.getDimMinX(), rot_poly.getDimMinY());
  auto seed_curve = rot_poly.createStraightLongLine(min_point, 0.0);

  std::vector<F2CSwaths> v_swaths(widths.size());
  int w_idx {0};
  double prev_w {0.0};
  double curve_y {0.0};
  while (field_height > curve_y + (getAllowOverlap() ? 0.0 : 0.5 * widths[w_idx])) {
    double w = widths[w_idx];
    curve_y += 0.5 * (w + prev_w);
    auto path = F2CPoint(0.0, 0.0).rotateFromPoint(angle,
        seed_curve + F2CPoint(0.0, curve_y));
    v_swaths[w_idx].append(path, poly, w, f2c::types::SwathType::MAINLAND);
    w_idx = (w_idx + 1) % widths.size();
    prev_w = w;
  }
  return v_swaths;
}



double SwathGeneratorBase::computeCostOfAngle(const f2c::obj::SGObjective& obj,
    double ang, double op_width, const F2CCell& poly,
    SwathOverlapType dist) const {
  if (obj.isFastCompAvailable()) {
    return obj.computeCostWithMinimizingSign(ang, op_width, poly);
  }
  return obj.computeCostWithMinimizingSign(poly,
      generateSwaths(ang, op_width, poly, dist));
}

double SwathGeneratorBase::computeCostOfAngle(const f2c::obj::SGObjective& obj,
    double ang, const std::vector<double>& widths, const F2CCell& poly) const {
  return obj.computeCostWithMinimizingSign(
      poly, generateSwaths(ang, widths, poly));
}

std::vector<double> SwathGeneratorBase::computeBestAngles(
    const f2c::obj::SGObjective& obj,
    double width, const F2CCells& polys, SwathOverlapType dist) const {
  std::vector<double> angs;
  for (auto&& p : polys) {
    angs.emplace_back(computeBestAngle(obj, width, p, dist));
  }
  return angs;
}


std::vector<double> SwathGeneratorBase::getSwathsDistribution(
    SwathOverlapType type, double field_width, double cov_width) {
  size_t n = static_cast<size_t>(std::ceil(fabs(field_width / cov_width)));
  if (type == SwathOverlapType::MIDDLE_OVERLAP ||
      type == SwathOverlapType::EVENLY_DISTRIBUTED_OVERLAP) {
    ++n;
  }
  if (n < 1) {
    return {};
  }

  std::vector<double> sx(n);
  sx[0] = 0.5 * cov_width;
  double overlap = type == SwathOverlapType::EVENLY_DISTRIBUTED_OVERLAP ?
    (1 - (field_width - cov_width) / (cov_width * (n - 1))) : 0.0;
  double not_overlap_width = cov_width * (1 - overlap);

  for (size_t i = 1; i < n; ++i) {
    sx[i] = sx[i-1] + not_overlap_width;
  }

  if (type == SwathOverlapType::END_OVERLAP) {
    sx.emplace_back(field_width - 0.5 * cov_width);
  }
  return sx;
}

}  // namespace f2c::sg

