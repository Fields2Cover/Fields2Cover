//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_HEADLAND_GENERATOR_HEADLAND_GENERATOR_BASE_H_
#define FIELDS2COVER_HEADLAND_GENERATOR_HEADLAND_GENERATOR_BASE_H_

#include <vector>
#include <utility>
#include "fields2cover/types.h"
#include "fields2cover/swath_generator/swath_generator_base.h"

namespace f2c::hg {

enum class CornerPlanners {
  FORWARD_CONT_IMPL_ON = 0,
  FORWARD_CONT_IMPL_OFF = 1,
  FORWARD_CONT_BUFF_IMPL_ON = 2,
  FISHTAIL = 3,
};


/// Base class to generate the headlands.
class HeadlandGeneratorBase {
 public:
  virtual F2CCells generateHeadlands(
    const F2CCell& field, const F2CRobot& robot, double track_ang) const;
  virtual F2CCells generateHeadlands(
    const F2CCells& field, const F2CRobot& robot,
    const std::vector<double>& track_angs) const;

  double maxHLWidthRequired(const F2CRobot& robot) const;


  /// Get the regions of the field without headlands.
  /// @param _field Original field
  /// @param _dist_headland Width of the headland
  /// @return Field without headlands
  virtual F2CCells generateHeadlands(
    const F2CCells& field, double dist_headland) const = 0;

  /// Generate headland area of the field for a given number of swaths
  ///   with given width.
  ///
  /// @param field Borders of the field and the obstacles on it.
  /// @param swath_width Width of each headland swath.
  /// @param n_swaths Number of headland swaths.
  /// @return Headland area
  virtual F2CCells generateHeadlandArea(
    const F2CCells& field, double swath_width, int n_swaths) const = 0;

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
  virtual std::vector<F2CMultiLineString> generateHeadlandSwaths(
    const F2CCells& field, double swath_width, int n_swaths,
    bool dir_out2in = true) const = 0;

  /*
  /// Return the headlands rings on the headlands as F2CCells.
  virtual std::vector<F2CCells> generateHeadlandSwaths(
    const F2CCells& field, double swath_width, int n_swaths,
    bool dir_out2in = true) = 0;
  */

  virtual ~HeadlandGeneratorBase() = default;

F2CLinearRing generateHeadlandExtBorder(
    const F2CLinearRing& border, std::vector<double> offsets) const;

  std::vector<F2CLinearRing> generateHeadlandBorders(
    const F2CLinearRing& border, std::vector<double> offsets,
    size_t n_headlands, double width_headland) const;

  F2CLineString generateHeadlandBorderRoute(
      const std::vector<F2CLinearRing>& borders, bool inside_out = false) const;

  F2CPathSimp connectHeadlandBorderRoute(const F2CRobot& robot,
      CornerPlanners planner, const F2CLineString& route,
      const std::vector<double>& offsets,
      bool is_border_right, bool inside_out) const;

  F2CPathSimp connectHeadlandBorderRoute(const F2CRobot& robot,
      const std::vector<CornerPlanners>& planners, const F2CLineString& route,
      const std::vector<double>& offsets,
      bool is_border_right, bool inside_out) const;

  F2CPathSimp genCornerTurn(const F2CRobot& robot,
      const F2CPoint& p1, const F2CPoint& p2, const F2CPoint& p3,
      bool is_border_right, CornerPlanners planner,
      size_t i_hl = 0, size_t max_hl = 0,
      double offset_start = 1e5, double offset_end = 1e5) const;


 public:
  static F2CPoint findCenterCornerTurn(const F2CPathSimp& path, double ang);

  // If dist_border_start or dist_border_end are negative,
  // there is no border near
  static F2CPathSimp outerCornerPlanning(const F2CRobot& robot,
      double ang_turn, CornerPlanners planner,
      size_t i_hl = 0, size_t max_hl = 0,
      double dist_border_start = 0.0, double dist_border_end = 0.0);

  static F2CPathSimp continuousFrontTurn(double ang_turn,
      const F2CRobot& robot, bool is_impl_on,
      double dist_wall_start, double dist_wall_end,
      double buff_cloth_pos = 0.0, double buff_cloth_neg = 0.0);
  static F2CPathSimp contFishTailTurn(
      double ang_turn, const F2CRobot& robot,
      double dist_wall_start = 0.0, double dist_wall_end = 0.0);

  static F2CPathSimp contConvexTurn(
      double ang, double max_kurv, double max_sigma, double buff_cloth = 0.0);
  static F2CPathSimp concaveTurn(
      double ang_end, double ang_t1, double ang_t2,
      double max_kurv, double max_sigma,
      double buff_cloth_pos, double buff_cloth_neg);
  static F2CPathSimp contConcaveTurn(
      double ang, const F2CRobot& robot, double max_kurv, double max_sigma,
      double rob_width, double offset_start, double offset_end,
      double buff_cloth_pos, double buff_cloth_neg);

  static std::vector<double> getConcaveTurnParams(
      double ang, const F2CRobot& robot, double kurv, double sigma,
      double buff_cloth_pos, double buff_cloth_neg);
  static std::vector<double> getTransAngleOnConcaveTurn(
      double ang, const F2CRobot& robot, double kurv, double sigma,
      double offset_start, double offset_end,
      double buff_cloth_pos, double buff_cloth_neg);


  static std::pair<double, double> computeOffsetTurn(
      const F2CRobot& robot, double ang_turn,
      CornerPlanners planner, size_t n_hl_swaths);
  static std::vector<std::pair<double, double>> computeOffsetTurn(
      const F2CRobot& robot, const F2CLinearRing& border, bool is_border_right,
      CornerPlanners planner, size_t n_hl_swaths);
  static std::vector<std::pair<double, double>> computeOffsetTurn(
      const F2CRobot& robot, const F2CLinearRing& border, bool is_border_right,
      std::vector<CornerPlanners> planners, size_t n_hl_swaths);
  static std::vector<double> pickOffsets(
      const std::vector<std::pair<double, double>>& pair_offsets,
      double w_robot, bool inside_out = false);

  static std::pair<double, double> computeConcaveOffset(
      double ang_t1, double ang_t2, const F2CRobot& robot,
      double kurv, double sigma, double buff_cloth_pos);

 public:
  static F2CPathSimp getConcaveHalfTurn(double ang_trans, double ang_end,
      double kurv, double sigma, double buff_cloth_pos, double buff_cloth_neg);

  static F2CPoint findOffsetCornerOnConcaveTurn(
      double ang, double offset1, double offset2);

 private:
  static bool checkConcaveTurnPathCrossBorder(
      double ang, const F2CRobot& robot, const F2CPathSimp& path,
      const std::pair<double, double>& offsets);

 protected:
  f2c::sg::SwathGeneratorBase* sgb;
  F2CSwathsByCells swaths;
  std::vector<double> swaths_angle;
};

}  // namespace f2c::hg

#endif  // FIELDS2COVER_HEADLAND_GENERATOR_HEADLAND_GENERATOR_BASE_H_
