//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/MultiLineString.h"

namespace f2c::types {

MultiLineString::MultiLineString() {
  data = std::shared_ptr<OGRMultiLineString>(static_cast<OGRMultiLineString*>(
      OGRGeometryFactory::createGeometry(wkbMultiLineString)),
    [](OGRMultiLineString* f) {OGRGeometryFactory::destroyGeometry(f);});
}

MultiLineString::MultiLineString(const OGRGeometry* geom) {
  append(geom);
}

MultiLineString::MultiLineString(const LineString& line) {
  data->addGeometry(line.get());
}

size_t MultiLineString::size() const {
  return isEmpty() ? 0 : data->getNumGeometries();
}

double MultiLineString::getLength() const {
  return this->data->get_Length();
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
      data->addGeometry(geom);
    }
  } else if (wkbFlatten(geom->getGeometryType()) ==
        OGRwkbGeometryType::wkbMultiLineString) {
    for (auto&& line : *geom->toMultiLineString()) {
      data->addGeometry(line);
    }
  } else if (wkbFlatten(geom->getGeometryType()) ==
      OGRwkbGeometryType::wkbPolygon) {
    for (auto&& ring : *geom->toPolygon()) {
      data->addGeometry(ring);
    }
  } else if (wkbFlatten(geom->getGeometryType()) ==
      OGRwkbGeometryType::wkbMultiPolygon) {
    for (auto&& poly : *geom->toMultiPolygon()) {
      for (auto&& ring : poly) {
        data->addGeometry(ring);
      }
    }
  }
}

void MultiLineString::getGeometry(int i, LineString& line) {
  line = LineString(this->data->getGeometryRef(i), EmptyDestructor());
}

void MultiLineString::getGeometry(int i, LineString& line) const {
  line = LineString(this->data->getGeometryRef(i),
      EmptyDestructor());
}

LineString MultiLineString::getGeometry(int i) {
  return LineString(this->data->getGeometryRef(i));
}

const LineString MultiLineString::getGeometry(int i) const {
  return LineString(this->data->getGeometryRef(i));
}

void MultiLineString::setGeometry(int i, const LineString& line) {
  auto n = this->size();
  if (i < n) {
    OGRLineString* ref = this->data->getGeometryRef(i);
    *ref = *line.get();
    return;
  } else if (i > n) {
    for (int j = i; j < n; ++j) {
      this->addGeometry(LineString());
    }
  }
  this->addGeometry(line);
}

void MultiLineString::addGeometry(const LineString& line) {
  this->data->addGeometry(line.get());
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

