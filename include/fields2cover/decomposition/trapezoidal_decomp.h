//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
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
  /// Setter of the \a split_angle
  void setSplitAngle(double ang) {split_angle = ang;}
  double getSplitAngle() const {return split_angle;}

 public:
  F2CMultiLineString genSplitLines(const F2CCells& cells,
      const obj::DecompObjective& obj = obj::DecompObjective()) override;

 private:
  double split_angle {0.0};
};


}  // namespace f2c::decomp


#endif  // FIELDS2COVER_DECOMPOSITION_TRAPEZOIDAL_DECOMP_H_
