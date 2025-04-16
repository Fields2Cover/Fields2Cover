//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/sg_obj/sg_objective.h"

namespace f2c::obj {

double SGObjective::computeCost(const F2CSwath&) const {
  return 0.0;
}

double SGObjective::computeCost(const F2CSwaths& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
      [this] (double init, const auto& s) {
      return init + this->computeCost(s);});
}

double SGObjective::computeCost(const F2CSwathsByCells& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
      [this] (double init, const auto& s) {
      return init + this->computeCost(s);});
}

double SGObjective::computeCost(const F2CCell& cell) const {
  return computeCost(0.0, 0.0, cell);
}

double SGObjective::computeCost(const F2CCells& cells) const {
  return std::accumulate(cells.begin(), cells.end(), 0.0,
      [this] (double init, const auto& s) {
      return init + this->computeCost(s);});
}

double SGObjective::computeCost(
    double ang, double op_width, const F2CCell& cell) const {
  return 0.0;
}

double SGObjective::computeCost(
    const F2CCell& c, const F2CSwath& s) const {
  return computeCost(c) + computeCost(s);
}

double SGObjective::computeCost(
    const F2CCells& c, const F2CSwath& s) const {
  return computeCost(c) + computeCost(s);
}

double SGObjective::computeCost(
    const F2CCell& c, const std::vector<F2CSwaths>& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}
double SGObjective::computeCost(
    const F2CCell& c, const F2CSwaths& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}

double SGObjective::computeCost(const F2CCell& c,
    const F2CSwathsByCells& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}
double SGObjective::computeCost(const F2CCell& c,
    const std::vector<F2CSwathsByCells>& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}


double SGObjective::computeCost(
    const F2CCells& c, const std::vector<F2CSwaths>& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}
double SGObjective::computeCost(
    const F2CCells& c, const F2CSwaths& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}

double SGObjective::computeCost(const F2CCells& c,
    const F2CSwathsByCells& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}

double SGObjective::computeCost(const F2CCells& c,
    const std::vector<F2CSwathsByCells>& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}


}  // namespace f2c::obj

