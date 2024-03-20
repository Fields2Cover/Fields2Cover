//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Point.h"

namespace f2c::types {

Point::Point() : Geometry() {}

Point::Point(double _x, double _y, double _z) : Geometry() {
  this->data_->setX(_x);
  this->data_->setY(_y);
  this->data_->setZ(_z);
}

Point::Point(const Point& p) : Geometry(p.clone()) {}
Point::Point(Point&& p) = default;
Point::~Point() = default;

Point& Point::operator=(Point&& p) {
  this->setX(p.X());
  this->setY(p.Y());
  this->setZ(p.Z());
  return *this;
}

Point& Point::operator=(const Point& p) {
  this->setX(p.X());
  this->setY(p.Y());
  this->setZ(p.Z());
  return *this;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
  os << "Point(" << p.X() << ", " << p.Y() << ", " << p.Z() << ")";
  return os;
}

bool Point::operator==(const Point& b) const {
  return (fabs(this->X() - b.X()) < 1e-7) && \
         (fabs(this->Y() - b.Y()) < 1e-7) && \
         (fabs(this->Z() - b.Z()) < 1e-7);
}

bool Point::operator!=(const Point& b) const {
  return !(this->operator==(b));
}

bool Point::operator<(const Point& b) const {
  return ((this->X() < b.X()) ||
      ((this->X() == b.X()) && (this->Y() < b.Y())));
}

Point Point::operator+(const Point& b) const {
  return Point(X() + b.X(), Y() + b.Y(), Z() + b.Z());
}

Point Point::operator-(const Point& b) const {
  return Point(X() - b.X(), Y() - b.Y(), Z() - b.Z());
}

Point& Point::operator*=(double b) {
  data_->setX(this->X() * b);
  data_->setY(this->Y() * b);
  data_->setZ(this->Z() * b);
  return *this;
}

Point Point::operator*(double b) const {
  return Point(X() * b, Y() * b, Z() * b);
}

double Point::operator*(const Point& b) const {
  return X() * b.X() + Y() * b.Y() + Z() * b.Z();
}

Point Point::operator/(double b) const {
  return Point(X() / b, Y() / b, Z() / b);
}

double Point::det(const Point& u, const Point& v) {
  return u.X() * v.Y() - u.Y() * v.X();
}

Point Point::clone() const {
  return Point(this->X(), this->Y(), this->Z());
}

double Point::X() const {return data_->getX();}
double Point::Y() const {return data_->getY();}
double Point::Z() const {return data_->getZ();}
double Point::getX() const {return data_->getX();}
double Point::getY() const {return data_->getY();}
double Point::getZ() const {return data_->getZ();}

void Point::setX(double x) {data_->setX(x);}
void Point::setY(double y) {data_->setY(y);}
void Point::setZ(double z) {data_->setZ(z);}

void Point::setPoint(double x, double y, double z) {
  data_->setX(x);
  data_->setY(y);
  data_->setZ(z);
}

void Point::setPoint(const OGRPoint& p) {
  data_->setX(p.getX());
  data_->setY(p.getY());
  data_->setZ(p.getZ());
}

void Point::setPoint(const Point& p) {
  data_->setX(p.X());
  data_->setY(p.Y());
  data_->setZ(p.Z());
}

Point Point::rotateFromPoint(double angle, const Point& p_r) const {
  const double s = sin(angle);
  const double c = cos(angle);
  double x = this->X();
  double y = this->Y();
  double p_x = p_r.X() - x;
  double p_y = p_r.Y() - y;
  return Point(p_x * c - p_y * s + x,
               p_x * s + p_y * c + y);
}

double Point::getAngleFromPoints(const Point& end) const {
  return mod_2pi(atan2(det(*this, end), *this * end));
}

double Point::getAngleFromPoint() const {
  return Point(1.0, 0.0).getAngleFromPoints(*this);
}

double Point::getAngleFromPoints(
    const Point& p1, const Point& p2, const Point& p3) {
  return mod_2pi((p3-p2).getAngleFromPoint() - (p2-p1).getAngleFromPoint());
}

Point Point::getPointFromAngle(double angle, double dist) const {
  return Point(X() + dist * cos(angle), Y() + dist * sin(angle));
}

double Point::signedDistance2Segment(
    const Point& start, const Point& end) const {
  Point this2start {start - *this};
  Point end2start {end - start};
  return -det(end2start, this2start) / sqrt(end2start * end2start);
}


Point Point::intersectionOfLines(
    const Point& l1_s, const Point& l1_e,
    const Point& l2_s, const Point& l2_e) {
  double den = det(l1_e - l1_s, l2_e - l2_s);
  if (den == 0) {
    return l1_s;
  }
  double det1 = det(l1_e, l1_s);
  double det2 = det(l2_e, l2_s);
  return ((l2_e - l2_s) * det1 - (l1_e - l1_s) * det2) / den;
}

Point Point::closestPointInSegment(
    const Point& seg_s, const Point& seg_e) const {
  Point v = seg_e - seg_s;
  Point u = seg_s - *this;
  double t = -(v * u) / (v * v);
  return v * std::max(0.0, std::min(1.0, t)) + seg_s;
}

}  // namespace f2c::types

