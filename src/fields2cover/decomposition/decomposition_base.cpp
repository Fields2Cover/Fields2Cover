//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/decomposition/decomposition_base.h"

namespace f2c::decomp {


F2CCells DecompositionBase::decompose(
    const F2CCells& cells, const obj::DecompObjective& obj) const {
  return merge(split(cells, obj), obj);
}

F2CCells DecompositionBase::decompose(
    const F2CCell& cell, const obj::DecompObjective& obj) const {
  return decompose(static_cast<F2CCells>(cell), obj);
}

F2CCells DecompositionBase::split(
    const F2CCells& cells, const obj::DecompObjective& obj) const {
  return cells.splitByLine(genSplitLines(cells, obj));
}

F2CCells DecompositionBase::merge(
    const F2CCells& cells, const obj::DecompObjective& obj) const {
  return cells;
}

std::vector<double> DecompositionBase::getCovAngles() const {
  return cov_angles;
}


}  // namespace f2c::decomp

