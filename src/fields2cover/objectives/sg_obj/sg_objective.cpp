//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/sg_obj/sg_objective.h"

namespace f2c::obj {

double SGObjective::computeCost(const F2CSwath&) {
  return 0.0;
}

double SGObjective::computeCost(const F2CSwaths& swaths) {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
      [this] (double init, const auto& s) {
      return init + this->computeCost(s);});
}

double SGObjective::computeCost(const F2CSwathsByCells& swaths) {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
      [this] (double init, const auto& s) {
      return init + this->computeCost(s);});
}

double SGObjective::computeCost(const F2CCell& cell) {
  return computeCost(0.0, 0.0, cell);
}

double SGObjective::computeCost(const F2CCells& cells) {
  return std::accumulate(cells.begin(), cells.end(), 0.0,
      [this] (double init, const auto& s) {
      return init + this->computeCost(s);});
}

double SGObjective::computeCost(
    double ang, double op_width, const F2CCell& cell) {
  return 0.0;
}

double SGObjective::computeCost(
    const F2CCell& c, const F2CSwath& s) {
  return computeCost(c) + computeCost(s);
}

double SGObjective::computeCost(
    const F2CCells& c, const F2CSwath& s) {
  return computeCost(c) + computeCost(s);
}

double SGObjective::computeCost(
    const F2CCell& c, const F2CSwaths& swaths) {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}

double SGObjective::computeCost(const F2CCell& c,
    const F2CSwathsByCells& swaths) {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}


double SGObjective::computeCost(
    const F2CCells& c, const F2CSwaths& swaths) {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}

double SGObjective::computeCost(const F2CCells& c,
    const F2CSwathsByCells& swaths) {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}


}  // namespace f2c::obj

