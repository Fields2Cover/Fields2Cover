//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_POLYGON_ALTITUDE_H_
#define FIELDS2COVER_OBJECTIVES_POLYGON_ALTITUDE_H_

#include "fields2cover/types.h"
#include "fields2cover/objectives/sg_obj/sg_objective.h"

namespace f2c::obj {

/// @brief SG cost function as the height of the polygon + height of the holes.
///    \cite huang2001optimal
class PolygonAltitude : public SGObjective {
 public:
  using SGObjective::computeCost;
  bool isFastCompAvailable() const override {return true;}
  double computeCost(double ang, double op_width, const F2CCell& cell) const override;
};

}  // namespace f2c::obj

#endif  //  FIELDS2COVER_OBJECTIVES_POLYGON_ALTITUDE_H_
