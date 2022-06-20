//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/MultiPoint.h"

namespace f2c {
namespace types {

MultiPoint::MultiPoint() {
  data = std::shared_ptr<OGRMultiPoint>(
    static_cast<OGRMultiPoint*>(
      OGRGeometryFactory::createGeometry(wkbMultiPoint)),
    [](OGRMultiPoint* f) {OGRGeometryFactory::destroyGeometry(f);});
}

MultiPoint::MultiPoint(const std::initializer_list<Point>& ps) {
  for (auto&& p : ps) {
    this->addGeometry(p);
  }
}

size_t MultiPoint::size() const {
  return isEmpty() ? 0 : this->data->getNumGeometries();
}

void MultiPoint::getGeometry(int i, Point& point) {
  point = Point(data->getGeometryRef(i), EmptyDestructor());
}

void MultiPoint::getGeometry(int i, Point& point) const {
  point = Point(data->getGeometryRef(i), EmptyDestructor());
}

Point MultiPoint::getGeometry(int i) {
  return Point(data->getGeometryRef(i));
}

const Point MultiPoint::getGeometry(int i) const {
  return Point(data->getGeometryRef(i));
}

void MultiPoint::setGeometry(int i, const Point& p) {
    OGRPoint* point = data->getGeometryRef(i);
    point->setX(p.getX());
    point->setY(p.getY());
    point->setZ(p.getZ());
}


void MultiPoint::addGeometry(const Point& p) {
  this->addPoint(p);
}

void MultiPoint::addPoint(const Point& p) {
  this->data->addGeometry(p.get());
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

}  // namespace types
}  // namespace f2c
