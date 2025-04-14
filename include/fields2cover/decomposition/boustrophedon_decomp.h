//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_DECOMPOSITION_BOUSTROPHEDON_DECOMP_H_
#define FIELDS2COVER_DECOMPOSITION_BOUSTROPHEDON_DECOMP_H_

#include "fields2cover/types.h"
#include "fields2cover/decomposition/trapezoidal_decomp.h"

namespace f2c::decomp {

/// @brief Boustrophedon Cellular decomposition \cite choset2000coverage
///
/// Each point in the geometry is checked if it is a critical point.
/// A critical point is defined as a point that split or merge cells
/// in the \a split_angle .
/// If it is a critical point, split lines are created in the perpendicular
/// to the \a split_angle.
class BoustrophedonDecomp : public TrapezoidalDecomp {
 public:
  /// Split the field into several cells that are easier to cover
  /// @param cells Original cells
  /// @return Smaller cells that compound the field
  F2CMultiLineString genSplitLines(const F2CCells& cells,
      const obj::DecompObjective& obj = obj::DecompObjective()) override;

 private:
  bool isLinePartOfLinearRing(const F2CLinearRing & ring, const F2CLineString & line) const;
};

}  // namespace f2c::decomp


#endif  // FIELDS2COVER_DECOMPOSITION_BOUSTROPHEDON_DECOMP_H_
