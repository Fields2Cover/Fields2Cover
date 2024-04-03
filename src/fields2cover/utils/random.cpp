//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/utils/random.h"

namespace f2c {

Random::Random(uint32_t seed) : mt_(seed) {}
Random::~Random() = default;
Random::Random(const Random&) = default;
Random& Random::operator=(const Random&) = default;
Random::Random(Random&&) = default;
Random& Random::operator=(Random&&) = default;

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

f2c::types::Cell Random::generateRandCell(double area, int n_sides,
    double min_width, double max_width) {
  if (n_sides <= 2) {
    throw std::invalid_argument("Minimum 3 sides needed");
  }
  if (area <= 0) {
    throw std::invalid_argument("The area should be positive");
  }
  f2c::types::LinearRing border;
  for (size_t i = 0; i < n_sides; ++i) {
    double r = this->getRandomLinear(min_width, max_width);
    double alpha = 2.0 * M_PI * (i + this->getRandomDouble()) / n_sides;
    border.addPoint(r * sin(alpha), r * cos(alpha));
  }
  border.addPoint(border.startPoint());
  f2c::types::Cell cell {border};
  cell *= sqrt(area / cell.area());
  return cell;
}

f2c::types::Field Random::generateRandField(double area, int n_sides,
    double min_width, double max_width) {
  return f2c::types::Field(f2c::types::Cells(
      this->generateRandCell(area, n_sides, min_width, max_width)));
}

f2c::types::Cell Random::genConvexCell(double area, size_t n_sides) {
  f2c::types::Cell cell;
  do {
    cell = this->generateRandCell(area, n_sides);
  } while (!cell.isConvex());
  return cell;
}

f2c::types::Field Random::genConvexField(double area, size_t n_sides) {
  return f2c::types::Field(f2c::types::Cells(
        this->genConvexCell(area, n_sides)));
}

f2c::types::Cell Random::genNonConvexCell(double area) {
  f2c::types::Cell cell;
  do {
    cell = this->generateRandCell(area, 9);
  } while (cell.isConvex());
  return cell;
}

f2c::types::Field Random::genNonConvexField(double area) {
  return f2c::types::Field(f2c::types::Cells(this->genNonConvexCell(area)));
}


}  // namespace f2c


