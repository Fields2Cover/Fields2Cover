//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/sg_objective.h"

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


double SGObjective::computeCost(
    const F2CCell& c, const F2CSwath& s) {
  return computeCost(s);
}

double SGObjective::computeCost(
    const F2CCells& c, const F2CSwath& s) {
  return computeCost(s);
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

