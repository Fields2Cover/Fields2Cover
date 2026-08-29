//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_HEADLAND_GENERATOR_REQUIRED_HEADLAND_H_
#define FIELDS2COVER_HEADLAND_GENERATOR_REQUIRED_HEADLAND_H_

#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/headland_generator/headland_generator_base.h"

namespace f2c::hg {

/// Class to generate headlands as wide as each border needs.
///
/// A border the swaths run along is only entered, while a border they end on
/// takes a whole turn. Sizing each one on its own leaves the difference to the
/// mainland instead of giving it up on every border.
class ReqHL : public HeadlandGeneratorBase {
 public:
  /// Generate the mainland of a cell covered at a given track angle.
  /// @param field Borders of the cell and the obstacles on it.
  /// @param robot Robot doing the coverage.
  /// @param track_ang Angle of the swaths on the cell.
  /// @return Mainland area
  F2CCells generateHeadlands(
    const F2CCell& field, const F2CRobot& robot, double track_ang) override;

  /// Generate the mainland of each cell covered at its own track angle.
  /// @param field Borders of the field and the obstacles on it.
  /// @param robot Robot doing the coverage.
  /// @param track_angs Angle of the swaths on each cell. Without one angle
  ///                   per cell, every border keeps the widest headland.
  /// @return Mainland area
  F2CCells generateHeadlands(
    const F2CCells& field, const F2CRobot& robot,
    const std::vector<double>& track_angs) override;

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
  /// @param field Borders of the field and the obstacles on it.
  /// @param swath_width Width of each headland swath.
  /// @param n_swaths Number of headland swaths.
  /// @param dir_out2in When true, headland swaths are created from
  ///          outer borders to inner borders.
  ///          Otherwise, they are created from inner borders to outers.
  /// @return Vector of size \a n_swaths for each headland swath.
  std::vector<F2CCells> generateHeadlandSwaths(
    const F2CCells& field, double swath_width, int n_swaths,
    bool dir_out2in = true) override;

  /// Width of the headland a border needs.
  /// @param robot Robot doing the coverage.
  /// @param track_ang Angle of the swaths on the cell.
  /// @param border_ang Angle of the border.
  /// @return Distance between the border and the mainland
  double requiredHeadlandDist(const F2CRobot& robot,
      double track_ang, double border_ang) const;

  /// Width of the headland each segment of a ring needs.
  /// @param robot Robot doing the coverage.
  /// @param track_ang Angle of the swaths on the cell.
  /// @param ring Border of the cell or of an obstacle on it.
  /// @return Distance for each segment of \a ring
  std::vector<double> requiredHeadlandDist(const F2CRobot& robot,
      double track_ang, const F2CLinearRing& ring) const;
};

}  // namespace f2c::hg


#endif  // FIELDS2COVER_HEADLAND_GENERATOR_REQUIRED_HEADLAND_H_
