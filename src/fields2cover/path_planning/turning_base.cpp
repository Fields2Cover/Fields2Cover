//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/path_planning/turning_base.h"

namespace f2c::pp {

std::vector<double> TurningBase::transformToNormalTurn(
    const F2CPoint& start_pos, double start_angle,
    const F2CPoint& end_pos, double end_angle) {
  double dist_start_end = start_pos.distance(end_pos);
  auto dir = end_pos - start_pos;
  auto angle = F2CPoint::mod_2pi(dir.getAngleFromPoint());
  bool inverted {false};
  start_angle = F2CPoint::mod_2pi(start_angle - angle);
  end_angle = F2CPoint::mod_2pi(end_angle - angle);
  if (start_angle > boost::math::constants::pi<double>()) {
    start_angle = F2CPoint::mod_2pi(-start_angle);
    end_angle = F2CPoint::mod_2pi(-end_angle);
    inverted = true;
  }
  return {dist_start_end, angle, start_angle, end_angle,
    static_cast<double>(inverted)};
}


F2CPath TurningBase::createTurn(const F2CRobot& robot,
    const F2CPoint& start_pos, double start_angle,
    const F2CPoint& end_pos, double end_angle) {
  auto turn_values =
    transformToNormalTurn(start_pos, start_angle, end_pos, end_angle);
  double dist_start_end = turn_values[0];
  double rot_angle = turn_values[1];
  double start_angle_t = turn_values[2];
  double end_angle_t = turn_values[3];
  double inverted = turn_values[4];

  F2CPath path;
  if (using_cache) {
    path = createTurnIfNotCached(robot,
        dist_start_end, start_angle_t, end_angle_t);
  } else {
    path = createSimpleTurn(robot,
        dist_start_end, start_angle_t, end_angle_t);
  }
  if (path.size() <= 1) {return F2CPath();}

  if (inverted) {
    std::for_each(path.begin(), path.end(), [] (auto& s) {
        s.point.setY(-s.point.getY());
        s.angle = F2CPoint::mod_2pi(-s.angle);});
  }
  for (auto&& s : path) {
    s.point = F2CPoint(.0, .0).rotateFromPoint(rot_angle, s.point) + start_pos;
    s.angle = F2CPoint::mod_2pi(s.angle + rot_angle);
  }

  correctPath(path, start_pos, end_pos);
  return path;
}

void TurningBase::correctPath(F2CPath& path, const F2CPoint& start_pos,
    const F2CPoint& end_pos, float max_error_dist) {
  if (path.size() < 2) {return;}

  auto is_near = [max_error_dist](const F2CPoint& a, const F2CPoint& b) {
      return (a.distance(b) < max_error_dist);
  };
  if (is_near(path[0].point, start_pos)) {
    path[0].point = start_pos;
  }
  if (is_near(path.back().point, end_pos)) {
    path.back().point = end_pos;
  }
}


F2CPath TurningBase::createTurnIfNotCached(const F2CRobot& robot,
    double dist_start_end, double start_angle, double end_angle) {
  std::vector<int> v_turn {
        static_cast<int>(1e3 * robot.getMaxCurv()),
        static_cast<int>(1e3 * robot.getMaxDiffCurv()),
        static_cast<int>(1e3 * dist_start_end),
        static_cast<int>(1e3 * start_angle),
        static_cast<int>(1e3 * end_angle)
  };
  auto it = path_cache_.find(v_turn);
  if (it != path_cache_.end()) {
    return it->second;
  }
  auto path = createSimpleTurn(robot, dist_start_end, start_angle, end_angle);
  path_cache_.insert({v_turn, path});
  return path;
}


bool TurningBase::isTurnValid(const types::Path& path,
    double dist_start_end, double end_angle,
    double max_dist, double max_rot_error) {
  for (auto&& s : path) {
    if (s.point.getY() < -max_dist) {
      return false;
    }
  }
  F2CPoint p_end = path.atEnd();
  return (cos(path.back().angle - end_angle) >= 1 - max_rot_error) &&
    (fabs(p_end.getX() - dist_start_end) < max_dist) &&
    (fabs(p_end.getY()) < max_dist);
}

double TurningBase::getDiscretization() const {
  return this->discretization;
}

void TurningBase::setDiscretization(double d) {
  this->discretization = d;
}

bool TurningBase::getUsingCache() const {
  return this->using_cache;
}

void TurningBase::setUsingCache(bool c) {
  this->using_cache = c;
}


}  // namespace f2c::pp

