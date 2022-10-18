//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/direct_dist_path_obj.h"

namespace f2c::obj {

double DirectDistPathObj::computeCost(
    const F2CPoint& p1, const F2CPoint& p2) {
  return p1.Distance(p2);
}

}  // namespace f2c::obj

