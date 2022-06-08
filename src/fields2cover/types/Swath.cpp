//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Swath.h"

namespace f2c {
namespace types {

Swath::Swath() = default;
Swath::Swath(const LineString& _path) : path_(_path) {}

Swath::Swath(double _width) {
  this->setWidth(_width);
}

Swath::Swath(const LineString& _path, double _width, int _id) :
    id_(_id), path_(_path) {
  this->setWidth(_width);
}

Swath::Swath(const Swath&) = default;
Swath::~Swath() = default;
Swath& Swath::operator=(Swath&&) = default;
Swath& Swath::operator=(const Swath&) = default;

bool Swath::operator!=(const Swath& s) const {
  if (s.getWidth() != this->getWidth()) {
    return true;
  }
  Point this_p {this->startPoint()};
  Point s_p = (this->hasSameDir(s) ? s.startPoint() : s.endPoint());
  return (this_p.getX() != s_p.getX() || this_p.getY() != s_p.getY()) ||
    s.getLength() != this->getLength();
}

bool Swath::operator==(const Swath& s) const {
  return !(s != *this);
}

bool Swath::operator<(const Swath& s) const {
  return this->getId() < s.getId();
}

bool Swath::operator>(const Swath& s) const {
  return this->getId() > s.getId();
}

bool Swath::operator>=(const Swath& s) const {
  return (this->getId() > s.getId() ||
      (this->getId() == s.getId() && *this == s));
}

bool Swath::operator<=(const Swath& s) const {
  return (this->getId() < s.getId() ||
      (this->getId() == s.getId() && *this == s));
}

double Swath::getLength(void) const {
  return this->path_.getLength();
}

double Swath::getArea(void) const {
  return this->computeAreaCovered().getArea();
}

double Swath::getArea(const Cells& _polys) const {
  return this->computeAreaCovered(_polys).getArea();
}

void Swath::reverse(void) {
  this->path_.reversePoints();
  this->creation_dir_ = !this->creation_dir_;
}

double Swath::getInAngle(void) const {
  if (path_.isEmpty()) {
    return -1.0;
  }
  return Point(path_.getX(1) - path_.getX(0), path_.getY(1) - path_.getY(0))
    .getAngleFromPoint();
}

double Swath::getOutAngle(void) const {
  if (this->path_.isEmpty()) {
    return -1.0;
  }
  auto n = this->getNumPoints() - 1;
  return Point(path_.getX(n) - path_.getX(n - 1),
      path_.getY(n) - path_.getY(n-1)).getAngleFromPoint();
}

Point Swath::startPoint(void) const {
  if (path_.isEmpty()) {
    return Point();
  }
  return path_.StartPoint();
}

Point Swath::endPoint(void) const {
  if (path_.isEmpty()) {
    return Point();
  }
  return path_.EndPoint();
}

Cells Swath::computeAreaCovered(void) const {
  if (path_.isEmpty()) {
    return Cells();
  }
  return Cells::Buffer(path_, width_ / 2.0);
}

Cells Swath::computeAreaCovered(const Cells& _polys) const {
  return _polys.getCellsInside(computeAreaCovered());
}

Swath Swath::clone() const {
  Swath new_s {this->path_.clone(), this->width_, this->id_};
  new_s.setCreationDir(this->getCreationDir());
  return new_s;
}

bool Swath::hasSameDir(const Swath& s) const {
  return s.getCreationDir() == this->getCreationDir();
}

void Swath::targetSameDirAs(const Swath& s) {
  if (!this->hasSameDir(s)) {
    this->reverse();
  }
}

void Swath::targetOppositeDirAs(const Swath& s) {
  if (this->hasSameDir(s)) {
    this->reverse();
  }
}

size_t Swath::getNumPoints() const {
  return this->path_->getNumPoints();
}

Point Swath::getPoint(int i) {
  return this->path_.getGeometry(i);
}

const Point Swath::getPoint(int i) const {
  return this->path_.getGeometry(i);
}

int Swath::getId() const {
  return this->id_;
}

void Swath::setId(int _id) {
  this->id_ = _id;
}

LineString Swath::getPath() const {
  return this->path_;
}

void Swath::setPath(const LineString& _path) {
  this->path_ = _path;
}

double Swath::getWidth() const {
  return this->width_;
}

void Swath::setWidth(double _width) {
  if (0 >= _width) {
    throw std::invalid_argument("Width needs to be positive");
  }
  this->width_ = _width;
}

bool Swath::getCreationDir() const {
  return this->creation_dir_;
}

void Swath::setCreationDir(bool _creation_dir) {
  this->creation_dir_ = _creation_dir;
}



}  // namespace types
}  // namespace f2c
