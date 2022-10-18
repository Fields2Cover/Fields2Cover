//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_UTILS_RANDOM_H_
#define FIELDS2COVER_UTILS_RANDOM_H_

#include <time.h>
#include <limits>
#include <random>
#include <boost/math/constants/constants.hpp>
#include "fields2cover/types/Point.h"
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/LinearRing.h"
#include "fields2cover/types/Field.h"
#include "fields2cover/types/Cell.h"

namespace f2c {

class Random {
 public:
  /// Constructor to initialize the seed to a known value to make experiments
  /// reproducible
  explicit Random(uint32_t seed = static_cast<uint32_t>(time(NULL))) :
    mt_(seed) {}

  /// @cond DOXYGEN_SHOULD_SKIP_THIS
  ~Random() = default;
  Random(const Random&) = default;
  Random &operator=(const Random&) = default;
  Random(Random&&) = default;
  Random &operator=(Random&&) = default;
  /// @endcond

 public:
  /// Get a random double \f$\in [0, 1]\f$
  /// @return Double \f$\in [0, 1]\f$
  double getRandomDouble();

  /// Get a random double in a range with uniform probability
  /// @param min Minimum of the range
  /// @param max Maximum of the range
  /// @return Double \f$\in [min, max]\f$
  double getRandomLinear(double min, double max);

  /// Get a random double in a range with exponential probability.
  /// Lower values are more probable than higher ones.
  /// @param min Minimum of the range
  /// @param max Maximum of the range
  /// @return Double \f$\in [min, max]\f$
  double getRandomExp(double min, double max);

  /// Get a random double in a range [0;+inf) with exponential
  /// probability.
  /// Lower values are more probable than higher ones.
  /// @param lambda is the rate parameter
  /// @return Double > 0
  double getRandomExpDist(double lambda);

  /// Return a random angle in radians.
  /// @return random angle \f$ \in [0, 2\pi) \f$
  double getAngleRandom();

  /// Random field generator to make tests.
  /// @param n_sides Number of sides of the generated field.
  /// @param area Area of the generated field
  /// @param min_width Minimum distance to the center for the base field.
  /// @param max_width Maximum distance to the center for the base field.
  /// @return Field with area area an n_sides.
  f2c::types::Field generateRandField(int n_sides, double area,
      double min_width = 0.5, double max_width = 1.0);

  /// Generate a convex field
  /// @param area Area of the generated field
  f2c::types::Field genConvexField(double area, size_t n_sides = 4);

  /// Generate a non-convex field
  /// @param area Area of the generated field
  f2c::types::Field genNonConvexField(double area);

 private:
  std::mt19937 mt_;
};


}  // namespace f2c

#endif  // FIELDS2COVER_UTILS_RANDOM_H_
