#include "fields2cover/route_planning/custom_order.h"

namespace f2c::rp {

CustomOrder::CustomOrder(const std::vector<size_t>& order) :
  custom_order(order) {
  check(order);
}

CustomOrder::~CustomOrder() = default;

void CustomOrder::setCustomOrder(const std::vector<size_t>& order) {
  check(order);
  custom_order = order;
}


void CustomOrder::sortSwaths(F2CSwaths& swaths) const {
  if (swaths.size() != custom_order.size()) {
    throw std::length_error(
        "Lengths of the order vector and swaths must be the same.");
  }
  F2CSwaths sorted_swaths(swaths.size());
  for (size_t i = 0; i < custom_order.size(); ++i) {
    sorted_swaths[i] = swaths[custom_order[i]];
  }
  swaths = std::move(sorted_swaths);
}

void CustomOrder::check(const std::vector<size_t>& order) {
  if (order.empty()) {
    return;
  }
  std::vector<size_t> unique(order.begin(), order.end());
  std::sort(unique.begin(), unique.end());
  auto last = std::unique(unique.begin(), unique.end());
  unique.erase(last, unique.end());
  if (unique.size() != order.size()) {
    throw std::invalid_argument(
        "Order vector does not contain unique elements.");
  }
  if (unique[0] != 0 || unique.back() + 1 != unique.size()) {
    throw std::invalid_argument(
        "Order vector values should go from 0 to order.size()-1");
  }
}

}  // namespace f2c::rp
