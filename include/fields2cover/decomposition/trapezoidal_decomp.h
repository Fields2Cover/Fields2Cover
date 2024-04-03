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
  /// Get split angle used to decompose the field
  double getSplitAngle() const;
  /// Set split angle used to decompose the field
  void setSplitAngle(double ang);

 public:
  /// Split the field into several cells that are easier to cover
  /// @param cells Original cells
  /// @return Smaller cells that compound the field
  F2CMultiLineString genSplitLines(const F2CCells& cells,
      const obj::DecompObjective& obj = obj::DecompObjective()) override;

 protected:
  double split_angle {0.0};
};


}  // namespace f2c::decomp


#endif  // FIELDS2COVER_DECOMPOSITION_TRAPEZOIDAL_DECOMP_H_
