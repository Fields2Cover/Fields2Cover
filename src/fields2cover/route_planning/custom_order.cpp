#include "fields2cover/route_planning/custom_order.h"

namespace f2c::rp {

CustomOrder::CustomOrder(const std::vector<size_t>& order) :
  custom_order(order) {
  this->check(order);
}

CustomOrder::~CustomOrder() = default;

void CustomOrder::setCustomOrder(const std::vector<size_t>& order) {
  this->check(order);
  custom_order = order;
}


void CustomOrder::sortSwaths(F2CSwaths& swaths) const {
  F2CSwaths sorted_swaths(swaths.size());
  size_t last_bulk_id {0};
  for (size_t i = 0; i < swaths.size(); ++i) {
    size_t bulk_id = (i / custom_order.size()) * custom_order.size();
    size_t i_b = i % custom_order.size();

    while (bulk_id + custom_order[i_b + last_bulk_id] >= swaths.size()) {
      ++last_bulk_id;
    }
    sorted_swaths[i] = swaths[bulk_id + custom_order[i_b + last_bulk_id]];
  }
  swaths = std::move(sorted_swaths);
}

void CustomOrder::check(const std::vector<size_t>& order) {
  if (order.size() == 0) {
    return;
  }

  // unique vector
  std::vector<size_t> unique(order.begin(), order.end());
  // sort the unique vector
  std::sort(unique.begin(), unique.end());
  auto last = std::unique(unique.begin(), unique.end());
  unique.erase(last, unique.end());
  // check the sizes
  if (unique.size() != order.size()) {
    throw std::invalid_argument(
        "Order vector does not contain unique elements.");
  }

  if (unique.back() + 1 != unique.size() || unique[0] != 0) {
    throw std::invalid_argument(
        "Order vector values should go from 0 to order.size()-1");
  }
}

}  // namespace f2c::rp
