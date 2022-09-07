#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_CUSTOM_ORDER_H_
#define FIELDS2COVER_ROUTE_PLANNING_CUSTOM_ORDER_H_

#include <vector>
#include <set>
#include <string>
#include <stdexcept>
#include "fields2cover/types.h"
#include "fields2cover/route_planning/single_cell_swaths_order_base.h"


namespace f2c::rp {

class CustomOrder : public SingleCellSwathsOrderBase {
 public:
  CustomOrder();
  CustomOrder(F2CSwaths& swaths);
  CustomOrder(F2CSwaths& swaths, const std::vector<size_t>& order);
  void setCustomOrder(const std::vector<size_t>& order);
 private:
  std::vector<size_t> custom_order;
  void check();
  void sortSwaths() override;
};


}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_CUSTOM_ORDER_H_
