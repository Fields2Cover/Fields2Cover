//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/LinearRing.h"
#include "fields2cover/types/LineString.h"

namespace f2c::types {

LinearRing::LinearRing() {
  this->data_ = std::shared_ptr<OGRLinearRing>(
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
  return this->data_->getX(i);
}

double LinearRing::getY(size_t i) const {
  return this->data_->getY(i);
}
double LinearRing::getZ(size_t i) const {
  return this->data_->getZ(i);
}
double LinearRing::length() const {
  return this->data_->get_Length();
}

void LinearRing::reversePoints() {
  this->data_->reversePoints();
}

size_t LinearRing::size() const {
  return isEmpty() ? 0 : this->data_->getNumPoints();
}

void LinearRing::getGeometry(size_t i, Point& point) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearRing does not contain point " +
        std::to_string(i));
  }
  data_->getPoint(i, point.get());
}

void LinearRing::getGeometry(size_t i, Point& point) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearRing does not contain point " +
        std::to_string(i));
  }
  data_->getPoint(i, point.get());
}

Point LinearRing::getGeometry(size_t i) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearRing does not contain point " +
        std::to_string(i));
  }
  OGRPoint point;
  data_->getPoint(i, &point);
  return Point(point);
}

const Point LinearRing::getGeometry(size_t i) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearRing does not contain point " +
        std::to_string(i));
  }
  return Point(data_->getX(i), data_->getY(i), data_->getZ(i));
}

void LinearRing::setGeometry(size_t i, const Point& p) {
  data_->setPoint(i, p.getX(), p.getY(), p.getZ());
}

void LinearRing::addGeometry(const Point& p) {
  this->addPoint(p);
}

void LinearRing::addPoint(double x, double y, double z) {
  data_->addPoint(x, y, z);
}

void LinearRing::addPoint(const Point& p) {
  data_->addPoint(p.getX(), p.getY(), p.getZ());
}

const Point LinearRing::startPoint() const {
  return getGeometry(0);
}

const Point LinearRing::endPoint() const {
  return getGeometry(size()-1);
}

bool LinearRing::isClockwise() const {
  return data_->isClockwise();
}

Point LinearRing::closestPointTo(const Point& p) const {
  std::vector<double> dist;
  std::vector<Point> ps;
  for (size_t i = 0; i < this->size() - 1; ++i) {
    ps.emplace_back(p.closestPointInSegment(this->at(i), this->at(i+1)));
    dist.emplace_back(ps.back().distance(p));
  }
  return ps[std::min_element(dist.begin(), dist.end()) - dist.begin()];
}


}  // namespace f2c::types

