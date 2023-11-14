//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/MultiPoint.h"

namespace f2c::types {

MultiPoint::MultiPoint() {
  data = std::shared_ptr<OGRMultiPoint>(
    static_cast<OGRMultiPoint*>(
      OGRGeometryFactory::createGeometry(wkbMultiPoint)),
    [](OGRMultiPoint* f) {OGRGeometryFactory::destroyGeometry(f);});
}

MultiPoint::MultiPoint(const std::vector<Point>& ps) {
  for (auto&& p : ps) {
    this->addGeometry(p);
  }
}

MultiPoint::MultiPoint(const std::initializer_list<Point>& ps) {
  for (auto&& p : ps) {
    this->addGeometry(p);
  }
}

size_t MultiPoint::size() const {
  return isEmpty() ? 0 : this->data->getNumGeometries();
}

void MultiPoint::getGeometry(size_t i, Point& point) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "MultiPoint does not contain point " + std::to_string(i));
  }
  point = Point(data->getGeometryRef(i), EmptyDestructor());
}

void MultiPoint::getGeometry(size_t i, Point& point) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "MultiPoint does not contain point " + std::to_string(i));
  }
  point = Point(data->getGeometryRef(i), EmptyDestructor());
}

Point MultiPoint::getGeometry(size_t i) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "MultiPoint does not contain point " + std::to_string(i));
  }
  return Point(data->getGeometryRef(i));
}

const Point MultiPoint::getGeometry(size_t i) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "MultiPoint does not contain point " + std::to_string(i));
  }
  return Point(data->getGeometryRef(i));
}

const Point MultiPoint::getFirstPoint() const {
  return getGeometry(0);
}

const Point MultiPoint::getLastPoint() const {
  return getGeometry(size() - 1);
}


void MultiPoint::setGeometry(size_t i, const Point& p) {
    OGRPoint* point = downCast<OGRPoint*>(data->getGeometryRef(i));
    point->setX(p.getX());
    point->setY(p.getY());
    point->setZ(p.getZ());
}


void MultiPoint::addGeometry(const Point& p) {
  this->addPoint(p);
}

void MultiPoint::addPoint(const Point& p) {
  this->addPoint(p.getX(), p.getY(), p.getZ());
}

void MultiPoint::addPoints(const MultiPoint& ps) {
  for (auto&& p : ps) {
    this->addPoint(p);
  }
}

void MultiPoint::addPoint(double x, double y, double z) {
  OGRPoint p(x, y, z);
  this->data->addGeometry(&p);
}


void MultiPoint::operator*=(double b) {
  for (auto&& p : *this) {
    p *= b;
  }
}

}  // namespace f2c::types

