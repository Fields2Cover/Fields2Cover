//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Point.h"

namespace f2c {
namespace types {

Point::Point() {
  data = std::shared_ptr<OGRPoint>(
    static_cast<OGRPoint*>(OGRGeometryFactory::createGeometry(wkbPoint)),
    [](OGRPoint* f) {OGRGeometryFactory::destroyGeometry(f);});
}


Point::Point(double x, double y, double z) {
  data->setX(x);
  data->setY(y);
  data->setZ(z);
}


std::ostream& operator<<(std::ostream& os, const Point& p) {
  os << "Point(" << p.getX() << ", " << p.getY() << ", " << p.getZ() << ")";
  return os;
}

bool Point::operator==(const Point& b) const {
  return *data == *b.get();
}

bool Point::operator<(const Point& b) const {
  return ((this->getX() < b.getX()) ||
      ((this->getX() == b.getX()) && (this->getY() < b.getY())));
}

Point Point::operator+(const Point& b) const {
  return Point(
      data->getX() + b->getX(),
      data->getY() + b->getY(),
      data->getZ() + b->getZ());
}

Point Point::operator-(const Point& b) const {
  return Point(
      data->getX() - b->getX(),
      data->getY() - b->getY(),
      data->getZ() - b->getZ());
}

void Point::operator*=(double b) {
  data->setX(data->getX() * b);
  data->setY(data->getY() * b);
  data->setZ(data->getZ() * b);
}

Point Point::operator*(double b) const {
  return Point(data->getX() * b, data->getY() * b, data->getZ() * b);
}

Point Point::clone() const {return Point(data->clone());}
double Point::getX() const {return data->getX();}
double Point::getY() const {return data->getY();}
double Point::getZ() const {return data->getZ();}
void Point::setX(double x) {data->setX(x);}
void Point::setY(double y) {data->setY(y);}
void Point::setZ(double z) {data->setZ(z);}

void Point::setPoint(double x, double y, double z) {
  data->setX(x);
  data->setY(y);
  data->setZ(z);
}

void Point::setPoint(const OGRPoint& p) {
  data->setX(p.getX());
  data->setY(p.getY());
  data->setZ(p.getZ());
}

void Point::setPoint(const Point& p) {
  data->setX(p.getX());
  data->setY(p.getY());
  data->setZ(p.getZ());
}

Point Point::rotateFromPoint(double _angle, const Point& _p_r) const {
  double s = sin(_angle);
  double c = cos(_angle);
  auto p = _p_r - *this;
  return Point(p.getX() * c - p.getY() * s + data->getX(),
      p.getX() * s + p.getY() * c + data->getY());
}

double Point::getAngleFromPoints(const Point& _end) const {
  auto dot = data->getX() * _end.getX() + data->getY() * _end.getY();
  auto det = data->getX() * _end.getY() - data->getY() * _end.getX();
  return mod_2pi(atan2(det, dot));
}

double Point::getAngleFromPoint() const {
  return f2c::types::Point(1.0, 0.0).getAngleFromPoints(*this);
}

Point Point::getPointFromAngle(double _angle, double _dist) const {
  return Point(data->getX() + _dist * cos(_angle),
       data->getY() + _dist * sin(_angle));
}



}  // namespace types
}  // namespace f2c
