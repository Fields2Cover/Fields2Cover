//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/LineString.h"

namespace f2c::types {

LineString::LineString() {
  data = std::shared_ptr<OGRLineString>(
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

double LineString::getX(size_t i) const {return data->getX(i);}
double LineString::getY(size_t i) const {return data->getY(i);}
double LineString::getZ(size_t i) const {return data->getZ(i);}
double LineString::getLength() const {return this->data->get_Length();}
void LineString::reversePoints() const { this->data->reversePoints();}
size_t LineString::size() const {
  return isEmpty() ? 0 : this->data->getNumPoints();
}


void LineString::getGeometry(int i, Point& point) {
 if (i < 0 || i >= this->size()) {
    throw std::out_of_range("Geometry does not contain point " + std::to_string(i));
  }
  data->getPoint(i, point.get());
}

void LineString::getGeometry(int i, Point& point) const {
 if (i < 0 || i >= this->size()) {
    throw std::out_of_range("Geometry does not contain point " + std::to_string(i));
  }
  data->getPoint(i, point.get());
}

Point LineString::getGeometry(int i) {
 if (i < 0 || i >= this->size()) {
    throw std::out_of_range("Geometry does not contain point " + std::to_string(i));
  }
  
  OGRPoint point;
  data->getPoint(i, &point);
  return Point(point);
}

const Point LineString::getGeometry(int i) const {
  if (i < 0 || i >= this->size()) {
    throw std::out_of_range("Geometry does not contain point " + std::to_string(i));
  }

  OGRPoint point;
  data->getPoint(i, &point);
  return Point(point);
}

void LineString::setGeometry(int i, const Point& p) {
  data->setPoint(i, p.get());
}


void LineString::operator*=(double b) {
  for (auto&& p : *this) {
    p *= b;
  }
}


void LineString::addPoint(double x, double y, double z) {
  data->addPoint(x, y, z);
}
void LineString::addPoint(const Point& p) {
  data->addPoint(p.get());
}

Point LineString::StartPoint() const {
  OGRPoint p;
  data->StartPoint(&p);
  return Point(p);
}

Point LineString::EndPoint() const {
  OGRPoint p;
  data->EndPoint(&p);
  return Point(p);
}

double LineString::startAngle() const {
  return (getGeometry(1) - StartPoint()).getAngleFromPoint();
}
double LineString::endAngle() const {
  return (EndPoint() - getGeometry(size() - 2)).getAngleFromPoint();
}




}  // namespace f2c::types

