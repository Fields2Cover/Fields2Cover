//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_SWATH_GENERATOR_CURVED_SWATHS_H_
#define FIELDS2COVER_SWATH_GENERATOR_CURVED_SWATHS_H_

#include <limits>
#include <utility>
#include <numeric>
#include <memory>
#include "fields2cover/types.h"
#include "fields2cover/swath_generator/swath_generator_base.h"

namespace f2c::sg {

class CurvedSwathsGen : public SwathGeneratorBase {
 public:
  using SwathGeneratorBase::generateBestSwaths;
  using SwathGeneratorBase::generateSwaths;
  using SwathGeneratorBase::computeBestAngle;
  using SwathGeneratorBase::computeBestAngles;

  double getThresholdAngle() const;
  void setThresholdAngle(double ang);

  double computeBestAngle(const f2c::obj::SGObjective& obj,
      double op_width, const F2CCell& poly,
      SwathOverlapType dist) const override;
  double computeBestAngle(const f2c::obj::SGObjective& obj,
      const std::vector<double>& widths, const F2CCell& poly) const override;

  F2CSwaths generateBestSwaths(const f2c::obj::SGObjective& obj,
      double op_width, const F2CCell& poly,
      SwathOverlapType dist) const override;

  F2CSwaths generateSwaths(double angle,
      double op_width, const F2CCell& poly,
      SwathOverlapType dist) const override;




 private:
  double threshold_angle {0.1};  // radians
};


}  // namespace f2c::sg



#endif  // FIELDS2COVER_SWATH_GENERATOR_CURVED_SWATHS_H_
