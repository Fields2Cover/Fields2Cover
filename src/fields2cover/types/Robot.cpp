//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


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

}  // namespace f2c::types

