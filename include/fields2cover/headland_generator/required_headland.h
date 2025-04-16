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

// TODO: Return the headland swaths
class ReqHL : public HeadlandGeneratorBase {
 public:
  F2CCells generateHeadlands(
    const F2CCell& field, const F2CRobot& robot,
    double track_ang) const override;
  F2CCells generateHeadlands(
    const F2CCells& field, const F2CRobot& robot,
    const std::vector<double>& track_ang) const override;

  F2CCells generateHeadlands(
    const F2CCells& field, double dist_headland) const override;

  F2CCells generateHeadlandArea(
    const F2CCells& field, double swath_width, int n_swaths) const override;

  std::vector<F2CMultiLineString> generateHeadlandSwaths(
    const F2CCells& field, double swath_width, int n_swaths,
    bool dir_out2in = true) const override;

  double requiredHeadlandDist(const F2CRobot& robot,
      double track_ang, double border_ang) const;

  std::vector<double> requiredHeadlandDist(const F2CRobot& robot,
      double track_ang, const F2CLinearRing& ring) const;
};

}  // namespace f2c::hg


#endif  // FIELDS2COVER_HEADLAND_GENERATOR_REQUIRED_HEADLAND_H_
