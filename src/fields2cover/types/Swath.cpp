//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Swath.h"

namespace f2c::types {

Swath::Swath() = default;
Swath::Swath(const LineString& path) : path_(path) {}

Swath::Swath(double width) {
  this->setWidth(width);
}

Swath::Swath(const LineString& path, double width, int id, SwathType type) :
    id_(id), path_(path), type_(type) {
  this->setWidth(width);
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

double Swath::getLength() const {
  return this->path_.getLength();
}

double Swath::getArea() const {
  return this->computeAreaCovered().getArea();
}

double Swath::getArea(const Cells& polys) const {
  return this->computeAreaCovered(polys).getArea();
}

void Swath::reverse() {
  this->path_.reversePoints();
  this->creation_dir_ = !this->creation_dir_;
}

double Swath::getInAngle() const {
  if (path_.isEmpty()) {
    return -1.0;
  }
  return Point(path_.getX(1) - path_.getX(0), path_.getY(1) - path_.getY(0))
    .getAngleFromPoint();
}

double Swath::getOutAngle() const {
  if (this->path_.isEmpty()) {
    return -1.0;
  }
  auto n = this->getNumPoints() - 1;
  return Point(path_.getX(n) - path_.getX(n - 1),
      path_.getY(n) - path_.getY(n-1)).getAngleFromPoint();
}

Point Swath::startPoint() const {
  if (path_.isEmpty()) {
    return Point();
  }
  return path_.StartPoint();
}

Point Swath::endPoint() const {
  if (path_.isEmpty()) {
    return Point();
  }
  return path_.EndPoint();
}

Cells Swath::computeAreaCovered() const {
  if (path_.isEmpty()) {
    return Cells();
  }
  return Cells::Buffer(path_, width_ / 2.0);
}

Cells Swath::computeAreaCovered(const Cells& polys) const {
  return polys.getCellsInside(computeAreaCovered());
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

void Swath::setId(int id) {
  this->id_ = id;
}

LineString Swath::getPath() const {
  return this->path_;
}

void Swath::setPath(const LineString& path) {
  this->path_ = path;
}

double Swath::getWidth() const {
  return this->width_;
}

void Swath::setWidth(double width) {
  if (0 >= width) {
    throw std::invalid_argument("Width needs to be positive");
  }
  this->width_ = width;
}

bool Swath::getCreationDir() const {
  return this->creation_dir_;
}

void Swath::setCreationDir(bool creation_dir) {
  this->creation_dir_ = creation_dir;
}

SwathType Swath::getType() const {
  return this->type_;
}

void Swath::setType(SwathType type) {
  this->type_ = type;
}

void Swath::moveTo(const Point& ref_pt) {
  for (auto&& s : this->path_) {
    s = s + ref_pt;
  }
}

}  // namespace f2c::types
