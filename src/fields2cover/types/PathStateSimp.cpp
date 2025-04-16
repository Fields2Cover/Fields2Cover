//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <steering_functions/utilities/utilities.hpp>
#include "fields2cover/types/PathStateSimp.h"

namespace f2c::types {

PathState PathStateSimp::toPathState(
    double t, double len_state, double vel) const {
  PointState ps = this->at(t);
  PointState ps_next = this->at(t + len_state);

  PathState s;
  s.point = ps.p;
  s.velocity = vel;
  s.angle = (ps_next.p - ps.p).getAngleFromPoint();
  s.len = ps.p.distance(ps_next.p);
  if (this->isDirFront()) {
    s.dir = PathDirection::FORWARD;
  } else {
    s.angle = Point::mod_2pi(s.angle + M_PI);
    s.dir = PathDirection::BACKWARD;
  }
  s.type = (this->type == SectionSimp::STRAIGHT &&
      this->isDirFront() && using_impl) ?
      PathSectionType::SWATH : PathSectionType::TURN;
  s.using_implement = this->using_impl;
  return s;
}

std::vector<PathState> PathStateSimp::toPathStates(
    double discret_turn, double vel) const {
  std::vector<PathState> states;
  if (this->type == SectionSimp::STRAIGHT) {
    states.emplace_back(this->toPathState(0.0, 1.0, vel));
  } else {
    double D = 1.0 / ceil(fabs(this->len / discret_turn));
    for (double i = 0.0; i <= 1.0 ; i += D) {
      double dist = (1.0 - i) < D ? (1.0 - i) : D;
      if (dist > 1e-7) {
        states.emplace_back(this->toPathState(i, dist, vel));
      }
    }
  }
  states.back().angle = this->atEnd().theta;
  return states;
}


double PathStateSimp::length() const {
  if (this->type != SectionSimp::BUFF_CLOTHOID) {
    return fabs(len);
  } else {
    return fabs(len) + .5 * fabs(dist_cloth * (std::pow(len, 2) * sigma));
  }
}

PointState PathStateSimp::at(double norm_length) const {
  double x, y, theta;
  double k {0.0};
  double dir = 1.0;
  double length = norm_length * this->len;
  if (norm_length == 0.0) {
    return this->start;
  }

  switch (this->type) {
    case SectionSimp::STRAIGHT:
      end_of_straight_line(
          this->start.p.getX(), this->start.p.getY(), this->start.theta,
          dir, length, &x, &y);
      theta = this->start.theta;
      break;
    case SectionSimp::CIRCLE:
      end_of_circular_arc(
          this->start.p.getX(), this->start.p.getY(),
          this->start.theta, this->start.kurv,
          dir, length, &x, &y, &theta);
      k = this->start.kurv;
      break;
    case SectionSimp::CLOTHOID:
      end_of_clothoid(
          this->start.p.getX(), this->start.p.getY(),
          this->start.theta, this->start.kurv, this->sigma,
          dir, length, &x, &y, &theta, &k);
      break;
    case SectionSimp::BUFF_CLOTHOID:
      x = this->start.p.getX();
      y = this->start.p.getY();
      double inner_k = this->start.kurv /
        (1 - fabs(this->start.kurv * this->dist_cloth));
      double xr, yr;
      end_of_clothoid(0, 0, 0, inner_k, this->sigma, dir, length,
          &xr, &yr, &theta, &k);
      double sgn_k = fabs(k) > 1e-7 ?
          k / fabs(k) : fabs(this->start.kurv) / this->start.kurv;
      xr += sgn_k * this->dist_cloth * sin(theta);
      yr += sgn_k * this->dist_cloth * (1. - cos(theta));
      x += xr * cos(this->start.theta) - yr * sin(this->start.theta);
      y += xr * sin(this->start.theta) + yr * cos(this->start.theta);
      theta += this->start.theta;
      k = k / (fabs(k * this->dist_cloth) + 1);
      break;
  }
  PointState p_end;
  p_end.p = Point(x, y);
  p_end.theta = theta;
  p_end.kurv = k;
  return p_end;
}

PointState PathStateSimp::atEnd() const {
  return this->at(1.0);
}

void PathStateSimp::reversePath() {
  this->start = this->atEnd();
  this->start.theta = Point::mod_2pi(M_PI + this->start.theta);
  this->start.kurv *= -1.0;
}

void PathStateSimp::reverseVehicleDir() {
  this->start.theta = Point::mod_2pi(M_PI + this->start.theta);
  this->len *= -1.0;
  this->start.kurv *= -1.0;
}

void PathStateSimp::mirrorX() {
  this->start.p.setX(-this->start.p.getX());
  this->start.theta = Point::mod_2pi(M_PI - this->start.theta);
  this->start.kurv *= -1.0;
  this->sigma *= -1.0;
}

void PathStateSimp::mirrorY() {
  this->start.p.setY(-this->start.p.getY());
  this->start.theta = Point::mod_2pi(-this->start.theta);
  this->start.kurv *= -1.0;
  this->sigma *= -1.0;
}

}  // namespace f2c::types

