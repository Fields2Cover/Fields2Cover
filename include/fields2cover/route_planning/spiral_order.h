#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_SPIRAL_ORDER_H_
#define FIELDS2COVER_ROUTE_PLANNING_SPIRAL_ORDER_H_

#include "fields2cover/types.h"
#include "fields2cover/route_planning/single_cell_swaths_order_base.h"

namespace f2c::rp {

class SpiralOrder : public SingleCellSwathsOrderBase {
 public:
  SpiralOrder();
  SpiralOrder(F2CSwaths& swaths);
  SpiralOrder(F2CSwaths& swaths, int sp_size);
  ~SpiralOrder();
  void setSpiralSize(int sp_size);

 private:
  int spiral_size;
  void sortSwaths() override;
  void spiral(size_t offset, int size);
};


}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_SPIRAL_ORDER_H_
