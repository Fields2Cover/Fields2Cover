//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/sg_obj/swath_length.h"

namespace f2c::obj {

double SwathLength::computeCost(const F2CSwath& s) {
  return s.length();
}

}  // namespace f2c::obj

