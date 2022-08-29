#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_CUSTOM_ORDER_H_
#define FIELDS2COVER_ROUTE_PLANNING_CUSTOM_ORDER_H_

#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/route_planning/single_cell_swaths_order_base.h"
#include <algorithm>
#include <numeric>
#include <cassert>
#include <variant>

namespace f2c::rp {

class CustomOrder : public SingleCellSwathsOrderBase {
 public:
  CustomOrder();
  CustomOrder(F2CSwaths& swaths);
  CustomOrder(F2CSwaths& swaths, std::vector<size_t> order);
  void set_custom_order(std::vector<size_t> order);
 private:
  std::vector<size_t> custom_order;
  void sort_swaths(F2CSwaths& swaths, std::vector<size_t>& order);
  void sortSwaths() override;
};


}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_CUSTOM_ORDER_H_
