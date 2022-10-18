//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_STRIP_H_
#define FIELDS2COVER_TYPES_STRIP_H_

#include <string>
#include "fields2cover/types/Cell.h"

namespace f2c::types {

struct Strip {
  f2c::types::Cell cell;
  std::string name;

 public:
  Strip clone() const;
};

}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_STRIP_H_
