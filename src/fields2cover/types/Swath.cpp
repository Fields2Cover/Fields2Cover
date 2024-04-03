//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Swath.h"

namespace f2c::types {

Swath::Swath() = default;
Swath::Swath(const LineString& _path) : path_(_path) {}

Swath::Swath(double _width) {
  this->setWidth(_width);
}

Swath::Swath(const LineString& _path, double _width, int _id, SwathType _type) :
    id_(_id), path_(_path), type_(_type) {
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
    s.length() != this->length();
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

double Swath::length() const {
  return this->path_.length();
}

double Swath::area() const {
  return this->areaCovered().area();
}

double Swath::area(const Cells& polys) const {
  return this->areaCovered(polys).area();
}

void Swath::reverse() {
  this->path_.reversePoints();
  this->creation_dir_ = !this->creation_dir_;
}

double Swath::getInAngle() const {
  if (this->path_.isEmpty()) {
    return -1.0;
  }
  return (path_.getGeometry(1) - path_.getGeometry(0)).getAngleFromPoint();
}

double Swath::getOutAngle() const {
  if (this->path_.isEmpty()) {
    return -1.0;
  }
  auto n = this->numPoints() - 1;
  return (path_.getGeometry(n) - path_.getGeometry(n-1)).getAngleFromPoint();
}

Point Swath::startPoint() const {
  if (this->path_.isEmpty()) {
    return {};
  }
  return this->path_.startPoint();
}

Point Swath::endPoint() const {
  if (this->path_.isEmpty()) {
    return {};
  }
  return this->path_.endPoint();
}

Cells Swath::areaCovered() const {
  if (this->path_.isEmpty()) {
    return Cells();
  }
  return Cells::buffer(this->path_, 0.5 * this->width_);
}

Cells Swath::areaCovered(const Cells& polys) const {
  return polys.getCellsInside(this->areaCovered());
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

size_t Swath::numPoints() const {
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

SwathType Swath::getType() const {
  return this->type_;
}

void Swath::setType(SwathType _type) {
  this->type_ = _type;
}

void Swath::moveTo(const Point& ref_pt) {
  for (auto&& s : this->path_) {
    s = s + ref_pt;
  }
}

}  // namespace f2c::types

