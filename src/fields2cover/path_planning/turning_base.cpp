//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/path_planning/turning_base.h"

namespace f2c {
namespace pp {

void TurningBase::setRobotParams(const F2CRobot& params) {
  robot = params;
}

std::vector<double> TurningBase::transformToNormalTurn(
    const F2CPoint& start_pos, double start_angle,
    const F2CPoint& end_pos, double end_angle) {
  double dist_start_end = start_pos.Distance(end_pos);
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


F2CPath TurningBase::createTurn(const F2CPoint& start_pos,
    double start_angle, const F2CPoint& end_pos, double end_angle) {
  auto turn_values =
    transformToNormalTurn(start_pos, start_angle, end_pos, end_angle);
  double dist_start_end = turn_values[0];
  double rot_angle = turn_values[1];
  double start_angle_t = turn_values[2];
  double end_angle_t = turn_values[3];
  double inverted = turn_values[4];

  F2CPath path;
  if (using_cache) {
    path = createTurnIfNotCached(dist_start_end, start_angle_t, end_angle_t);
  } else {
    path = createSimpleTurn(dist_start_end, start_angle_t, end_angle_t);
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
      p = p + start_pos;});

  correctPath(path, start_pos, end_pos);
  return path;
}

void TurningBase::correctPath(F2CPath& path, const F2CPoint& start_pos,
    const F2CPoint& end_pos, float max_error_dist) {
  if (path.points.size() < 2) {return;}

  auto is_near = [&](const F2CPoint& a, const F2CPoint& b) {
      return (a.Distance(b) < max_error_dist);
  };
  if (is_near(path.points[0], start_pos)) {
    path.points[0] = start_pos;
  }
  if (is_near(path.points.back(), end_pos)) {
    path.points.back() = end_pos;
  }
}


F2CPath TurningBase::createTurnIfNotCached(double dist_start_end,
    double start_angle, double end_angle) {
  std::vector<int> v_turn {
        static_cast<int>(1e3 * dist_start_end),
        static_cast<int>(1e3 * start_angle),
        static_cast<int>(1e3 * end_angle)
  };
  if (max_headland_width < 1e5) {
    v_turn.emplace_back(static_cast<int>(1e3 * max_headland_width));
  }

  auto it = path_cache_.find(v_turn);
  if (it != path_cache_.end()) {
    return it->second.clone();
  }
  auto path = createSimpleTurn(dist_start_end, start_angle, end_angle);
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
