//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <steering_functions/hc_cc_state_space/cc00_dubins_state_space.hpp>
#include "fields2cover/path_planning/steer_to_path.hpp"
#include "fields2cover/path_planning/dubins_curves_cc.h"

namespace f2c::pp {

F2CPath DubinsCurvesCC::createSimpleTurn(const F2CRobot& robot,
    double dist_start_pos, double start_angle, double end_angle,
    bool using_impl, double max_headland_width) const {
  steer::State start, end;

  start.x = 0.0;
  start.y = 0.0;
  start.theta = start_angle;
  start.kappa = 0.0;
  start.d = 0;

  end.x = dist_start_pos;
  end.y = 0.0;
  end.theta = end_angle;
  end.kappa = 0.0;
  end.d = 0;

  double n = 0.0;

  while (true) {
    CC00_Dubins_State_Space ss(
        robot.getMaxCurv(using_impl) / (1 + 0.05 * n),
        robot.getMaxDiffCurv(using_impl) / (1 + 0.2 * n),
        discretization,
        true);
    if (loop_detected(ss.get_controls(start, end)) && n <= 20.) {
      n += 1.0;
    } else {
      return steerStatesToPath(ss.get_path(start, end),
          robot.getTurnVel(using_impl), using_impl);
    }
  }
}

}  // namespace f2c::pp

