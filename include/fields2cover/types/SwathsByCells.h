//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_SWATHS_BY_CELLS_H_
#define FIELDS2COVER_TYPES_SWATHS_BY_CELLS_H_

#include <vector>
#include "fields2cover/types/Swath.h"
#include "fields2cover/types/Swaths.h"

namespace f2c::types {

struct SwathsByCells {
  SwathsByCells();
  explicit SwathsByCells(int i);
  explicit SwathsByCells(const std::initializer_list<Swaths>& s);
  explicit SwathsByCells(const std::vector<Swaths>& s);
  ~SwathsByCells();

  void emplace_back(const Swaths& s);
  void push_back(const Swaths& s);

  std::vector<Swaths>::iterator begin();
  std::vector<Swaths>::iterator end();
  std::vector<Swaths>::const_iterator begin() const;
  std::vector<Swaths>::const_iterator end() const;
  void reverse();

  Swaths& back();
  const Swaths& back() const;

  Swaths& at(size_t i);
  const Swaths& at(size_t i) const;

  // Return the swath i^th
  Swath& getSwath(size_t i);
  const Swath& getSwath(size_t i) const;

  size_t size() const;

  // Sum of the size of each Swaths.
  size_t sizeTotal() const;

  Swaths& operator[] (int i);
  const Swaths& operator[] (int i) const;

  SwathsByCells clone() const;
  Swaths flatten() const;

 private:
  std::vector<Swaths> data_;
};


}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_SWATHS_BY_CELLS_H_
