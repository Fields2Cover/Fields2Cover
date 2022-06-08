//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/path_planning/path_planning.h"

namespace f2c {
namespace pp {

F2CPath PathPlanning::searchBestPath(const F2CSwaths& _swaths,
    TurningBase& _turn) {
  F2CPath path;
  if (_swaths.size() > 1) {
    for (size_t i = 0; i < (_swaths.size() - 1); ++i) {
      path.appendSwath(_swaths[i], _turn.robot.cruise_speed);
      F2CPath turn = _turn.createTurn(
          _swaths[i].endPoint(), _swaths[i].getOutAngle(),
          _swaths[i + 1].startPoint(), _swaths[i + 1].getInAngle());
      if (turn_point_dist > 0.0 && turn.size() > 1) {
        int n = static_cast<int>((10.0 / turn_point_dist) *
          _swaths[i].endPoint().Distance(_swaths[i + 1].startPoint()));
        turn.populate(n);
        turn.reduce(turn_point_dist);
      }
      path += turn;
    }
  }
  if (_swaths.size() > 0) {
    path.appendSwath(_swaths[_swaths.size()-1], _turn.robot.cruise_speed);
  }
  return path;
}

}  // namespace pp
}  // namespace f2c

