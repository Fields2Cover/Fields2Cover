//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_SINGLE_CELL_SWATHS_ORDER_BASE_H_
#define FIELDS2COVER_ROUTE_PLANNING_SINGLE_CELL_SWATHS_ORDER_BASE_H_

#include "fields2cover/types.h"

namespace f2c::rp {

class SingleCellSwathsOrderBase {
 public:
  virtual F2CSwaths genSortedSwaths(
      const F2CSwaths& swaths, uint32_t variant = 0) const;

  virtual ~SingleCellSwathsOrderBase() = default;

  F2CRoute genRoute(
      const F2CMultiLineString& transit_lanes,
      const F2CSwathsByCells& swaths_by_cells,
      double d_tol = 1e-4) const;

  F2CRoute genRoute(
      const std::vector<F2CMultiLineString>& transit_lanes,
      const F2CSwathsByCells& swaths_by_cells,
      double d_tol = 1e-4) const;

  F2CRoute genRoute(
      const std::vector<F2CMultiLineString>& transit_lanes,
      const F2CCells& hl_area,
      const F2CSwathsByCells& swaths_by_cells,
      double d_tol = 1e-4) const;

  F2CRoute genRoute(const F2CCells& cells, const F2CSwathsByCells& swaths,
      double d_tol = 1e-4) const;

  virtual F2CGraph2D createShortestGraph(
      const F2CMultiLineString& transit_lines,
      const F2CSwathsByCells& swaths_by_cells,
      double d_tol = 1e-4) const;

 protected:
  virtual void changeStartPoint(F2CSwaths& swaths, uint32_t variant) const;
  virtual void sortSwaths(F2CSwaths& swaths) const = 0;
};



}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_SINGLE_CELL_SWATHS_ORDER_BASE_H_
