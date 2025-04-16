#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_REV_SPIRAL_ORDER_H_
#define FIELDS2COVER_ROUTE_PLANNING_REV_SPIRAL_ORDER_H_

#include "fields2cover/types.h"
#include "fields2cover/route_planning/spiral_order.h"

namespace f2c::rp {

class RevSpiralOrder : public SpiralOrder {
  using SpiralOrder::SpiralOrder;
 protected:
  virtual void pattern(F2CSwaths& swaths, size_t offset, size_t size) const;
};


}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_REV_SPIRAL_ORDER_H_
