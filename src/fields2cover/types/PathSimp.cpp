//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/PathSimp.h"

namespace f2c::types {

const PathStateSimp& PathSimp::operator[](size_t idx) const {
  return this->states[idx];
}

PathStateSimp& PathSimp::operator[](size_t idx) {
  return this->states[idx];
}

PathSimp& PathSimp::operator+=(const PathSimp& path) {
  if (this->states.size() > 0) {
    auto end_this = this->atEnd();
    double dist = end_this.p.distance(path.states[0].start.p);
    if (dist > 1e-5) {
      double ang_points = (path.states[0].start.p - end_this.p)
        .getAngleFromPoint();
      if (Point::getAngleDiffAbs(ang_points, end_this.theta) > 0.5 * M_PI) {
        dist *= -1.0;
      }
      this->appendStraightLine(dist);
    }
  }
  for (auto&& state : path.states) {
    this->states.emplace_back(state);
  }
  return *this;
}

Path PathSimp::toPath(double discret, double vel) const {
  Path path;
  for (auto&& s : this->states) {
    std::vector<PathState> new_states = s.toPathStates(discret, vel);
    path.getStates().insert(path.getStates().end(), new_states.begin(), new_states.end());
  }
  return path;
}

PathSimp& PathSimp::appendStraightLine(double length, bool using_impl) {
  PathStateSimp state;
  state.start = this->atEnd();
  state.type = SectionSimp::STRAIGHT;
  state.len = length;
  state.using_impl = using_impl;
  this->states.emplace_back(state);
  return *this;
}

PathSimp& PathSimp::prependStraightLine(double length, bool using_impl) {
  PathStateSimp state;
  if (this->states.size() > 0) {
    state.start = this->states[0].start;
  }
  state.type = SectionSimp::STRAIGHT;
  state.len = -length;

  // Change dir
  state.start = state.atEnd();
  state.len = length;
  state.using_impl = using_impl;

  // this->states.insert(this->states.begin(), state); // Returns Seg fault
  if (this->states.size() > 0) {
    this->states.push_back(states.back());
    for (size_t i = this->states.size() - 1; i > 0; --i) {
      this->states[i] = this->states[i - 1];
    }
    this->states[0] = state;
  } else {
    this->states.emplace_back();
  }
  return *this;
}

PathSimp& PathSimp::appendCircle(double length, bool using_impl) {
  PathStateSimp state;
  state.start = this->atEnd();
  state.type = SectionSimp::CIRCLE;
  state.len = length;
  state.using_impl = using_impl;
  this->states.emplace_back(state);
  return *this;
}

PathSimp& PathSimp::appendCircleKurv(
    double length, double max_kurv, bool using_impl) {
  PathStateSimp state;
  state.start = this->atEnd();
  state.start.kurv = max_kurv;
  state.type = SectionSimp::CIRCLE;
  state.len = length;
  state.using_impl = using_impl;
  this->states.emplace_back(state);
  return *this;
}

PathSimp& PathSimp::appendClothoid(
    double length, double sigma, bool using_impl) {
  PathStateSimp state;
  state.start = this->atEnd();
  state.type = SectionSimp::CLOTHOID;
  state.len = length;
  state.sigma = sigma;
  state.using_impl = using_impl;
  this->states.emplace_back(state);
  return *this;
}

PathSimp& PathSimp::appendBuffCloth(
    double length, double sigma, double dist_cloth, bool using_impl) {
  PathStateSimp state;
  state.start = this->atEnd();
  state.type = SectionSimp::BUFF_CLOTHOID;
  state.len = (length);
  state.sigma = sigma;
  state.dist_cloth = dist_cloth;
  state.using_impl = using_impl;
  this->states.emplace_back(state);
  return *this;
}

PathSimp& PathSimp::appendSmoothCircularSegment(
    double ang_diff, double max_kurv, double max_sigma, double buff_cloth,
    bool using_impl) {
  this->appendStartHalfSmoothCircularSegment(
      ang_diff, max_kurv, max_sigma, buff_cloth, using_impl);
  this->appendEndHalfSmoothCircularSegment(
      ang_diff, max_kurv, max_sigma, buff_cloth, using_impl);
  return *this;
}

PathSimp& PathSimp::appendStartHalfSmoothCircularSegment(
    double ang_diff, double max_kurv, double max_sigma, double buff_cloth,
    bool using_impl) {
  double theta = fabs(0.5 * max_kurv * max_kurv / max_sigma);
  if (theta > fabs(0.5 * ang_diff)) {
    max_kurv = sqrt(fabs(max_sigma * ang_diff));
  }
  double len = fabs(max_kurv/max_sigma);
  if (buff_cloth == 0.0) {
    this->appendClothoid(len, max_sigma, using_impl);
  } else {
    this->appendBuffCloth(len, max_sigma, buff_cloth, using_impl);
  }
  if (theta < fabs(0.5*ang_diff)) {
    this->appendCircle(0.5 * fabs((ang_diff - 2.0 * theta) /
          this->atEnd().kurv), using_impl);
  }
  return *this;
}

PathSimp& PathSimp::appendEndHalfSmoothCircularSegment(
    double ang_diff, double max_kurv, double max_sigma, double buff_cloth,
    bool using_impl) {
  double theta = fabs(0.5 * max_kurv * max_kurv / max_sigma);
  if (theta > fabs(0.5 * ang_diff)) {
    max_kurv = sqrt(fabs(max_sigma * ang_diff));
  }
  double len = fabs(max_kurv/max_sigma);
  if (theta < fabs(0.5*ang_diff)) {
    this->appendCircle(0.5 * fabs((ang_diff - 2.0 * theta) /
          this->atEnd().kurv), using_impl);
  }
  if (buff_cloth == 0.0) {
    this->appendClothoid(len, -max_sigma, using_impl);
  } else {
    this->appendBuffCloth(len, -max_sigma, buff_cloth, using_impl);
  }
  return *this;
}

PathSimp& PathSimp::appendClassicSmoothCircularSegment(
    double ang_diff, double max_kurv, double max_sigma, bool using_impl) {
  double theta = fabs(0.5 * max_kurv * max_kurv / max_sigma);
  double len = fabs(max_kurv/max_sigma);
  this->appendClothoid(len, max_sigma, using_impl);
  this->appendCircle(
      (Point::mod_2pi(ang_diff - 2.0 * theta)/max_kurv), using_impl);
  this->appendClothoid(len, -max_sigma, using_impl);
  return *this;
}

size_t PathSimp::size() const {
  return this->states.size();
}


PathSimp& PathSimp::moveTo(const Point& new_p_start) {
  for (auto&& s : this->states) {
    s.start.p = s.start.p + new_p_start;
  }
  return *this;
}

PathSimp& PathSimp::rotate(const Point& rot_point, double ang) {
  for (auto&& s : this->states) {
    s.start.p = rot_point.rotateFromPoint(ang, s.start.p);
    s.start.theta = Point::mod_2pi(s.start.theta + ang);
  }
  return *this;
}


PointState PathSimp::atStart() const {
  if (this->states.size() > 0) {
    return this->states[0].start;
  }
  return {};
}

PointState PathSimp::atEnd() const {
  if (this->states.size() > 0) {
    return this->states.back().atEnd();
  }
  return {};
}

PointState PathSimp::at(double t) const {
  if (this->states.size() == 0) {
    return this->atStart();
  }
  double len = this->length();
  if (t >= len) {
    return this->atEnd();
  }
  size_t i = 0;
  double s_len = this->states[i].length();
  while (t > s_len) {
    i++;
    t -= s_len;
    s_len = this->states[i].length();
  }
  return this->states[i].at(t/s_len);
}



void PathSimp::reverseVehicleDir() {
  for (size_t i = 0; i < this->states.size(); ++i) {
    this->states[i].reverseVehicleDir();
  }
}

void PathSimp::mirrorX() {
  for (auto&& s : this->states) {
    s.mirrorX();
  }
}

void PathSimp::mirrorY() {
  for (auto&& s : this->states) {
    s.mirrorY();
  }
}

void PathSimp::setImplOn() {
  for (auto&& s : this->states) {
    s.using_impl = true;
  }
}

void PathSimp::setImplOff() {
  for (auto&& s : this->states) {
    s.using_impl = false;
  }
}

double PathSimp::length() const {
  double len {0.0};
  for (auto&& s : states) {
    len += s.length();
  }
  return len;
}


}  // namespace f2c::types

