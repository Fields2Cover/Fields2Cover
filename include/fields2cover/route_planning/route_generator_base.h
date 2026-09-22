//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_ROUTE_GENERATOR_BASE_H_
#define FIELDS2COVER_ROUTE_PLANNING_ROUTE_GENERATOR_BASE_H_

#include "fields2cover/types.h"

namespace f2c::rp {

/// Common interface of everything that turns swaths into a route.
class RouteGeneratorBase {
 public:
  /// Generate a route covering the swaths, connected through the headland.
  ///
  /// @param cells Headland swath rings used to travel between swaths
  /// @param swaths_by_cells Swaths to be covered, kept apart per cell
  /// @param d_tol Tolerance distance to consider if two points are the same.
  /// @return Route that covers all the swaths
  virtual F2CRoute genRoute(
      const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
      double d_tol = 1e-4) const = 0;

  virtual ~RouteGeneratorBase() = default;
};


}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_ROUTE_GENERATOR_BASE_H_
