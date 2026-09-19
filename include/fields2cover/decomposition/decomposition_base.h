//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_DECOMPOSITION_DECOMPOSITION_BASE_H_
#define FIELDS2COVER_DECOMPOSITION_DECOMPOSITION_BASE_H_

#include "fields2cover/types.h"
#include "fields2cover/objectives/decomp_obj/decomp_objective.h"

namespace f2c::decomp {

///  @brief Base class for algorithms that decompose non-convex cells
/// into multiple simpler cells.
///
/// The class has one main method (f2c::decomp::decompose) that split
/// complex-shape cells into simpler cells.
class DecompositionBase {
 public:
  /// @brief Decompose F2CCells into simpler F2CCells
  ///
  /// This method decompose every Cell in \p cells into simpler cells.
  /// @param cells Complex-shape cells
  /// @return Same space as @p cells recoded into simpler cells
  virtual F2CCells decompose(const F2CCells& cells,
      const obj::DecompObjective& obj = obj::DecompObjective());

 public:
  /// Split the field into several cells that are easier to cover
  /// @param cells Original cells
  /// @return Smaller cells that compound the field
  virtual F2CCells split(const F2CCells& cells,
      const obj::DecompObjective& obj);

  /// Generate the lines used by f2c::decomp::split to split the cells into
  /// simpler cells
  /// @param cells Complex-shape cells
  /// @return Lines that split the complex-shape cells into simpler cells
  virtual F2CMultiLineString genSplitLines(const F2CCells& cells,
      const obj::DecompObjective& obj) = 0;

  /// Apply a merge strategy to reduce the number of simpler cells
  /// @param cells Simple cells
  /// @return Merged cells.
  virtual F2CCells merge(const F2CCells& cells,
      const obj::DecompObjective& obj);

  virtual ~DecompositionBase() = default;
};

/// Drop border detail a robot of this width cannot act on.
///
/// A decomposition splits a field wherever its border turns back on itself,
/// and a digitised border does that on details far smaller than the machine:
/// the field comes back cut into pieces that exist only in the survey, each
/// one paying for a headland it does not need. Detail below a share of the
/// coverage width is removed first, so the split lines follow the shape of
/// the field rather than the noise on it.
///
/// The result is not clipped to \a cells: simplifying moves the border both
/// ways, so a point can land outside. Pass ground that is already inside the
/// field -- a mainland from a headland generator, not the raw boundary.
/// @param cells Cells about to be decomposed.
/// @param robot Robot doing the coverage.
/// @return The same cells with border detail below the threshold removed
F2CCells simplifyForDecomposition(const F2CCells& cells, const F2CRobot& robot);

}  // namespace f2c::decomp


#endif  // FIELDS2COVER_DECOMPOSITION_DECOMPOSITION_BASE_H_
