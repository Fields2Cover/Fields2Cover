//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include <algorithm>
#include <steering_functions/utilities/utilities.hpp>
#include "fields2cover/types/Robot.h"

namespace f2c::types {

Robot::Robot(double width, double cov_width,
    double max_curv, double max_diff_curv) :
      width_(width), cov_width_(cov_width),
      max_icc_(max_curv), linear_curv_change_(max_diff_curv) {
  if (width <= 0.0 || cov_width < 0.0) {
    throw std::out_of_range("Robot widths have to be greater than 0.");
  }
  if (cov_width == 0.0) {
    this->cov_width_ = this->width_;
  }
}

Robot::Robot() = default;
Robot::~Robot() = default;
Robot::Robot(const Robot&) = default;
Robot::Robot(Robot &&) = default;
Robot& Robot::operator=(const Robot&) = default;
Robot& Robot::operator=(Robot&&) = default;



std::string Robot::getName() const {
  return this->name_;
}

void Robot::setName(const std::string& str) {
  this->name_ = str;
}

double Robot::getWidth() const {
  return this->width_;
}

void Robot::setWidth(double w) {
  this->width_ = w;
}

double Robot::getCovWidth() const {
  return this->cov_width_;
}

void Robot::setCovWidth(double w) {
  this->cov_width_ = w;
}

double Robot::getCruiseVel() const {
  return this->cruise_speed_;
}

void Robot::setCruiseVel(double v) {
  this->cruise_speed_ = v;
}

double Robot::getTurnVel() const {
  return this->turn_vel_ ? *this->turn_vel_ : this->cruise_speed_;
}

void Robot::setTurnVel(double v) {
  this->turn_vel_ = v;
}

double Robot::getMinTurningRadius() const {
  return 1. / (this->max_icc_ + 1e-7);
}

void Robot::setMinTurningRadius(double rad) {
  this->max_icc_ = 1.0 / (fabs(rad) + 1e-7);
}

double Robot::getMaxCurv() const {
  return this->max_icc_;
}

void Robot::setMaxCurv(double c) {
  this->max_icc_ = fabs(c);
}

double Robot::getMaxDiffCurv() const {
  return this->linear_curv_change_;
}

void Robot::setMaxDiffCurv(double dc) {
  this->linear_curv_change_ = fabs(dc);
}

double Robot::getSmoothTurningRadius() const {
  double x, y, ang, k;
  end_of_clothoid(0.0, 0.0, 0.0, 0.0, this->getMaxDiffCurv(), 1.0,
      this->getMaxCurv() / this->getMaxDiffCurv(),
      &x, &y, &ang, &k);
  double xi = x - sin(ang) / this->getMaxCurv();
  double yi = y + cos(ang) / this->getMaxCurv();
  return sqrt(xi * xi + yi * yi);
}

double Robot::getTurnRadius(double sweep, bool continuous) const {
  const double min_radius = this->getMinTurningRadius();
  const double rate = this->getMaxDiffCurv();
  if (!continuous || rate <= 0.0) {
    return min_radius;
  }
  double radius = std::max(min_radius, this->getSmoothTurningRadius());
  if (sweep > 0.0) {
    radius = std::max(radius, 1.0 / sqrt(rate * sweep));
  }
  return radius;
}

double Robot::getMaxCornerCut() const {
  if (this->max_corner_cut_) {
    return *this->max_corner_cut_;
  }
  return std::max(
      std::min(this->getMinTurningRadius(), this->cov_width_),
      0.5 * this->cov_width_);
}

void Robot::setMaxCornerCut(double d) {
  this->max_corner_cut_ = fabs(d);
}

double Robot::getDirectHopMaxDev() const {
  return this->direct_hop_max_dev_ ?
      *this->direct_hop_max_dev_ : 0.25 * this->cov_width_;
}

void Robot::setDirectHopMaxDev(double d) {
  this->direct_hop_max_dev_ = fabs(d);
}

double Robot::getUturnMaxHop() const {
  return this->uturn_max_hop_ ?
      *this->uturn_max_hop_ : 3.0 * this->cov_width_;
}

void Robot::setUturnMaxHop(double d) {
  this->uturn_max_hop_ = fabs(d);
}

double Robot::getTrackSimplifyTol() const {
  return this->track_simplify_tol_ ?
      *this->track_simplify_tol_ : 0.1 * this->cov_width_;
}

void Robot::setTrackSimplifyTol(double d) {
  this->track_simplify_tol_ = fabs(d);
}

double Robot::getReversalSweep() const {
  return this->reversal_sweep_;
}

void Robot::setReversalSweep(double a) {
  this->reversal_sweep_ = fabs(a);
}

double Robot::getTurnSlack() const {
  return this->turn_slack_;
}

void Robot::setTurnSlack(double a) {
  this->turn_slack_ = fabs(a);
}

double Robot::getMinSweep() const {
  return this->min_sweep_;
}

void Robot::setMinSweep(double a) {
  this->min_sweep_ = fabs(a);
}

double Robot::getLegShare() const {
  return this->leg_share_;
}

void Robot::setLegShare(double s) {
  this->leg_share_ = fabs(s);
}

double Robot::getRadiusMargin() const {
  return this->radius_margin_;
}

void Robot::setRadiusMargin(double m) {
  this->radius_margin_ = fabs(m);
}

double Robot::getMinBackoffRadii() const {
  return this->min_backoff_radii_;
}

void Robot::setMinBackoffRadii(double r) {
  this->min_backoff_radii_ = fabs(r);
}

double Robot::getUturnReachRadii() const {
  return this->uturn_reach_radii_;
}

void Robot::setUturnReachRadii(double r) {
  this->uturn_reach_radii_ = fabs(r);
}

size_t Robot::getMaxCornerSpan() const {
  return this->max_corner_span_;
}

void Robot::setMaxCornerSpan(size_t n) {
  this->max_corner_span_ = n;
}

}  // namespace f2c::types

