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

}  // namespace f2c::decomp


#endif  // FIELDS2COVER_DECOMPOSITION_DECOMPOSITION_BASE_H_
