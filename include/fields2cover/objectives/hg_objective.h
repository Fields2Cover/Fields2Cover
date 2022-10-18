//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_HG_OBJECTIVE_H_
#define FIELDS2COVER_OBJECTIVES_HG_OBJECTIVE_H_

#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/objectives/base_objective.h"

namespace f2c::obj {

/// @brief Base class for objective functions of Headland generator.
class HGObjective : public BaseObjective<HGObjective> {
 public:
  virtual double computeCost(
      const F2CCell& total_cell, const F2CCell& rem_cell);
  virtual double computeCost(
      const F2CCells& total_cell, const F2CCell& rem_cell);
  virtual double computeCost(
      const F2CCell& total_cell, const F2CCells& rem_cell);
  virtual double computeCost(
      const F2CCells& total_cell, const F2CCells& rem_cell);
};


}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_HG_OBJECTIVE_H_
