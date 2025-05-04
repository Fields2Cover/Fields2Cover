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

PathState PathState::toPathState(const Point& start, const Point& end) {
  PathState s;
  s.point = start;
  s.angle = (end - start).getAngleFromPoint();
  s.len = start.distance(end);
  return s;
}

}  // namespace f2c::types

