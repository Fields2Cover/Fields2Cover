#include "fields2cover/route_planning/rev_spiral_order.h"

namespace f2c::rp {

void RevSpiralOrder::pattern(
    F2CSwaths& swaths, size_t offset, size_t size) const {
  for (size_t j = (offset + 1) % 2; j < size; j += 2) {
    std::rotate(swaths.begin() + offset + j,
                swaths.begin() + offset + size - 1,
                swaths.begin() + offset + size);
  }
}

}  // namespace f2c::rp

