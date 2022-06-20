//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Swaths.h"
#include <algorithm>

namespace f2c::types {

Swaths::Swaths() = default;
Swaths::Swaths(int i) : data(i) {}

Swaths::Swaths(const std::initializer_list<Swath>& v_s) {
  for (auto&& s : v_s) {
    emplace_back(s);
  }
}

Swaths::~Swaths() = default;

void Swaths::emplace_back(const Swath& s) {
  data.emplace_back(s);
}

void Swaths::emplace_back(const LineString& l, double w, int id) {
  data.emplace_back(l, w, id);
}

void Swaths::push_back(const Swath& s) {
  data.push_back(s);
}

std::vector<Swath>::iterator Swaths::begin() {
  return data.begin();
}

std::vector<Swath>::iterator Swaths::end() {
  return data.end();
}

std::vector<Swath>::const_iterator Swaths::begin() const {
  return data.begin();
}

std::vector<Swath>::const_iterator Swaths::end() const {
  return data.end();
}

void Swaths::reverse() {
  std::reverse(data.begin(), data.end());
}

Swath& Swaths::back() {
  return data.back();
}

const Swath& Swaths::back() const {
  return data.back();
}

Swath& Swaths::at(size_t i) {
  return data.at(i);
}

size_t Swaths::size() const {
  return data.size();
}

Swath& Swaths::operator[] (int i) {
  return data[i];
}

const Swath& Swaths::operator[] (int i) const {
  return data[i];
}

void Swaths::append(const LineString& line, double width) {
  this->emplace_back(line, width, this->size());
}

void Swaths::append(const MultiLineString& lines, double width) {
  for (auto&& line : lines) {
    append(line.clone(), width);
  }
}

void Swaths::append(const LineString& line, const Cell& poly, double width) {
  append(poly.getLinesInside(line), width);
}

void Swaths::append(const LineString& line, const Cells& polys,
    double width) {
  append(polys.getLinesInside(line), width);
}

void Swaths::append(const MultiLineString& lines, const Cell& poly,
    double width) {
  append(poly.getLinesInside(lines), width);
}

void Swaths::append(const MultiLineString& lines, const Cells& polys,
    double width) {
  append(polys.getLinesInside(lines), width);
}

void Swaths::sort() {
  for (auto&& s : data) {
    if (s.endPoint() < s.startPoint()) {
      s.reverse();
    }
  }
  std::sort(data.begin(), data.end());
}

void Swaths::reverseDirOddSwaths() {
  for (size_t i = 1; i < data.size(); ++i) {
    data[i].targetOppositeDirAs(data[i - 1]);
  }
}

Swaths Swaths::clone() const {
  Swaths new_s;
  for (auto&& s : data) {
    new_s.emplace_back(s.clone());
  }
  return new_s;
}



}  // namespace f2c::types

