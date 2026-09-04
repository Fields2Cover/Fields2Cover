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

/// How CorridorHL splits the corridor between two cells that share a border.
enum class CorridorShareMode {
  /// The smaller cell gives the whole corridor; ties split evenly. Default.
  ASYMMETRIC,
  /// Every shared border splits evenly, regardless of the cells' size.
  SYMMETRIC,
};

/// One shared border between two cells, and how the corridor over it is split.
///
/// A pair of touching cells gives two of these, one seen from each cell. The
/// pair that shares the border evenly gives 0.5 twice; otherwise the smaller
/// cell gives the whole corridor and the larger one keeps a row with a share
/// of 0, so a report can tell which side of the border the corridor came from.
struct CorridorShare {
  /// Cell the corridor is taken out of.
  size_t cell_i {0};
  /// Neighbour on the other side of the border.
  size_t cell_k {0};
  /// Perimeter of cell \a cell_i, the size the rule compares.
  double perimeter_i {0.0};
  /// Perimeter of cell \a cell_k.
  double perimeter_k {0.0};
  /// True when both perimeters are equal up to the tolerance of the rule.
  bool same_size {false};
  /// Part of the corridor width taken out of \a cell_i: 0, 0.5 or 1.
  double share {0.0};
  /// Length of the border the two cells share.
  double shared_length {0.0};
  /// Border segments the corridor is opened along.
  F2CMultiLineString shared_border;
};

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

  /// Split the corridor between each pair of cells that share a border.
  ///
  /// This is the rule generateHeadlands() applies, on its own: which cell
  /// gives the corridor, how much of it, and over which part of the border.
  /// @param field Cells that share borders, usually from a decomposition.
  /// @param mode Rule to split the corridor with.
  /// @return One share per ordered pair of cells that touch along a border.
  std::vector<CorridorShare> corridorShares(
      const F2CCells& field,
      CorridorShareMode mode = CorridorShareMode::ASYMMETRIC) const;

  /// Rule generateHeadlands() uses to split the corridor. Defaults to
  /// ASYMMETRIC.
  void setShareMode(CorridorShareMode mode);

  /// Rule generateHeadlands() currently uses to split the corridor.
  CorridorShareMode getShareMode() const;

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

 private:
  CorridorShareMode share_mode_ {CorridorShareMode::ASYMMETRIC};

  /// Tolerance the neighbour is buffered by to find the shared border.
  double tol_ {1e-3};
  /// Width the zero-width spur a difference can leave behind is opened by.
  double spur_ {1e-9};
  /// Tolerance two perimeters are compared with to count as the same size.
  double same_size_tol_ {1e-9};
  /// Shortest border kept as real: buffering the neighbour by tol_ turns a
  /// shared corner into a piece a few millimetres long on each edge that
  /// reaches it, and a border that short is a corner, not a corridor.
  double min_border_ {1e-2};
};

}  // namespace f2c::hg


#endif  // FIELDS2COVER_HEADLAND_GENERATOR_CORRIDOR_HEADLAND_H_
