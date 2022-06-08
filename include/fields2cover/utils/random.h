//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_UTILS_RANDOM_H_
#define FIELDS2COVER_UTILS_RANDOM_H_

#include <time.h>
#include <limits>
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
  explicit Random(uint32_t _seed = static_cast<uint32_t>(time(NULL))) :
    seed_(_seed) {}

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
  double getRandomDouble(void);

  /// Get a random double in a range with uniform probability
  /// @param _min Minimum of the range
  /// @param _max Maximum of the range
  /// @return Double \f$\in [_min, _max]\f$
  double getRandomLinear(double _min, double _max);

  /// Get a random double in a range with exponential probability.
  /// Lower values are more probable than higher ones.
  /// @param _min Minimum of the range
  /// @param _max Maximum of the range
  /// @return Double \f$\in [_min, _max]\f$
  double getRandomExp(double _min, double _max);

  /// Return a random angle in radians.
  /// @return random angle \f$ \in [0, 2\pi) \f$
  double getAngleRandom(void);

  /// Random field generator to make tests.
  /// @param _n_sides Number of sides of the generated field.
  /// @param _area Area of the generated field
  /// @param _min_width Minimum distance to the center for the base field.
  /// @param _max_width Maximum distance to the center for the base field.
  /// @return Field with area _area an _n_sides.
  f2c::types::Field generateRandField(int _n_sides, double _area,
      double _min_width = 0.5, double _max_width = 1.0);

  /// Generate a convex field
  /// @param _area Area of the generated field
  f2c::types::Field genConvexField(double _area);

  /// Generate a non-convex field
  /// @param _area Area of the generated field
  f2c::types::Field genNonConvexField(double _area);

 private:
  uint32_t seed_;
};


}  // namespace f2c

#endif  // FIELDS2COVER_UTILS_RANDOM_H_
