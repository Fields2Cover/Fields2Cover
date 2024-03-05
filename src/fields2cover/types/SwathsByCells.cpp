//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/SwathsByCells.h"
#include <algorithm>
#include <numeric>

namespace f2c::types {

SwathsByCells::SwathsByCells() = default;
SwathsByCells::SwathsByCells(int i) : data_(i) {}

SwathsByCells::SwathsByCells(const std::initializer_list<Swaths>& v_s) {
  for (const auto& s : v_s) {
    emplace_back(s);
  }
}

SwathsByCells::SwathsByCells(const std::vector<Swaths>& v_s) {
  for (const auto& s : v_s) {
    emplace_back(s);
  }
}

SwathsByCells::~SwathsByCells() = default;

void SwathsByCells::emplace_back(const Swaths& s) {
  this->data_.emplace_back(s);
}

void SwathsByCells::push_back(const Swaths& s) {
  this->data_.push_back(s);
}

std::vector<Swaths>::iterator SwathsByCells::begin() {
  return this->data_.begin();
}

std::vector<Swaths>::iterator SwathsByCells::end() {
  return this->data_.end();
}

std::vector<Swaths>::const_iterator SwathsByCells::begin() const {
  return this->data_.begin();
}

std::vector<Swaths>::const_iterator SwathsByCells::end() const {
  return this->data_.end();
}

void SwathsByCells::reverse() {
  std::reverse(this->data_.begin(), this->data_.end());
}

Swaths& SwathsByCells::back() {
  return this->data_.back();
}

const Swaths& SwathsByCells::back() const {
  return this->data_.back();
}

Swaths& SwathsByCells::at(size_t i) {
  return this->data_.at(i);
}

const Swaths& SwathsByCells::at(size_t i) const {
  return this->data_.at(i);
}

Swath& SwathsByCells::getSwath(size_t i) {
  size_t j {0};
  auto size_s = this->data_.at(j).size();
  while (i >= size_s) {
    i -= size_s;
    ++j;
    size_s = this->data_.at(j).size();
  }
  return this->data_.at(j).at(i);
}

const Swath& SwathsByCells::getSwath(size_t i) const {
  size_t j {0};
  auto size_s = this->data_.at(j).size();
  while (i >= size_s) {
    i -= size_s;
    ++j;
    size_s = this->data_.at(j).size();
  }
  return this->data_.at(j).at(i);
}

size_t SwathsByCells::size() const {
  return this->data_.size();
}

size_t SwathsByCells::sizeTotal() const {
  return std::accumulate(
      this->data_.begin(), this->data_.end(), 0,
      [](size_t a, const Swaths& b) {return a + b.size();});
}

Swaths& SwathsByCells::operator[] (int i) {
  return this->data_[i];
}

const Swaths& SwathsByCells::operator[] (int i) const {
  return this->data_[i];
}

SwathsByCells SwathsByCells::clone() const {
  SwathsByCells sbc;
  for (auto&& s : this->data_) {
    sbc.emplace_back(s.clone());
  }
  return sbc;
}

Swaths SwathsByCells::flatten() const {
  Swaths swaths;
  for (auto&& s : *this) {
    swaths.append(s);
  }
  return swaths;
}


}  // namespace f2c::types

