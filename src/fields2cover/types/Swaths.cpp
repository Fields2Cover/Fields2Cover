//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Swaths.h"
#include <algorithm>

namespace f2c {
namespace types {

Swaths::Swaths() = default;
Swaths::Swaths(int i) : data(i) {}

Swaths::Swaths(const std::initializer_list<Swath>& _s) {
  for (auto&& s : _s) {
    emplace_back(s);
  }
}

Swaths::~Swaths() = default;

void Swaths::emplace_back(const Swath& _s) {
  data.emplace_back(_s);
}

void Swaths::emplace_back(const LineString& _l, double _w, int _id) {
  data.emplace_back(_l, _w, _id);
}

void Swaths::push_back(const Swath& _s) {
  data.push_back(_s);
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

void Swaths::append(const LineString& _line, double _width) {
  this->emplace_back(_line, _width, this->size());
}

void Swaths::append(const MultiLineString& _lines, double _width) {
  for (auto&& line : _lines) {
    append(line.clone(), _width);
  }
}

void Swaths::append(const LineString& _line, const Cell& _poly, double _width) {
  append(_poly.getLinesInside(_line), _width);
}

void Swaths::append(const LineString& _line, const Cells& _polys,
    double _width) {
  append(_polys.getLinesInside(_line), _width);
}

void Swaths::append(const MultiLineString& _lines, const Cell& _poly,
    double _width) {
  append(_poly.getLinesInside(_lines), _width);
}

void Swaths::append(const MultiLineString& _lines, const Cells& _polys,
    double _width) {
  append(_polys.getLinesInside(_lines), _width);
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



}  // namespace types
}  // namespace f2c
