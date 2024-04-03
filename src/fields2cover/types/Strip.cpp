//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Strip.h"

namespace f2c::types {

Cell& Strip::getCell() {
  return this->cell_;
}

const Cell& Strip::getCell() const {
  return this->cell_;
}

void Strip::setCell(const Cell& c)  {
  this->cell_ = c;
}

std::string Strip::getName() const {
  return name_;
}

void Strip::setName(const std::string& str) {
  this->name_ = str;
}

Strip Strip::clone() const {
  Strip new_strip;
  new_strip.setName(this->name_);
  new_strip.setCell(this->cell_.clone());
  return new_strip;
}

}  // namespace f2c::types

