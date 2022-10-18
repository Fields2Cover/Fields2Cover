//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_REM_AREA_H_
#define FIELDS2COVER_OBJECTIVES_REM_AREA_H_

#include "fields2cover/types.h"
#include "fields2cover/objectives/hg_objective.h"

namespace f2c::obj {

class RemArea : public HGObjective {
 public:
  using HGObjective::computeCost;
  bool isMinimizing(void) const override;
};

}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_REM_AREA_H_
