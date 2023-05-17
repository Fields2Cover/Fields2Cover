//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/LinearRing.h"

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

void LinearRing::reversePoints() const {
  this->data->reversePoints();
}

size_t LinearRing::size() const {
  return isEmpty() ? 0 : this->data->getNumPoints();
}

void LinearRing::getGeometry(int i, Point& point) {
  if (i < 0 || i >= this->size()) {
    throw std::out_of_range("Geometry does not contain point " + std::to_string(i));
  }
  data->getPoint(i, point.get());
}

void LinearRing::getGeometry(int i, Point& point) const {
  if (i < 0 || i >= this->size()) {
    throw std::out_of_range("Geometry does not contain point " + std::to_string(i));
  }
  data->getPoint(i, point.get());
}

Point LinearRing::getGeometry(int i) {
  if (i < 0 || i >= this->size()) {
    throw std::out_of_range("Geometry does not contain point " + std::to_string(i));
  }

  OGRPoint point;
  data->getPoint(i, &point);
  return Point(point);
}

const Point LinearRing::getGeometry(int i) const {
  if (i < 0 || i >= this->size()) {
    throw std::out_of_range("Geometry does not contain point " + std::to_string(i));
  }

  OGRPoint point;
  data->getPoint(i, &point);
  return Point(point);
}

void LinearRing::setGeometry(int i, const Point& p) {
  data->setPoint(i, p.get());
}



void LinearRing::addPoint(double x, double y, double z) {
  data->addPoint(x, y, z);
}

void LinearRing::addPoint(const Point& p) {
  data->addPoint(p.get());
}

Point LinearRing::StartPoint() const {
  OGRPoint p;
  data->StartPoint(&p);
  return Point(p);
}

Point LinearRing::EndPoint() const {
  OGRPoint p;
  data->EndPoint(&p);
  return Point(p);
}


}  // namespace f2c::types

