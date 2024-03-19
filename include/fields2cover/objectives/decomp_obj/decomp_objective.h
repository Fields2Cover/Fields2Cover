//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_DECOMP_OBJECTIVE_H_
#define FIELDS2COVER_OBJECTIVES_DECOMP_OBJECTIVE_H_

#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/objectives/base_objective.h"

namespace f2c::obj {

/// @brief Base class for objective functions of cell decomposition.
class DecompObjective : public BaseObjective<DecompObjective> {
 public:
  virtual double computeCost(const F2CCells& cells);
};


}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_DECOMP_OBJECTIVE_H_
