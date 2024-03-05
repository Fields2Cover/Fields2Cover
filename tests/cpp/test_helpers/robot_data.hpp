//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef ROBOT_DATA_HPP_
#define ROBOT_DATA_HPP_

#include "fields2cover.h"

inline F2CRobot getSimpleRobot() {
  F2CRobot robot(1.0, 1.0);
  robot.setMaxCurv(1.0);
  robot.setMaxDiffCurv(10.0);
  return robot;
}

#endif  // ROBOT_DATA_HPP_
