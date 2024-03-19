//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/PathState.h"

namespace f2c::types {

Point PathState::atEnd() const {
  return point.getPointFromAngle(angle, len * static_cast<double>(dir));
}

}  // namespace f2c::types

