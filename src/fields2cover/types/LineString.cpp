//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/LineString.h"

namespace f2c::types {

LineString::LineString() {
  data_ = std::shared_ptr<OGRLineString>(
    static_cast<OGRLineString*>(
      OGRGeometryFactory::createGeometry(wkbLineString)),
    [](OGRLineString* f) {OGRGeometryFactory::destroyGeometry(f);});
}

LineString::LineString(const LinearRing& ring) {
  for (auto&& p : ring) {
    this->addPoint(p);
  }
}

LineString::LineString(const std::vector<Point>& ps) {
  for (auto&& p : ps) {
    this->addPoint(p);
  }
}

LineString::LineString(const Point& p1, const Point& p2) {
  this->addPoint(p1);
  this->addPoint(p2);
}

LineString::LineString(const std::initializer_list<Point>& ps) {
  for (auto&& p : ps) {
    this->addPoint(p);
  }
}

double LineString::X(size_t i) const {return data_->getX(i);}
double LineString::Y(size_t i) const {return data_->getY(i);}
double LineString::Z(size_t i) const {return data_->getZ(i);}
double LineString::getX(size_t i) const {return data_->getX(i);}
double LineString::getY(size_t i) const {return data_->getY(i);}
double LineString::getZ(size_t i) const {return data_->getZ(i);}
double LineString::length() const {return this->data_->get_Length();}
void LineString::reversePoints() { this->data_->reversePoints();}
size_t LineString::size() const {
  return isEmpty() ? 0 : this->data_->getNumPoints();
}


void LineString::getGeometry(size_t i, Point& point) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearString does not contain point " +
        std::to_string(i));
  }
  data_->getPoint(i, point.get());
}

void LineString::getGeometry(size_t i, Point& point) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearString does not contain point " +
        std::to_string(i));
  }
  data_->getPoint(i, point.get());
}

Point LineString::getGeometry(size_t i) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearString does not contain point " +
        std::to_string(i));
  }
  OGRPoint point;
  data_->getPoint(i, &point);
  return Point(point);
}

const Point LineString::getGeometry(size_t i) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Error getGeometry: LinearString does not contain point " +
        std::to_string(i));
  }
  return Point(data_->getX(i), data_->getY(i), data_->getZ(i));
}

void LineString::setGeometry(size_t i, const Point& p) {
  data_->setPoint(i, p.getX(), p.getY(), p.getZ());
}

void LineString::addGeometry(const Point& p) {
  this->addPoint(p);
}


void LineString::operator*=(double b) {
  for (auto&& p : *this) {
    p *= b;
  }
}

void LineString::addPoint(double x, double y, double z) {
  data_->addPoint(x, y, z);
}
void LineString::addPoint(const Point& p) {
  data_->addPoint(p.getX(), p.getY(), p.getZ());
}

const Point LineString::startPoint() const {
  return getGeometry(0);
}

const Point LineString::endPoint() const {
  return getGeometry(size()-1);
}

double LineString::startAngle() const {
  return (getGeometry(1) - startPoint()).getAngleFromPoint();
}

double LineString::endAngle() const {
  return (endPoint() - getGeometry(size() - 2)).getAngleFromPoint();
}

Point LineString::closestPointTo(const Point& p) const {
  std::vector<double> dist;
  std::vector<Point> ps;
  for (size_t i = 0; i < this->size() - 1; ++i) {
    ps.emplace_back(p.closestPointInSegment(this->at(i), this->at(i+1)));
    dist.emplace_back(ps.back().distance(p));
  }
  return ps[std::min_element(dist.begin(), dist.end()) - dist.begin()];
}


}  // namespace f2c::types

