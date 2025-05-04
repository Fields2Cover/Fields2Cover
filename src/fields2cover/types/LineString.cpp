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

LinearRing LineString::toLinearRing() const {
  LinearRing ring;
  for (size_t i = 0; i < this->size(); ++i) {
    ring.addPoint(this->at(i));
  }
  if (this->at(0) != this->at(this->size() - 1)) {
    ring.addPoint(this->at(0));
  }
  return ring;
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

LineString& LineString::removePoint(size_t i) {
  this->data_->removePoint(i);
  return *this;
}


void LineString::operator*=(double b) {
  for (auto&& p : *this) {
    p *= b;
  }
}

bool LineString::operator==(const LineString& o_line) const {
  if (this->size() != o_line.size()) {
    return false;
  }
  for (size_t i = 0; i < this->size(); ++i) {
    if (this->at(i) != o_line.at(i)) {
      return false;
    }
  }
  return true;
}

bool LineString::operator!=(const LineString& o_line) const {
  return !(this->operator==(o_line));
}

void LineString::addPoint(double x, double y, double z) {
  data_->addPoint(x, y, z);
}

void LineString::addPoint(const Point& p) {
  data_->addPoint(p.getX(), p.getY(), p.getZ());
}

void LineString::concat(const LineString& line) {
  for (auto&& p : line) {
    this->addPoint(p);
  }
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

LineString LineString::getSegment(size_t i) const {
  return LineString(this->at(i), this->at(i+1));
}

LineString LineString::getLastSegment() const {
  return this->getSegment(size()-2);
}

double LineString::segmentLength(size_t i) const {
  return this->at(i).distance(this->at(i+1));
}

double LineString::segmentAng(size_t i) const {
  return (this->at(i + 1) - this->at(i)).getAngleFromPoint();
}

LineString& LineString::filterSelfIntersections() {
  if (this->size() < 3) {
    return *this;
  }

  if (false && this->getSegment(0).intersects(this->getLastSegment())) {
    auto p_i = Point::intersectionOfLines(
        at(0), at(1), at(size() - 2), at(size() - 1));
    this->setGeometry(0, p_i);
    this->setGeometry(size() - 1, p_i);
    auto ring = this->toLinearRing();
    ring.filterSelfIntersections();
    *this = ring.toLineString();
  } else {
    for (size_t i = 0; i + 2 < size(); ++i) {
      for (size_t j = this->size() - 2; j > i + 1; --j) {
        if (this->getSegment(i).intersects(this->getSegment(j))) {
          auto p_i = Point::intersectionOfLines(at(i), at(i+1), at(j), at(j+1));
          for (size_t k = j; k > i + 1; --k) {
            this->removePoint(k);
          }
          this->setGeometry(i+1, p_i);
          break;
        }
      }
    }
  }
  return *this;
}

LineString& LineString::extend(double L) {
  Point p0 {startPoint().getPointFromAngle(startAngle(), -L)};
  Point p1 {endPoint().getPointFromAngle(endAngle(),      L)};

  LineString new_line {p0};
  for (size_t i = 1; i < this->size()-1; ++i) {
    new_line.addPoint(this->at(i));
  }
  new_line.addPoint(p1);
  *this = new_line;
  return *this;
}

LineString& LineString::reduceLength(double final_len) {
  const double len = this->length();
  if (len <= final_len) {
    return *this;
  }
  double remove_len = len - final_len;
  double prev_len = 0.5 * remove_len;

  double dist = this->segmentLength(0);
  while (prev_len >= dist) {
    prev_len -= dist;
    removePoint(0);
    dist = this->segmentLength(0);
  }
  this->setGeometry(0,
      this->startPoint().getPointFromAngle(this->startAngle(), prev_len));

  double post_len = 0.5 * remove_len;
  dist = this->segmentLength(this->size()-2);
  while (post_len >= dist) {
    post_len -= dist;
    removePoint(this->size()-1);
    dist = this->segmentLength(this->size()-2);
  }
  this->setGeometry(this->size()-1,
      this->at(this->size()-2).getPointFromAngle(
        this->endAngle(), dist - post_len));
  return *this;
}

LineString LineString::getParallelLine(double d) const {
  return getParallelLine(std::vector<double>(this->size()-1, d));
}


LineString LineString::getParallelLine(const std::vector<double>& d) const {
  if (this->size() != d.size() + 1) {
    throw std::invalid_argument(
        "Error LineString::getParallelLine. "  \
        "LineString::size() and d.size() + 1 are not equal");
  }
  auto c_this = this->clone().extend(10.0*this->length());

  std::vector<Point> ps0, ps1;
  for (size_t i = 0; i < d.size(); ++i) {
    double ang = c_this.segmentAng(i);
    ps0.emplace_back(c_this.at(i).getPointFromAngle(ang + 0.5 * M_PI, d[i]));
    ps1.emplace_back(c_this.at(i+1).getPointFromAngle(ang + 0.5 * M_PI, d[i]));
  }

  LineString line {ps0[0]};
  for (size_t i = 1; i < ps0.size(); ++i) {
    line.addPoint(Point::intersectionOfLines(
          ps0[i-1], ps1[i-1], ps0[i], ps1[i]));
  }

  line.addPoint(ps1.back());

  line.filterSelfIntersections();

  line.reduceLength(this->length());
  return line;
}

std::ostream& operator<<(std::ostream& os, const LineString& line) {
  os << "LineString(";
  for (size_t i = 0; i < line.size(); ++i) {
    if (i > 0) {
      os << ", ";
    }
    os << line[i];
  }
  os << ")";
  return os;
}


}  // namespace f2c::types

