//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/LineString.h"

namespace f2c {
namespace types {

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
  data->getPoint(i, point.get());
}

void LineString::getGeometry(int i, Point& point) const {
  data->getPoint(i, point.get());
}

Point LineString::getGeometry(int i) {
  OGRPoint point;
  data->getPoint(i, &point);
  return Point(point);
}

const Point LineString::getGeometry(int i) const {
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


}  // namespace types
}  // namespace f2c
