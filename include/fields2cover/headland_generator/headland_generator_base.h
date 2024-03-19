//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_HEADLAND_GENERATOR_HEADLAND_GENERATOR_BASE_H_
#define FIELDS2COVER_HEADLAND_GENERATOR_HEADLAND_GENERATOR_BASE_H_

#include <vector>
#include "fields2cover/types.h"

namespace f2c::hg {

/// Base class to generate the headlands.
class HeadlandGeneratorBase {
 public:
  /// Get the regions of the field without headlands.
  /// @param _field Original field
  /// @param _dist_headland Width of the headland
  /// @return Field without headlands
  virtual F2CCells generateHeadlands(
    const F2CCells& field, double dist_headland) = 0;

  /// Generate headland area of the field for a given number of swaths
  ///   with given width.
  ///
  /// @param field Borders of the field and the obstacles on it.
  /// @param swath_width Width of each headland swath.
  /// @param n_swaths Number of headland swaths.
  /// @return Headland area
  virtual F2CCells generateHeadlandArea(
    const F2CCells& field, double swath_width, int n_swaths) = 0;

  /// Generate headland swaths of the field for a given number of swaths
  ///   with given width.
  ///
  /// @param field Borders of the field and the obstacles on it.
  /// @param swath_width Width of each headland swath.
  /// @param n_swaths Number of headland swaths.
  /// @param dir_out2in When true, headland swaths are created from
  ///          outer borders to inner borders.
  ///          Otherwise, they are created from inner borders to outers.
  /// @return Vector of size \a n_swaths for each headland swath.
  ///   Each F2CLinearRing on it is a headland swath ring.
  virtual std::vector<F2CCells> generateHeadlandSwaths(
    const F2CCells& field, double swath_width, int n_swaths,
    bool dir_out2in = true) = 0;
};

}  // namespace f2c::hg

#endif  // FIELDS2COVER_HEADLAND_GENERATOR_HEADLAND_GENERATOR_BASE_H_
