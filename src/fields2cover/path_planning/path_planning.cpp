//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/path_planning/path_planning.h"

namespace f2c::pp {

F2CPath PathPlanning::searchBestPath(const F2CRobot& robot,
    const F2CSwaths& swaths, TurningBase& turn) {
  F2CPath path;
  if (swaths.size() > 1) {
    for (size_t i = 0; i < (swaths.size() - 1); ++i) {
      path.appendSwath(swaths[i], robot.cruise_speed);
      F2CPath turn_path = turn.createTurn(robot,
          swaths[i].endPoint(), swaths[i].getOutAngle(),
          swaths[i + 1].startPoint(), swaths[i + 1].getInAngle());
      if (turn_point_dist > 0.0 && turn_path.size() > 1) {
        int n = static_cast<int>((10.0 / turn_point_dist) *
          swaths[i].endPoint().Distance(swaths[i + 1].startPoint()));
        turn_path.populate(n);
        turn_path.reduce(turn_point_dist);
      }
      path += turn_path;
    }
  }
  if (swaths.size() > 0) {
    path.appendSwath(swaths[swaths.size()-1], robot.cruise_speed);
  }
  return path;
}

}  // namespace f2c::pp

