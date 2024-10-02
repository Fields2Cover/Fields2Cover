//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <steering_functions/utilities/utilities.hpp>
#include "fields2cover/path_planning/path_planning.h"

namespace f2c::pp {

F2CPath PathPlanning::planPath(const F2CRobot& robot,
    const F2CRoute& route, TurningBase& turn) {
  F2CPath path;
  for (size_t i = 0; i < route.sizeVectorSwaths(); ++i) {
    auto prev_swaths = (i >0) ? route.getSwaths(i-1) : F2CSwaths();
    path += planPathForConnection(robot,
        prev_swaths, route.getConnection(i), route.getSwaths(i), turn);
    path += planPath(robot, route.getSwaths(i), turn);
  }
  if (route.sizeConnections() > route.sizeVectorSwaths()) {
    path += planPathForConnection(robot,
      route.getLastSwaths(), route.getLastConnection(), F2CSwaths(), turn);
  }
  return path;
}

F2CPath PathPlanning::planPath(const F2CRobot& robot,
    const F2CSwaths& swaths, TurningBase& turn) {
  F2CPath path;
  if (swaths.size() > 1) {
    for (size_t i = 0; i < swaths.size()-1; ++i) {
      path.appendSwath(swaths[i], robot.getCruiseVel());
      F2CPath turn_path = turn.createTurn(robot,
          swaths[i].endPoint(), swaths[i].getOutAngle(),
          swaths[i + 1].startPoint(), swaths[i + 1].getInAngle());
      path += turn_path.discretize(0.1);
    }
  }
  if (swaths.size() > 0) {
    path.appendSwath(swaths.back(), robot.getCruiseVel());
  }
  return path;
}

F2CPath PathPlanning::planPathForConnection(const F2CRobot& robot,
    const F2CSwaths& s1,
    const F2CMultiPoint& mp,
    const F2CSwaths& s2,
    TurningBase& turn) {
  F2CPoint p1, p2;
  double ang1, ang2;

  if (s1.size() > 0) {
    p1 = s1.back().endPoint();
    ang1 = s1.back().getOutAngle();
  } else if (mp.size() > 0) {
    p1 = mp[0];
    ang1 = mp.getOutAngle(0);
  } else {
    return {};
  }
  if (s2.size() > 0) {
    p2 = s2[0].startPoint();
    ang2 = s2[0].getInAngle();
  } else if (mp.size() > 0) {
    p2 = mp.getLastPoint();
    ang2 = mp.getInAngle(mp.size()-1);
  } else {
    return {};
  }
  return planPathForConnection(robot, p1, ang1, mp, p2, ang2, turn);
}

F2CPath PathPlanning::planPathForConnection(const F2CRobot& robot,
    const F2CPoint& p1, double ang1,
    const F2CMultiPoint& mp,
    const F2CPoint& p2, double ang2,
    TurningBase& turn) {
  auto v_con = simplifyConnection(robot,
      p1, ang1, mp, p2, ang2);

  F2CPath path;
  for (int i = 1; i < v_con.size(); ++i) {
    path += turn.createTurn(robot,
        v_con[i-1].first, v_con[i-1].second,
        v_con[i].first, v_con[i].second);
  }
  return path;
}



double PathPlanning::getSmoothTurningRadius(const F2CRobot& robot) {
  double x, y, ang, k;
  end_of_clothoid(0.0, 0.0, 0.0, 0.0, robot.getMaxDiffCurv(), 1.0,
      robot.getMaxCurv() / robot.getMaxDiffCurv(),
      &x, &y, &ang, &k);
  double xi = x - sin(ang) / robot.getMaxCurv();
  double yi = y + cos(ang) / robot.getMaxCurv();
  return sqrt(xi*xi + yi*yi);
}

std::vector<std::pair<F2CPoint, double>> PathPlanning::simplifyConnection(
    const F2CRobot& robot,
    const F2CPoint& p1, double ang1,
    const F2CMultiPoint& mp,
    const F2CPoint& p2, double ang2) {
  const double safe_dist = getSmoothTurningRadius(robot);
  std::vector<std::pair<F2CPoint, double>> path;
  path.emplace_back(p1, ang1);

  if (p1.distance(p2) < 6.0 * safe_dist || mp.size() < 2) {
    path.emplace_back(p2, ang2);
    return path;
  }

  std::vector<F2CPoint> vp;
  for (int i = 1; i < mp.size() - 1; ++i) {
    double ang_in  = (mp[i] - mp[i-1]).getAngleFromPoint();
    double ang_out = (mp[i+1] - mp[i]).getAngleFromPoint();
    if (fabs(ang_in - ang_out) > 0.1) {
      vp.emplace_back(mp[i]);
    }
  }

  if (vp.size() < 2) {
    path.emplace_back(p2, ang2);
    return path;
  }

  for (int i = 1; i < vp.size() - 1; ++i) {
    double dist_in  = vp[i].distance(vp[i-1]);
    double dist_out  = vp[i].distance(vp[i+1]);
    if (dist_in == 0.0 || dist_out == 0.0) {
      continue;
    }
    double d_in  = min(0.5 * dist_in,  safe_dist);
    double d_out  = min(0.5 * dist_out,  safe_dist);
    F2CPoint p_in =  (vp[i-1] - vp[i]) * (d_in  / dist_in)  + vp[i];
    F2CPoint p_out = (vp[i+1] - vp[i]) * (d_out / dist_out) + vp[i];
    if (p_in.distance(path.back().first) > 3.0 * safe_dist &&
        p_in.distance(p1)                > 3.0 * safe_dist &&
        p_in.distance(p2)                > 3.0 * safe_dist) {
      double ang_in   = (vp[i  ] - vp[i-1]).getAngleFromPoint();
      path.emplace_back(p_in, ang_in);
    }
    if (p_out.distance(vp[i+1]) > 3.0 * safe_dist &&
        p_out.distance(p1)                > 3.0 * safe_dist &&
        p_out.distance(p2)                > 3.0 * safe_dist &&
        p_out.distance(p_in)              > 3.0 * safe_dist) {
      double ang_out  = (vp[i+1] - vp[i]).getAngleFromPoint();
      path.emplace_back(p_out, ang_out);
    }
  }
  path.emplace_back(p2, ang2);
  return path;
}

}  // namespace f2c::pp

