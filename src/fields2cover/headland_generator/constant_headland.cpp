//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <utility>
#include "fields2cover/headland_generator/constant_headland.h"

namespace f2c::hg {

F2CCells ConstHL::generateHeadlands(
    const F2CCells& field, double dist_headland) {
  return field.Buffer(-dist_headland);
}


}  // namespace f2c::hg

