//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/decomposition/trapezoidal_decomp.h"

namespace f2c::decomp {

F2CCells TrapezoidalDecomp::decompose(
    const F2CCells& cells, const obj::DecompObjective& obj) const {
  auto split_cells = merge(split(cells, obj), obj);
  this->cov_angles = std::vector<double>(
      split_cells.size(), this->cov_angles[0]);
  return split_cells;
}

F2CCells TrapezoidalDecomp::decompose(
    const F2CCells& cells, double split_angle) const {
  auto split_cells = this->split(cells, split_angle);
  this->cov_angles = std::vector<double>(split_cells.size(), split_angle);
  return split_cells;
}

F2CCells TrapezoidalDecomp::split(
    const F2CCells& cells, double split_angle) const {
  return cells.splitByLine(genSplitLines(cells, split_angle));
}

F2CMultiLineString TrapezoidalDecomp::genSplitLines(
    const F2CCells& cells, const obj::DecompObjective& obj) const {
  double ang = this->computeMinDecompAngle(cells, obj);
  auto lines = genSplitLines(cells, ang);
  this->cov_angles = std::vector<double>(1, ang);
  return lines;
}

double TrapezoidalDecomp::computeMinDecompAngle(
    const F2CCells& cells, const obj::DecompObjective& obj) const {
  // TODO: Check this exception
  if (cells.area() <= 0.0) {
    return 0.0;
  }
  if (obj.computeCostWithMinimizingSign(cells) == 0) {
    throw std::out_of_range("Decomposition Objective always get 0");
  }

  double step_angle = M_PI/180.0;  // TODO: add it as parameter of class

  int n = static_cast<int>(
      boost::math::constants::two_pi<double>() / step_angle);
  std::vector<double> costs(n);
  std::vector<int> ids(n);
  std::iota(ids.begin(), ids.end(), 0);

  auto getCostSwaths = [this, &cells, step_angle, &obj] (const int& i) {
    auto split_cells = merge(cells.splitByLine(
          genSplitLines(cells, i * step_angle)), obj);
    return obj.computeCostWithMinimizingSign(split_cells);
  };

  #ifdef ALLOW_PARALLELIZATION
    std::transform(std::execution::par_unseq, ids.begin(), ids.end(),
        costs.begin(), getCostSwaths);
  #else
    std::transform(ids.begin(), ids.end(), costs.begin(), getCostSwaths);
  #endif

  return ids[std::min_element(
      costs.begin(), costs.end()) - costs.begin()] * step_angle;
}

F2CMultiLineString TrapezoidalDecomp::genSplitLines(
    const F2CCells& cells, double split_angle) const {
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

