//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/pp_obj/pp_objective.h"

namespace f2c::obj {

double PPObjective::computeCost(const F2CPath& path) {
  return path.length();
}

}  // namespace f2c::obj

