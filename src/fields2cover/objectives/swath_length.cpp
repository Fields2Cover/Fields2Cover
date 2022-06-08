//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/swath_length.h"

namespace f2c {
namespace obj {

double SwathLength::computeCost(const F2CSwath& s) const {
  return s.getLength();
}

double SwathLength::computeCost(const F2CSwaths& swaths) const {
  return std::accumulate(swaths.begin(), swaths.end(), 0.0,
    [] (int init, const F2CSwath& s) {return init + s.getLength();});
}

}  // namespace obj
}  // namespace f2c

