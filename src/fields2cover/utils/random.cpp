//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/utils/random.h"

namespace f2c {

double Random::getRandomDouble() {
  return getRandomLinear(0.0, 1.0);
}

double Random::getRandomLinear(double min, double max) {
  assert(min < max);

  std::uniform_real_distribution<double> distribution(min, max);
  return distribution(mt_);
}

double Random::getRandomExp(double min, double max) {
  assert(min < max);

  double log_min {log(min)};
  return exp((log(max) - log_min) * getRandomDouble() + log_min);
}

double Random::getRandomExpDist(double lambda) {
  std::exponential_distribution<double> distribution(lambda);
  return distribution(mt_);
}

double Random::getAngleRandom() {
  return getRandomLinear(0.0, boost::math::constants::two_pi<double>());
}

f2c::types::Field Random::generateRandField(int n_sides, double area,
    double min_width, double max_width) {
  if (n_sides <= 2) {
    throw std::invalid_argument("Minimum 3 sides needed");
  }
  if (area <= 0) {
    throw std::invalid_argument("The area should be positive");
  }
  f2c::types::LinearRing border;
  f2c::types::Field field;
  f2c::types::Cell cell;
  for (size_t i = 0; i < n_sides; ++i) {
    double r = getRandomLinear(min_width, max_width);
    double alpha = boost::math::constants::two_pi<double>()
      * (i + getRandomDouble()) / n_sides;
    border.addPoint(r * sin(alpha), r * cos(alpha));
  }
  border.addPoint(border.StartPoint());
  cell.addRing(border);
  field.field.addGeometry(cell);
  field.field *= sqrt(area / field.field->get_Area());
  return field;
}

f2c::types::Field Random::genConvexField(double area, size_t n_sides) {
  f2c::types::Field field;
  do {
    field = generateRandField(n_sides, area);
  } while (!field.field.isConvex());
  return field;
}

f2c::types::Field Random::genNonConvexField(double area) {
  f2c::types::Field field;
  do {
    field = generateRandField(9, area);
  } while (field.field.isConvex());
  return field;
}


}  // namespace f2c


