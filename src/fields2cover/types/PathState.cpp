//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/PathState.h"

namespace f2c::types {

PathState PathState::clone() const {
  PathState new_state {*this};
  new_state.point = this->point.clone();
  return new_state;
}

Point PathState::atEnd() const {
  return point.getPointFromAngle(angle, len * static_cast<double>(dir));
}

}  // namespace f2c::types

