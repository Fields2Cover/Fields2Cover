//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/MultiLineString.h"

namespace f2c::types {

MultiLineString::MultiLineString() {
  data_ = std::shared_ptr<OGRMultiLineString>(static_cast<OGRMultiLineString*>(
      OGRGeometryFactory::createGeometry(wkbMultiLineString)),
    [](OGRMultiLineString* f) {OGRGeometryFactory::destroyGeometry(f);});
}

MultiLineString::MultiLineString(const OGRGeometry* geom) {
  this->append(geom);
}

MultiLineString::MultiLineString(const LineString& line) {
  this->addGeometry(line);
}

MultiLineString::MultiLineString(const std::initializer_list<LineString>& ls) {
  for (auto&& line : ls) {
    this->addGeometry(line);
  }
}

size_t MultiLineString::size() const {
  return isEmpty() ? 0 : data_->getNumGeometries();
}

double MultiLineString::length() const {
  return this->data_->get_Length();
}

void MultiLineString::operator*=(double b) {
  for (auto&& r : *this) {
    r *= b;
  }
}

void MultiLineString::append(const OGRGeometry* geom) {
  if (wkbFlatten(geom->getGeometryType()) ==
      OGRwkbGeometryType::wkbLineString) {
    if (!geom->IsEmpty()) {
      data_->addGeometry(geom);
    }
  } else if (wkbFlatten(geom->getGeometryType()) ==
        OGRwkbGeometryType::wkbMultiLineString) {
    for (auto&& line : *geom->toMultiLineString()) {
      data_->addGeometry(line);
    }
  } else if (wkbFlatten(geom->getGeometryType()) ==
      OGRwkbGeometryType::wkbPolygon) {
    for (auto&& ring : *geom->toPolygon()) {
      data_->addGeometry(ring);
    }
  } else if (wkbFlatten(geom->getGeometryType()) ==
      OGRwkbGeometryType::wkbMultiPolygon) {
    for (auto&& poly : *geom->toMultiPolygon()) {
      for (auto&& ring : poly) {
        data_->addGeometry(ring);
      }
    }
  }
}

void MultiLineString::getGeometry(size_t i, LineString& line) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }
  line = LineString(this->data_->getGeometryRef(i), EmptyDestructor());
}

void MultiLineString::getGeometry(size_t i, LineString& line) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }
  line = LineString(this->data_->getGeometryRef(i), EmptyDestructor());
}

LineString MultiLineString::getGeometry(size_t i) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }
  return LineString(this->data_->getGeometryRef(i));
}

const LineString MultiLineString::getGeometry(size_t i) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }
  return LineString(this->data_->getGeometryRef(i));
}

void MultiLineString::setGeometry(size_t i, const LineString& line) {
  auto n = this->size();
  if (i < n) {
    MultiLineString lines;
    for (size_t j = 0; j < n; ++j) {
      lines.addGeometry((i == j) ? line : this->getGeometry(j));
    }
    *this = lines;
    return;
  } else if (i != n) {
    for (size_t j = n; j < i; ++j) {
      this->addGeometry(LineString());
    }
  }
  this->addGeometry(line);
}

void MultiLineString::addGeometry(const LineString& line) {
  this->data_->addGeometry(line.get());
}

void MultiLineString::addGeometry(const MultiLineString& lines) {
  for (auto&& line : lines) {
    addGeometry(line);
  }
}

MultiLineString MultiLineString::getLineSegments(const LineString& line) {
  MultiLineString res;
  for (size_t i = 0; i < line.size()-1; ++i) {
    res.addGeometry(LineString({line.getGeometry(i), line.getGeometry(i+1)}));
  }
  return res;
}

MultiLineString MultiLineString::getLineSegments(const LinearRing& line) {
  return getLineSegments(LineString(line));
}

}  // namespace f2c::types

