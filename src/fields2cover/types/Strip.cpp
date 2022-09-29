//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Strip.h"

namespace f2c::types {

Strip Strip::clone() const {
  Strip new_strip;
  new_strip.name = this->name;
  new_strip.cell = this->cell.clone();
  return new_strip;
}

}  // namespace f2c::types

