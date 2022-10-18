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
  explicit CustomOrder(
      const std::vector<size_t>& order = std::vector<size_t>());
  ~CustomOrder();
  void setCustomOrder(const std::vector<size_t>& order);

 protected:
  void sortSwaths(F2CSwaths& swaths) const override;

 private:
  std::vector<size_t> custom_order;
  void check(const F2CSwaths& swaths) const;
};


}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_CUSTOM_ORDER_H_
