//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_STRIP_H_
#define FIELDS2COVER_TYPES_STRIP_H_

#include <string>
#include "fields2cover/types/Cell.h"

namespace f2c::types {

struct Strip {
 public:
  Cell& getCell();
  const Cell& getCell() const;
  void setCell(const Cell& cell);

  std::string getName() const;
  void setName(const std::string& str);

  Strip clone() const;

 private:
  Cell cell_;
  std::string name_;
};

}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_STRIP_H_
