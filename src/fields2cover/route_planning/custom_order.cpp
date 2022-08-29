#include "fields2cover/route_planning/custom_order.h"

namespace f2c::rp {

CustomOrder::CustomOrder() : SingleCellSwathsOrderBase(){
}

CustomOrder::CustomOrder(F2CSwaths& swaths) : SingleCellSwathsOrderBase(swaths) {
}

CustomOrder::CustomOrder(F2CSwaths& swaths, std::vector<size_t> order) : SingleCellSwathsOrderBase(swaths) {
  set_custom_order(order);
}

void CustomOrder::set_custom_order(std::vector<size_t> order){
  this->custom_order = order;
}

void CustomOrder::sortSwaths() {
  this->changeStartPoint();
  sort_swaths(swaths_, this->custom_order);
}


void CustomOrder::sort_swaths(F2CSwaths& swaths, std::vector<size_t>& order) {
  size_t i, j, k;
  F2CSwath sw;
  for(i = 0; i < swaths.size(); i++){
    if(i != order[i]){
        sw = swaths[i];
        k = i;
        while(i != (j = order[k])){
            swaths[k] = swaths[j];
            order[k] = k;
            k = j;
        }
        swaths[k] = sw;
        order[k] = k;
    }
  }
}

} // namespace f2c::rp