//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/hg_obj/hg_objective.h"

namespace f2c::obj {

double HGObjective::computeCost(
    const F2CCell& total_cell, const F2CCell& rem_cell) {
  return computeCost(
      static_cast<F2CCells>(total_cell), static_cast<F2CCells>(rem_cell));
}
double HGObjective::computeCost(
    const F2CCell& total_cell, const F2CCells& rem_cell) {
  return computeCost(static_cast<F2CCells>(total_cell), rem_cell);
}
double HGObjective::computeCost(
    const F2CCells& total_cell, const F2CCell& rem_cell) {
  return computeCost(total_cell, static_cast<F2CCells>(rem_cell));
}
double HGObjective::computeCost(
    const F2CCells& total_cell, const F2CCells& rem_cell) {
  return 1 - total_cell.difference(rem_cell).area() / total_cell.area();
}

}  // namespace f2c::obj

