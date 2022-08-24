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
  CustomOrder(F2CSwaths& swaths, std::vector<int> order);
  void set_custom_order(std::vector<int> order);
  void sort_swaths(std::vector<int>& keyvector, F2CSwaths& swaths);
  //int order_size;
 private:
  std::vector<int> custom_order;
  void sortSwaths() override;
  void reorder(F2CSwaths& vect, std::vector<std::size_t> index);
};


}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_CUSTOM_ORDER_H_
