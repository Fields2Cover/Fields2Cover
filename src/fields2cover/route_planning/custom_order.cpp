#include "fields2cover/route_planning/custom_order.h"

namespace f2c::rp {

CustomOrder::CustomOrder() : SingleCellSwathsOrderBase(){
}

CustomOrder::CustomOrder(F2CSwaths& swaths) : SingleCellSwathsOrderBase(swaths) {
}

CustomOrder::CustomOrder(F2CSwaths& swaths, std::vector<int> order) : SingleCellSwathsOrderBase(swaths) {
  set_custom_order(order);
}

void CustomOrder::set_custom_order(std::vector<int> order){
  this->custom_order = order;
}

void CustomOrder::sortSwaths() {
  this->changeStartPoint();
  sort_swaths(this->custom_order, swaths_);
}

void CustomOrder::reorder(F2CSwaths& vect, std::vector<std::size_t> index) {
  for (std::size_t i = 0; i < vect.size(); i++) {
    if (index[i] != i) {
      std::swap(vect[index[i]], vect[i]);
      std::swap(index[index[i]], index[i]);
    }
  }
}

void CustomOrder::sort_swaths(std::vector<int>& order_vector, F2CSwaths& swaths) {
  std::vector<std::size_t> index(order_vector.size());
  std::iota(index.begin(), index.end(), 0);
  std::sort(index.begin(), index.end(),
            [&](std::size_t a, std::size_t b) { return order_vector[a] < order_vector[b]; });
  for (size_t i = 0; i < index.size(); i++)
  {
    std::cout << index[i] << ' ';
  }
  
  reorder(swaths, index);
}

} // namespace f2c::rp