//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef ROBOT_DATA_HPP_
#define ROBOT_DATA_HPP_

#include "fields2cover.h"

inline F2CRobot getSquare1Robot() {
  F2CRobot robot(1.0, 1.0);
  f2c::types::GenericImplement impl;
  impl.c_impl = F2CPoint(0.0, 0.0);
  impl.c_cov = F2CPoint(0.0, 0.0);
  impl.width = 1.0;
  impl.length = 1.0;
  impl.cov_width = 1.0;
  impl.cov_length = 1e-4;
  impl.max_icc_impl_off = 1.0;
  impl.max_icc_impl_on = 1.0;
  double sigma = 1.0;
  impl.linear_curv_change_impl_off = sigma;
  impl.linear_curv_change_impl_on = sigma;
  robot.setImpl(F2CPoint(0, 0), impl);
  return robot;
}

inline F2CRobot getSimpleRobot() {
  F2CRobot robot(1.0, 1.0);
  robot.setMaxCurv(1.0);
  robot.setMaxDiffCurv(10.0);
  return robot;
}

#endif  // ROBOT_DATA_HPP_
