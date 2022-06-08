//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/utils/random.h"

namespace f2c {

double Random::getRandomDouble(void) {
  return static_cast<double>(rand_r(&seed_)) / RAND_MAX;
}

double Random::getRandomLinear(double _min, double _max) {
  return (_max - _min) * getRandomDouble() + _min;
}

double Random::getRandomExp(double _min, double _max) {
  double log_min {log(_min)};
  return exp((log(_max) - log_min) * getRandomDouble() + log_min);
}

double Random::getAngleRandom(void) {
  return getRandomDouble() * boost::math::constants::two_pi<double>();
}

f2c::types::Field Random::generateRandField(int _n_sides, double _area,
    double _min_width, double _max_width) {
  if (_n_sides <= 2) {
    throw std::invalid_argument("Minimum 3 sides needed");
    return f2c::types::Field();
  }
  if (_area <= 0) {
    throw std::invalid_argument("The area should be positive");
    return f2c::types::Field();
  }
  f2c::types::LinearRing border;
  f2c::types::Field field;
  f2c::types::Cell cell;
  for (size_t i = 0; i < _n_sides; ++i) {
    double r = getRandomLinear(_min_width, _max_width);
    double alpha = boost::math::constants::two_pi<double>()
      * (i + getRandomDouble()) / _n_sides;
    border.addPoint(r * sin(alpha), r * cos(alpha));
  }
  border.addPoint(border.StartPoint());
  cell.addRing(border);
  field.field.addGeometry(cell);
  field.field *= sqrt(_area / field.field->get_Area());
  return field;
}

f2c::types::Field Random::genConvexField(double _area) {
  f2c::types::Field field;
  do {
    field = generateRandField(4, _area);
  } while (!field.field.isConvex());
  return field;
}

f2c::types::Field Random::genNonConvexField(double _area) {
  f2c::types::Field field;
  do {
    field = generateRandField(7, _area);
  } while (field.field.isConvex());
  return field;
}


}  // namespace f2c


