#include "fields2cover/route_planning/custom_order.h"

namespace f2c::rp {

CustomOrder::CustomOrder(const std::vector<size_t>& order) :
  custom_order(order) {
}

CustomOrder::~CustomOrder() = default;

void CustomOrder::setCustomOrder(const std::vector<size_t>& order) {
  custom_order = order;
}


void CustomOrder::sortSwaths(F2CSwaths& swaths) const {
  check(swaths);
  F2CSwaths sorted_swaths(swaths.size());
  for (size_t i = 0; i < custom_order.size(); ++i) {
    sorted_swaths[i] = swaths[custom_order[i]];
  }
  swaths = std::move(sorted_swaths);
}

void CustomOrder::check(const F2CSwaths& swaths) const {
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

  if (swaths.size() != custom_order.size()) {
    throw std::length_error(
        "Lengths of the order vector and swaths must be the same.");
  }
  auto max_el = *std::max_element(custom_order.begin(), custom_order.end());
  if (max_el > swaths.size() - 1) {
    throw std::invalid_argument( \
        "Custom order element is out of the swath range [" + \
        std::to_string(max_el) + "]");
  }
}

}  // namespace f2c::rp
