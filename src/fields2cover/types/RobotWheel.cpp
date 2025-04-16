//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include "fields2cover/types/RobotWheel.h"

namespace f2c::types {

void RobotWheel::setPressurePoints(const LineString& line) {
  this->pressure_points = line;
}

void RobotWheel::addPressurePoint(const Point& p) {
  this->pressure_points.addPoint(p);
}

Point RobotWheel::getPressurePoint(size_t i) const {
  return (pressure_points.size() > i) ?
    pressure_points.getGeometry(i) : Point(0, 0);
}

LineString RobotWheel::getPressurePoints() const {
  return pressure_points.isEmpty() ?
    LineString({Point(0, 0)}) : pressure_points;
}

void RobotWheel::setWidth(double w) {
  this->width = w;
}
double RobotWheel::getWidth() const {
  return this->width;
}

size_t RobotWheel::size() const {
  return pressure_points.size() > 0 ? pressure_points.size() : 1;
}

//double RobotWheel::getAngleWheel(double diff_ang_states) const {
//    return 0;
//}

}  // namespace f2c::types

