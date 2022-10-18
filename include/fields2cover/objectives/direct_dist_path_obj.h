//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_DIRECT_DIST_PATH_OBJ_H_
#define FIELDS2COVER_OBJECTIVES_DIRECT_DIST_PATH_OBJ_H_

#include "fields2cover/types.h"
#include "fields2cover/objectives/rp_objective.h"

namespace f2c::obj {

/// Path cost function as the straight distance between points in a route
class DirectDistPathObj : public RPObjective {
 public:
  using RPObjective::computeCost;

  double computeCost(const F2CPoint& p1, const F2CPoint& p2) override;
};


}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_DIRECT_DIST_PATH_OBJ_H_
