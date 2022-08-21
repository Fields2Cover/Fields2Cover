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
  //this->changeStartPoint();

  unsigned int order_size = this->custom_order.size();

  // run for loop from 0 to order_size
  for(unsigned int i = 0; i < order_size; i++)
  {
    swaths_.at(i);
  }


}  

} // namespace f2c::rp