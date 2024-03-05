//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Swaths.h"
#include <algorithm>

namespace f2c::types {

Swaths::Swaths() = default;
Swaths::Swaths(int i) : data_(i) {}

Swaths::Swaths(const std::initializer_list<Swath>& v_s) {
  for (auto&& s : v_s) {
    emplace_back(s);
  }
}

Swaths::Swaths(const std::vector<Swath>& v_s) {
  for (auto&& s : v_s) {
    emplace_back(s);
  }
}

Swaths::~Swaths() = default;

void Swaths::emplace_back(const Swath& s) {
  this->data_.emplace_back(s);
}

void Swaths::emplace_back(
    const LineString& l, double w, int id, SwathType type) {
  this->data_.emplace_back(l, w, id, type);
}

void Swaths::push_back(const Swath& s) {
  this->data_.push_back(s);
}

std::vector<Swath>::iterator Swaths::begin() {
  return this->data_.begin();
}

std::vector<Swath>::iterator Swaths::end() {
  return this->data_.end();
}

std::vector<Swath>::const_iterator Swaths::begin() const {
  return this->data_.begin();
}

std::vector<Swath>::const_iterator Swaths::end() const {
  return this->data_.end();
}

void Swaths::reverse() {
  std::reverse(this->data_.begin(), this->data_.end());
}

Swath& Swaths::back() {
  return this->data_.back();
}

const Swath& Swaths::back() const {
  return this->data_.back();
}

Swath& Swaths::at(size_t i) {
  return this->data_.at(i);
}

const Swath& Swaths::at(size_t i) const {
  return this->data_.at(i);
}

size_t Swaths::size() const {
  return this->data_.size();
}

Swath& Swaths::operator[] (int i) {
  return this->data_[i];
}

const Swath& Swaths::operator[] (int i) const {
  return this->data_[i];
}

void Swaths::append(const LineString& line, double width, SwathType type) {
  this->emplace_back(line, width, this->size(), type);
}

void Swaths::append(
    const MultiLineString& lines, double width, SwathType type) {
  for (auto&& line : lines) {
    this->append(line.clone(), width, type);
  }
}

void Swaths::append(const LineString& line, const Cell& poly, double width,
    SwathType type) {
  this->append(poly.getLinesInside(line), width, type);
}

void Swaths::append(const LineString& line, const Cells& polys,
    double width, SwathType type) {
  this->append(polys.getLinesInside(line), width, type);
}

void Swaths::append(const MultiLineString& lines, const Cell& poly,
    double width, SwathType type) {
  this->append(poly.getLinesInside(lines), width, type);
}

void Swaths::append(const MultiLineString& lines, const Cells& polys,
    double width, SwathType type) {
  this->append(polys.getLinesInside(lines), width, type);
}

void Swaths::append(const Swaths& swaths) {
  for (auto&& s : swaths) {
    this->emplace_back(s);
  }
}

void Swaths::sort() {
  for (auto&& s : this->data_) {
    if (s.endPoint() < s.startPoint()) {
      s.reverse();
    }
  }
  std::sort(this->data_.begin(), this->data_.end());
}

void Swaths::reverseDirOddSwaths() {
  for (size_t i = 1; i < this->size(); ++i) {
    this->data_[i].targetOppositeDirAs(this->data_[i - 1]);
  }
}

Swaths Swaths::clone() const {
  Swaths new_s;
  for (auto&& s : this->data_) {
    new_s.emplace_back(s.clone());
  }
  return new_s;
}

void Swaths::moveTo(const Point& ref_pt) {
  for (auto&& s : this->data_) {
    s.moveTo(ref_pt);
  }
}


}  // namespace f2c::types

