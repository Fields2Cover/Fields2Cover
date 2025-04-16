//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_H_
#define FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_H_

#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/objectives/sg_obj/sg_objective.h"

namespace f2c::sg {

enum class SwathOverlapType {
  NO_OVERLAP = 0,
  END_OVERLAP = 1,
  MIDDLE_OVERLAP = 2,
  EVENLY_DISTRIBUTED_OVERLAP = 3,
};

class SwathGeneratorBase {
 public:
  bool getAllowOverlap() const;
  void setAllowOverlap(bool);
  SwathOverlapType getOverlapType() const;
  void setOverlapType(SwathOverlapType dist);

  virtual F2CSwaths generateBestSwaths(const f2c::obj::SGObjective& obj,
      double op_width, const F2CCell& poly) const;

  virtual F2CSwaths generateBestSwaths(const f2c::obj::SGObjective& obj,
      double op_width, const F2CCell& poly, SwathOverlapType dist) const;
  virtual std::vector<F2CSwaths> generateBestSwaths(const f2c::obj::SGObjective& obj,
      const std::vector<double>& op_width, const F2CCell& poly) const;

  virtual F2CSwathsByCells generateBestSwaths(const f2c::obj::SGObjective& obj,
      double op_width, const F2CCells& polys) const;
  virtual std::vector<F2CSwathsByCells> generateBestSwaths(const f2c::obj::SGObjective& obj,
      const std::vector<double>& op_width, const F2CCells& polys) const;

  // TODO: search a better way to modify the distribution
  virtual F2CSwathsByCells generateBestSwaths(const f2c::obj::SGObjective& obj,
      double op_width, const F2CCells& polys, SwathOverlapType dist) const;

  virtual F2CSwaths generateSwaths(double angle,
      double op_width, const F2CCell& poly) const;
  virtual std::vector<F2CSwaths> generateSwaths(double angle,
      const std::vector<double>& widths, const F2CCell& poly) const;

  virtual F2CSwaths generateSwaths(double angle,
      double op_width, const F2CCell& poly, SwathOverlapType dist) const;

  virtual F2CSwathsByCells generateSwaths(double angle,
      double op_width, const F2CCells& polys) const;
  virtual std::vector<F2CSwathsByCells> generateSwaths(double angle,
      const std::vector<double>& widths, const F2CCells& poly) const;

  virtual F2CSwathsByCells generateSwaths(const std::vector<double>& angles,
      double op_width, const F2CCells& polys, SwathOverlapType dist) const;

  virtual F2CSwathsByCells generateSwaths(double angle,
      double op_width, const F2CCells& polys, SwathOverlapType dist) const;

  virtual double computeCostOfAngle(const f2c::obj::SGObjective& obj,
      double ang, double op_width, const F2CCell& poly,
      SwathOverlapType dist) const;
  virtual double computeCostOfAngle(const f2c::obj::SGObjective& obj,
      double ang, const std::vector<double>& widths, const F2CCell& poly) const;

  virtual double computeBestAngle(const f2c::obj::SGObjective& obj,
      double op_width, const F2CCell& poly,
      SwathOverlapType dist) const = 0;
  virtual double computeBestAngle(const f2c::obj::SGObjective& obj,
      const std::vector<double>& op_width, const F2CCell& poly) const = 0;


  virtual std::vector<double> computeBestAngles(
      const f2c::obj::SGObjective& obj,
      double op_width, const F2CCells& polys,
      SwathOverlapType dist) const;

  virtual ~SwathGeneratorBase() = default;

  static std::vector<double> getSwathsDistribution(
      SwathOverlapType type, double field_width, double cov_width);

 protected:
  SwathOverlapType dist_type {SwathOverlapType::NO_OVERLAP};
};

}  // namespace f2c::sg




#endif  // FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_H_
