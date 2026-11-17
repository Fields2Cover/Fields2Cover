//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_SINGLE_CELL_SWATHS_ORDER_BASE_H_
#define FIELDS2COVER_ROUTE_PLANNING_SINGLE_CELL_SWATHS_ORDER_BASE_H_

#include <optional>
#include "fields2cover/types.h"
#include "fields2cover/route_planning/route_generator_base.h"

namespace f2c::rp {

class SingleCellSwathsOrderBase : public RouteGeneratorBase {
 public:
  virtual F2CSwaths genSortedSwaths(
      const F2CSwaths& swaths, uint32_t variant = 0) const;

  /// Sort each cell's swaths on their own, so an order never runs across cells.
  virtual F2CSwathsByCells genSortedSwaths(
      const F2CSwathsByCells& swaths, uint32_t variant = 0) const;

  /// Route covering the swaths in this order, connected through the headland.
  ///
  /// @param cells Headland swath rings used to travel between swaths
  /// @param swaths Swaths to be covered, kept apart per cell
  /// @param d_tol Tolerance distance to consider if two points are the same.
  F2CRoute genRoute(const F2CCells& cells, const F2CSwathsByCells& swaths,
      double d_tol = 1e-4) const override;

  /// Set the point the route starts from and returns to.
  ///
  /// Without it the route starts at the cell the swaths came in first, which
  /// says nothing about where the machine actually stands.
  void setStartAndEndPoint(const F2CPoint& p);

  virtual ~SingleCellSwathsOrderBase() = default;

 protected:
  std::optional<F2CPoint> r_start_end;

  virtual void changeStartPoint(F2CSwaths& swaths, uint32_t variant) const;
  virtual void sortSwaths(F2CSwaths& swaths) const = 0;
};



}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_SINGLE_CELL_SWATHS_ORDER_BASE_H_
