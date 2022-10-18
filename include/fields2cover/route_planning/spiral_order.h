#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_SPIRAL_ORDER_H_
#define FIELDS2COVER_ROUTE_PLANNING_SPIRAL_ORDER_H_

#include "fields2cover/types.h"
#include "fields2cover/route_planning/single_cell_swaths_order_base.h"

namespace f2c::rp {

class SpiralOrder : public SingleCellSwathsOrderBase {
 public:
  explicit SpiralOrder(size_t sp_size = 2);
  ~SpiralOrder();
  void setSpiralSize(size_t sp_size);

 protected:
  void sortSwaths(F2CSwaths& swaths) const override;

 private:
  size_t spiral_size;
  void spiral(F2CSwaths& swaths, size_t offset, size_t size) const;
};


}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_SPIRAL_ORDER_H_
