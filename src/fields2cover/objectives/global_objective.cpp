//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/global_objective.h"

namespace f2c::obj {

bool GlobalObjective::isMinimizing() const {
  return true;
}

bool GlobalObjective::isMaximizing() const {
  return !isMinimizing();
}


double GlobalObjective::computeCost(const F2CSwath&) const {
  return 0.0;
}

double GlobalObjective::computeCost(const F2CSwaths&) const {
  return 0.0;
}

double GlobalObjective::computeCost(const F2CSwathsByCells& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
      [this] (double init, const auto& s) {
      return init + this->computeCost(s);});
}

double GlobalObjective::computeCost(const F2CCells&) const {
  return 0.0;
}

double GlobalObjective::computeCost(const F2CCell&) const {
  return 0.0;
}

double GlobalObjective::computeCost(const F2CRoute&) const {
  return 0.0;
}

double GlobalObjective::computeCost(const F2CPath&) const {
  return 0.0;
}

double GlobalObjective::computeCost(
    const F2CCell& c, const F2CSwath& s) const {
  return computeCost(c) + computeCost (s);
}

double GlobalObjective::computeCost(
    const F2CCell& c, const F2CSwaths& s) const {
  return computeCost(c) + computeCost (s);
}

double GlobalObjective::computeCost(const F2CCell& c,
    const F2CSwathsByCells& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}

double GlobalObjective::computeCost(
    const F2CCells& c, const F2CSwath& s) const {
  return computeCost(c) + computeCost (s);
}

double GlobalObjective::computeCost(
    const F2CCells& c, const F2CSwaths& s) const {
  return computeCost(c) + computeCost (s);
}

double GlobalObjective::computeCost(const F2CCells& c,
    const F2CSwathsByCells& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [this, &c] (double init, const auto& s) {return init + computeCost(c, s);});
}

double GlobalObjective::computeCost(
    const F2CRoute& r, const F2CPath& p) const {
  return computeCost (r) + computeCost(p);
}

double GlobalObjective::computeCost(
    const F2CCell& c, const F2CSwath& s,
    const F2CRoute& r) const {
  return computeCost(c, s) + computeCost (r);
}

double GlobalObjective::computeCost(
    const F2CCell& c, const F2CSwath& s,
    const F2CPath& p) const {
  return computeCost(c, s) + computeCost(p);
}

double GlobalObjective::computeCost(
    const F2CCell& c, const F2CSwath& s,
    const F2CRoute& r, const F2CPath& p) const {
  return computeCost(c, s) + computeCost (r) + computeCost(p);
}

double GlobalObjective::computeCost(
    const F2CCell& c, const F2CSwaths& s,
    const F2CRoute& r) const {
  return computeCost(c, s) + computeCost (r);
}

double GlobalObjective::computeCost(
    const F2CCell& c, const F2CSwaths& s,
    const F2CPath& p) const {
  return computeCost(c, s) + computeCost(p);
}

double GlobalObjective::computeCost(
    const F2CCell& c, const F2CSwaths& s,
    const F2CRoute& r, const F2CPath& p) const {
  return computeCost(c, s) + computeCost (r) + computeCost(p);
}

double GlobalObjective::computeCost(
  const F2CCell& c, const F2CSwathsByCells& s,
    const F2CRoute& r) const {
  return computeCost(c, s) + computeCost (r);
}

double GlobalObjective::computeCost(
    const F2CCell& c, const F2CSwathsByCells& s,
    const F2CPath& p) const {
  return computeCost(c, s) + computeCost(p);
}

double GlobalObjective::computeCost(
    const F2CCell& c, const F2CSwathsByCells& s,
    const F2CRoute& r, const F2CPath& p) const {
  return computeCost(c, s) + computeCost (r) + computeCost(p);
}

double GlobalObjective::computeCost(
    const F2CCells& c, const F2CSwath& s,
    const F2CRoute& r) const {
  return computeCost(c, s) + computeCost (r);
}

double GlobalObjective::computeCost(
    const F2CCells& c, const F2CSwath& s,
    const F2CPath& p) const {
  return computeCost(c, s) + computeCost(p);
}

double GlobalObjective::computeCost(
    const F2CCells& c, const F2CSwath& s,
    const F2CRoute& r, const F2CPath& p) const {
  return computeCost(c, s) + computeCost (r) + computeCost(p);
}

double GlobalObjective::computeCost(
    const F2CCells& c, const F2CSwaths& s,
    const F2CRoute& r) const {
  return computeCost(c, s) + computeCost (r);
}

double GlobalObjective::computeCost(
    const F2CCells& c, const F2CSwaths& s,
    const F2CPath& p) const {
  return computeCost(c, s) + computeCost(p);
}

double GlobalObjective::computeCost(
    const F2CCells& c, const F2CSwaths& s,
    const F2CRoute& r, const F2CPath& p) const {
  return computeCost(c, s) + computeCost (r) + computeCost(p);
}

double GlobalObjective::computeCost(
    const F2CCells& c, const F2CSwathsByCells& s,
    const F2CRoute& r) const {
  return computeCost(c, s) + computeCost (r);
}

double GlobalObjective::computeCost(
    const F2CCells& c, const F2CSwathsByCells& s,
    const F2CPath& p) const {
  return computeCost(c, s) + computeCost(p);
}

double GlobalObjective::computeCost(
    const F2CCells& c, const F2CSwathsByCells& s,
    const F2CRoute& r, const F2CPath& p) const {
  return computeCost(c, s) + computeCost (r) + computeCost(p);
}


}  // namespace f2c::obj

