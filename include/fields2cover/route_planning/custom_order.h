#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_CUSTOM_ORDER_H_
#define FIELDS2COVER_ROUTE_PLANNING_CUSTOM_ORDER_H_

#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/route_planning/single_cell_swaths_order_base.h"

namespace f2c::rp {

class CustomOrder : public SingleCellSwathsOrderBase {
 public:
  CustomOrder();
  CustomOrder(F2CSwaths& swaths);
  CustomOrder(F2CSwaths& swaths, std::vector<int> order);
  void set_custom_order(std::vector<int> order);
 private:
  std::vector<int> custom_order;
  void sortSwaths() override;
};


}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_CUSTOM_ORDER_H_
