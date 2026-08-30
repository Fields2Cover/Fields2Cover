//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_HEADLAND_GENERATOR_CORRIDOR_HEADLAND_H_
#define FIELDS2COVER_HEADLAND_GENERATOR_CORRIDOR_HEADLAND_H_

#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/headland_generator/headland_generator_base.h"

namespace f2c::hg {

/// Class to open a corridor where cells border each other, leaving the edges
/// that face the outer boundary or a void untouched.
///
/// Meant for cells that came out of a decomposition and already have a
/// headland around the field: shrinking every border again would take a second
/// headland off ground that has one.
class CorridorHL : public HeadlandGeneratorBase {
 public:
  using HeadlandGeneratorBase::generateHeadlands;

  /// Open a corridor of the given width between cells that share a border.
  /// @param field Cells that share borders, usually from a decomposition.
  /// @param dist_headland Width of the corridor.
  /// @return Mainland area
  F2CCells generateHeadlands(
    const F2CCells& field, double dist_headland) override;

  /// Open a corridor wide enough for \a n_swaths passes.
  /// @param field Borders of the field and the obstacles on it.
  /// @param swath_width Width of each headland swath.
  /// @param n_swaths Number of headland swaths.
  /// @return Mainland area
  F2CCells generateHeadlandArea(
    const F2CCells& field, double swath_width, int n_swaths) override;

  /// Widen the corridor one swath at a time.
  /// @param field Borders of the field and the obstacles on it.
  /// @param swath_width Width of each headland swath.
  /// @param n_swaths Number of headland swaths.
  /// @param dir_out2in When true, the widest corridor comes first.
  /// @return Vector of size \a n_swaths, each one carved a swath wider.
  std::vector<F2CCells> generateHeadlandSwaths(
    const F2CCells& field, double swath_width, int n_swaths,
    bool dir_out2in = true) override;
};

}  // namespace f2c::hg


#endif  // FIELDS2COVER_HEADLAND_GENERATOR_CORRIDOR_HEADLAND_H_
