//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_SWATHS_H_
#define FIELDS2COVER_TYPES_SWATHS_H_

#include <vector>
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/MultiLineString.h"
#include "fields2cover/types/Cell.h"
#include "fields2cover/types/Cells.h"
#include "fields2cover/types/Swath.h"

namespace f2c::types {

struct Swaths {
  Swaths();
  explicit Swaths(int i);
  Swaths(const std::initializer_list<Swath>& s);
  Swaths(std::vector<Swath>& s);
  ~Swaths();

  void emplace_back(const Swath& s);

  void emplace_back(const LineString& l, double w, int id = 0);

  void push_back(const Swath& s);

  std::vector<Swath>::iterator begin();
  std::vector<Swath>::iterator end();
  std::vector<Swath>::const_iterator begin() const;
  std::vector<Swath>::const_iterator end() const;
  void reverse();

  Swath& back();
  const Swath& back() const;

  Swath& at(size_t i);

  size_t size() const;

  Swath& operator[] (int i);

  const Swath& operator[] (int i) const;

  void append(const LineString& line, double width = 0);
  void append(const MultiLineString& line, double width = 0);
  void append(const LineString& line, const Cell& poly, double width = 0);
  void append(const MultiLineString& lines, const Cell& poly,
      double width = 0);
  void append(const LineString& line, const Cells& polys,
      double width = 0);
  void append(const MultiLineString& lines, const Cells& poly,
      double width = 0);
  void sort();
  void reverseDirOddSwaths();

  Swaths clone() const;

 private:
  std::vector<Swath> data;
};


}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_SWATHS_H_
