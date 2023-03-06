#include "fields2cover/route_planning/spiral_order.h"

namespace f2c::rp {


SpiralOrder::SpiralOrder(size_t sp_size) {
  setSpiralSize(static_cast<size_t>(std::max(2, static_cast<int>(sp_size))));
}

SpiralOrder::~SpiralOrder() = default;

void SpiralOrder::setSpiralSize(size_t sp_size) {
  this->spiral_size = sp_size;
}

void SpiralOrder::sortSwaths(F2CSwaths& swaths) const {
  size_t spiral_count = swaths.size() / spiral_size;
  for (size_t i = 0; i < spiral_count; i++) {
    spiral(swaths, i * spiral_size, spiral_size);
  }

  int swaths_left = swaths.size() - spiral_count * spiral_size;
  if (swaths_left > 1) {
    spiral(swaths, spiral_count * spiral_size, swaths_left);
  }
}

void SpiralOrder::spiral(F2CSwaths& swaths, size_t offset, size_t size) const {
  for (size_t j = (offset + 1) % 2; j < size; j += 2) {
    std::rotate(swaths.begin() + offset + j,
                swaths.begin() + offset + size - 1,
                swaths.begin() + offset + size);
  }
}

}  // namespace f2c::rp

