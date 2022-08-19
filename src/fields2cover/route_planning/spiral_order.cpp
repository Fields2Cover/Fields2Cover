#include "fields2cover/route_planning/spiral_order.h"

namespace f2c::rp {

SpiralOrder::SpiralOrder() : SingleCellSwathsOrderBase(), spiral_size(2) {}

SpiralOrder::SpiralOrder(F2CSwaths& swaths) :
  SingleCellSwathsOrderBase(swaths), spiral_size(2) {}

SpiralOrder::SpiralOrder(F2CSwaths& swaths, int sp_size) :
    SingleCellSwathsOrderBase(swaths) {
  set_spiral_size(std::max(2, sp_size));
}

SpiralOrder::~SpiralOrder() = default;

void SpiralOrder::set_spiral_size(int sp_size){
  this->spiral_size = sp_size;
}

void SpiralOrder::sortSwaths() {
  this->changeStartPoint();

  int spiral_count = swaths_.size() / spiral_size;
  for (size_t i = 0; i < spiral_count; i++) {
    spiral(i * spiral_size, spiral_size);
  }

  int swaths_left = swaths_.size() - spiral_count * spiral_size;
  if (swaths_left > 1) {
    spiral(spiral_count * spiral_size, swaths_left);
  }
}

void SpiralOrder::spiral(size_t offset, int size) {
  for (size_t j = offset % 2 ? 0 : 1; j <= (size + 1) / 2; j += 2) {
    std::rotate(swaths_.begin() + offset + j, 
                swaths_.begin() + offset + size - 1, 
                swaths_.begin() + offset + size);
  }
}

}  // namespace f2c::rp

