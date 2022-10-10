#include "fields2cover/route_planning/custom_order.h"

namespace f2c::rp {

CustomOrder::CustomOrder() : SingleCellSwathsOrderBase() {
}

CustomOrder::CustomOrder(F2CSwaths& swaths) :
  SingleCellSwathsOrderBase(swaths) {}

CustomOrder::CustomOrder(
    F2CSwaths& swaths, const std::vector<size_t>& order) :
      SingleCellSwathsOrderBase(swaths) {
  setCustomOrder(order);
}

void CustomOrder::setCustomOrder(const std::vector<size_t>& order) {
  custom_order = order;
}

void CustomOrder::sortSwaths() {
  check();
  this->changeStartPoint();
  F2CSwaths sorted_swaths(swaths_.size());
  for (size_t i = 0; i < custom_order.size(); ++i) {
    sorted_swaths[i] = swaths_[custom_order[i]];
  }
  swaths_ = std::move(sorted_swaths);
}

void CustomOrder::check() {
  // unique vector
  std::vector<size_t> unique(custom_order.begin(), custom_order.end());
  // sort the unique vector
  std::sort(unique.begin(), unique.end());
  auto last = std::unique(unique.begin(), unique.end());
  unique.erase(last, unique.end());
  // check the sizes
  if (unique.size() != custom_order.size()) {
    throw std::invalid_argument(
        "Order vector does not contain unique elements.");
  }

  if (swaths_.size() != custom_order.size()) {
    throw std::length_error(
        "Lengths of the order vector and swaths must be the same.");
  }
  auto max_el = *std::max_element(custom_order.begin(), custom_order.end());
  if (max_el > swaths_.size() - 1) {
    throw std::invalid_argument( \
        "Custom order element is out of the swath range [" + \
        std::to_string(max_el) + "]");
  }
}

}  // namespace f2c::rp
