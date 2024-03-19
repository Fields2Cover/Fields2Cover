//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_HEADLAND_GENERATOR_CONSTANT_HEADLAND_H_
#define FIELDS2COVER_HEADLAND_GENERATOR_CONSTANT_HEADLAND_H_

#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/headland_generator/headland_generator_base.h"

namespace f2c::hg {

/// Class to generate headlands with equal width in each border.
class ConstHL : public HeadlandGeneratorBase {
 public:
  /// Generate headland area of the field at a given distance from borders
  /// @param field Borders of the field and the obstacles on it.
  /// @param dist_headland Distance between exterior and interior borders
  ///                      of the headlands.
  /// @return Mainland area
  F2CCells generateHeadlands(
    const F2CCells& field, double dist_headland) override;

  /// Generate headland area of the field for a given number of swaths
  ///   with given width.
  /// @param field Borders of the field and the obstacles on it.
  /// @param swath_width Width of each headland swath.
  /// @param n_swaths Number of headland swaths.
  /// @return Headland area
  F2CCells generateHeadlandArea(
    const F2CCells& field, double swath_width, int n_swaths) override;

  /// Generate headland swaths of the field for a given number of swaths
  /// with given width.
  ///
  /// @param field Borders of the field and the obstacles on it.
  /// @param swath_width Width of each headland swath.
  /// @param n_swaths Number of headland swaths.
  /// @param dir_out2in When true, headland swaths are created from
  ///          outer borders to inner borders.
  ///           Otherwise, they are created from inner borders to outers.
  /// @return Vector of size \a n_swaths for each headland swath.
  ///   Each F2CLinearRing on it is a headland swath ring.
  std::vector<F2CCells> generateHeadlandSwaths(
    const F2CCells& field, double swath_width, int n_swaths,
    bool dir_out2in = true) override;
};

}  // namespace f2c::hg


#endif  // FIELDS2COVER_HEADLAND_GENERATOR_CONSTANT_HEADLAND_H_
