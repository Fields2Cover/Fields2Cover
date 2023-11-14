//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/LinearRing.h"
#include "fields2cover/types/LineString.h"

namespace f2c::types {

LinearRing::LinearRing() {
  data = std::shared_ptr<OGRLinearRing>(
    static_cast<OGRLinearRing*>(
      OGRGeometryFactory::createGeometry(wkbLinearRing)),
    [](OGRLinearRing* f) {OGRGeometryFactory::destroyGeometry(f);});
}

LinearRing::LinearRing(const std::vector<Point>& ps) {
  for (auto&& p : ps) {
    this->addPoint(p);
  }
}

LinearRing::LinearRing(const std::initializer_list<Point>& ps) {
  for (auto&& p : ps) {
    this->addPoint(p);
  }
}

void LinearRing::operator*=(double b) {
  for (auto&& p : *this) {
    p *= b;
  }
}

double LinearRing::getX(size_t i) const {
  return data->getX(i);
}

double LinearRing::getY(size_t i) const {
  return data->getY(i);
}
double LinearRing::getZ(size_t i) const {
  return data->getZ(i);
}
double LinearRing::getLength() const {
  return this->data->get_Length();
}

void LinearRing::reversePoints() {
  this->data->reversePoints();
}

size_t LinearRing::size() const {
  return isEmpty() ? 0 : this->data->getNumPoints();
}

void LinearRing::getGeometry(size_t i, Point& point) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearRing does not contain point " +
        std::to_string(i));
  }
  data->getPoint(i, point.get());
}

void LinearRing::getGeometry(size_t i, Point& point) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearRing does not contain point " +
        std::to_string(i));
  }
  data->getPoint(i, point.get());
}

Point LinearRing::getGeometry(size_t i) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearRing does not contain point " +
        std::to_string(i));
  }
  OGRPoint point;
  data->getPoint(i, &point);
  return Point(point);
}

const Point LinearRing::getGeometry(size_t i) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearRing does not contain point " +
        std::to_string(i));
  }
  return Point(data->getX(i), data->getY(i), data->getZ(i));
}

void LinearRing::setGeometry(size_t i, const Point& p) {
  data->setPoint(i, p.getX(), p.getY(), p.getZ());
}

void LinearRing::addGeometry(const Point& p) {
  this->addPoint(p);
}

void LinearRing::addPoint(double x, double y, double z) {
  data->addPoint(x, y, z);
}

void LinearRing::addPoint(const Point& p) {
  data->addPoint(p.getX(), p.getY(), p.getZ());
}

const Point LinearRing::StartPoint() const {
  return getGeometry(0);
}

const Point LinearRing::EndPoint() const {
  return getGeometry(size()-1);
}

bool LinearRing::isClockwise() const {
  return data->isClockwise();
}



}  // namespace f2c::types

