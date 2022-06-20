//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_OPTIMIZATIONPARAMS_H_
#define FIELDS2COVER_TYPES_OPTIMIZATIONPARAMS_H_

namespace f2c::types {

struct OptimizationParams {
 public:
  double best_angle;
  double headland_width;
  uint16_t max_turns;
  double cost_if_no_crop;
  double greening_subsidy;
  double increment;
  double penalty_extra_swath;
};
}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_OPTIMIZATIONPARAMS_H_
