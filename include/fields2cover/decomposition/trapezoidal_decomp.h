//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_DECOMPOSITION_TRAPEZOIDAL_DECOMP_H_
#define FIELDS2COVER_DECOMPOSITION_TRAPEZOIDAL_DECOMP_H_

#include "fields2cover/types.h"
#include "fields2cover/decomposition/decomposition_base.h"

namespace f2c::decomp {

/// @brief Trapezoidal decomposition \cite latombe1991exact
///
/// Each point generate split lines perpendicular to the
/// \a split_angle.
class TrapezoidalDecomp : public DecompositionBase {
 public:
  using DecompositionBase::decompose;
  using DecompositionBase::split;
  // using DecompositionBase::genSplitLines;

  F2CCells decompose(const F2CCells& cells,
      const obj::DecompObjective& obj = obj::DecompObjective()) const override;

  virtual F2CCells decompose(const F2CCells& cells, double split_angle) const;

  virtual F2CCells split(const F2CCells& cells, double split_angle) const;

  /// Split the field into several cells that are easier to cover
  /// @param cells Original cells
  /// @return Smaller cells that compound the field
  F2CMultiLineString genSplitLines(const F2CCells& cells,
      const obj::DecompObjective& obj = obj::DecompObjective()) const override;

  virtual F2CMultiLineString genSplitLines(const F2CCells& cells,
      double split_angle) const;

  double computeMinDecompAngle(
      const F2CCells& cells, const obj::DecompObjective& obj) const;
};


}  // namespace f2c::decomp


#endif  // FIELDS2COVER_DECOMPOSITION_TRAPEZOIDAL_DECOMP_H_
