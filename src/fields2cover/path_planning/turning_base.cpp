//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/path_planning/turning_base.h"

namespace f2c {
namespace pp {

void TurningBase::setRobotParams(const F2CRobot& _params) {
  robot = _params;
}

std::vector<double> TurningBase::transformToNormalTurn(
    const F2CPoint& _start_pos, double _start_angle,
    const F2CPoint& _end_pos, double _end_angle) {
  double dist_start_end = _start_pos.Distance(_end_pos);
  auto dir = _end_pos - _start_pos;
  auto angle = F2CPoint::mod_2pi(dir.getAngleFromPoint());
  bool inverted {false};
  _start_angle = F2CPoint::mod_2pi(_start_angle - angle);
  _end_angle = F2CPoint::mod_2pi(_end_angle - angle);
  if (_start_angle > boost::math::constants::pi<double>()) {
    _start_angle = F2CPoint::mod_2pi(-_start_angle);
    _end_angle = F2CPoint::mod_2pi(-_end_angle);
    inverted = true;
  }
  return {dist_start_end, angle, _start_angle, _end_angle,
    static_cast<double>(inverted)};
}


F2CPath TurningBase::createTurn(const F2CPoint& _start_pos,
    double _start_angle, const F2CPoint& _end_pos, double _end_angle) {
  auto turn_values =
    transformToNormalTurn(_start_pos, _start_angle, _end_pos, _end_angle);
  double dist_start_end = turn_values[0];
  double rot_angle = turn_values[1];
  double start_angle = turn_values[2];
  double end_angle = turn_values[3];
  double inverted = turn_values[4];

  F2CPath path;
  if (using_cache) {
    path = createTurnIfNotCached(dist_start_end, start_angle, end_angle);
  } else {
    path = createSimpleTurn(dist_start_end, start_angle, end_angle);
  }
  if (!path.isValid() || path.points.size() <= 1) {return F2CPath();}

  if (inverted) {
    std::for_each(path.points.begin(), path.points.end(), [&](auto& p) {
        p.setY(-p.getY());});
    std::for_each(path.angles.begin(), path.angles.end(), [&](auto& ang) {
      ang = F2CPoint::mod_2pi(-ang) ;});
  }
  path.points = F2CPoint(0.0, 0.0).rotateFromPoint(rot_angle, path.points);

  std::for_each(path.angles.begin(), path.angles.end(), [&](auto& ang) {
      ang = F2CPoint::mod_2pi(ang + rot_angle);});

  std::for_each(path.points.begin(), path.points.end(), [&](auto& p) {
      p = p + _start_pos;});

  correctPath(path, _start_pos, _end_pos);
  return path;
}

void TurningBase::correctPath(F2CPath& _path, const F2CPoint& _start_pos,
    const F2CPoint& _end_pos, float _max_error_dist) {
  if (_path.points.size() < 2) {return;}

  auto is_near = [&](const F2CPoint& a, const F2CPoint& b) {
      return (a.Distance(b) < _max_error_dist);
  };
  if (is_near(_path.points[0], _start_pos)) {
    _path.points[0] = _start_pos;
  }
  if (is_near(_path.points.back(), _end_pos)) {
    _path.points.back() = _end_pos;
  }
}


F2CPath TurningBase::createTurnIfNotCached(double _dist_start_end,
    double _start_angle, double _end_angle) {
  std::vector<int> v_turn {
        static_cast<int>(1e3 * _dist_start_end),
        static_cast<int>(1e3 * _start_angle),
        static_cast<int>(1e3 * _end_angle)
  };
  if (max_headland_width < 1e5) {
    v_turn.emplace_back(static_cast<int>(1e3 * max_headland_width));
  }

  auto it = path_cache_.find(v_turn);
  if (it != path_cache_.end()) {
    return it->second.clone();
  }
  auto path = createSimpleTurn(_dist_start_end, _start_angle, _end_angle);
  path_cache_.insert({v_turn, path.clone()});
  return path;
}


bool TurningBase::isTurnValid(const types::Path& path,
    double dist_start_end, double end_angle,
    double max_dist_error, double max_rot_error) {
  if ((path.points.back().getX() > dist_start_end + max_dist_error) ||
      (path.points.back().getX() < dist_start_end - max_dist_error)) {
    std::cout << "Turn not valid. X = " <<
      path.points.back().getX() << std::endl;
    return false;
  }
  if ((path.points.back().getY() > + max_dist_error) ||
      (path.points.back().getY() < - max_dist_error)) {
    std::cout << "Turn not valid. Y = " <<
      path.points.back().getY() << std::endl;
    return false;
  }
  if (cos(path.angles.back() - end_angle) <= 1 - max_rot_error) {
    std::cout << "Turn not valid. Ang = " << path.angles.back() << std::endl;
    return false;
  }
  for (auto&& p : path.points) {
    if (p.getY() < -max_dist_error) {
      std::cout << "Turn not valid. Y Neg = " << p.getY() << std::endl;
      return false;
    }
  }
  std::cout << "Turn VALID." << std::endl;
  return true;
}

}  // namespace pp
}  // namespace f2c
