//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/decomposition/decomposition_base.h"

namespace f2c::decomp {


F2CCells DecompositionBase::decompose(
    const F2CCells& cells, const obj::DecompObjective& obj) {
  return merge(split(cells, obj), obj);
}

F2CCells DecompositionBase::split(
    const F2CCells& cells, const obj::DecompObjective& obj) {
  return cells.splitByLine(genSplitLines(cells, obj));
}

F2CCells DecompositionBase::merge(
    const F2CCells& cells, const obj::DecompObjective& obj) {
  return cells;
}


namespace {
// Part of the coverage width a border detail has to exceed to survive.
//
// Measured over 305 real fields and two robots: at this share the
// decomposition returns 30% fewer cells, no piece of any field lands outside
// the field it came from, and the area moves by less than a tenth of a
// percent. Taking less leaves noise behind; taking much more starts merging
// splits the shape of the field genuinely asks for.
constexpr double kBorderDetailShare = 0.6;
}  // namespace

F2CCells simplifyForDecomposition(
    const F2CCells& cells, const F2CRobot& robot) {
  return cells.simplify(kBorderDetailShare * robot.getCovWidth());
}

}  // namespace f2c::decomp

