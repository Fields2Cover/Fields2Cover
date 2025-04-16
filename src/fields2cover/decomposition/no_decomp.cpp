//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/decomposition/no_decomp.h"

namespace f2c::decomp {

F2CMultiLineString NoDecomp::genSplitLines(
    const F2CCells& cells, const obj::DecompObjective& obj) const {
  return {};
}

}  // namespace f2c::decomp

