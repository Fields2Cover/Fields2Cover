//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/decomp_obj/decomp_objective.h"

namespace f2c::obj {

double DecompObjective::computeCost(const F2CLinearRing& ring) const {
  return 0.0;
}

double DecompObjective::computeCost(const F2CCell& cell) const {
  double cost {0.0};
  for (auto&& r : cell) {
    cost += computeCost(r);
  }
  return cost;
}

double DecompObjective::computeCost(const F2CCells& cells) const {
  double cost {0.0};
  for (auto&& c : cells) {
    cost += computeCost(c);
  }
  return cost;
}

}  // namespace f2c::obj

