//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <algorithm>
#include <vector>
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

bool LinearRing::isClosed() const {
  return data_->IsEmpty() || data_->get_IsClosed();
}

LinearRing& LinearRing::closeRing() {
  if (!this->isClosed()) {
    this->addPoint(this->startPoint());
  }
  return *this;
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

LinearRing& LinearRing::removePoint(size_t i) {
  this->data_->removePoint(i);
  return *this;
}

LineString LinearRing::getSegment(size_t i) const {
  return LineString(this->at(i), this->at(i+1));
}

LineString LinearRing::getLastSegment() const {
  return this->getSegment(this->size()-2);
}

double LinearRing::segmentLength(size_t i) const {
  return this->at(i).distance(this->at(i+1));
}

double LinearRing::segmentAng(size_t i) const {
  return (this->at(i + 1) - this->at(i)).getAngleFromPoint();
}

LinearRing& LinearRing::filterSelfIntersections() {
  if (this->size() <= 3) {
    return *this;
  }
  for (size_t i = 0; i + 2 < size(); ++i) {
    auto s_i = this->getSegment(i);
    auto s_j = this->getSegment(i+1);
    if (s_i.within(s_j) || s_i.contains(s_j)) {
      this->removePoint(i+1);
    }
  }
  for (size_t i = 0; i + 2 < size(); ++i) {
    for (size_t j = this->size() - 2; j > i + 1; --j) {
      if (i == 0 && j + 2 == size()) {continue;}
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
  return *this;
}

namespace {
// A sharp corner with very different offsets on each side sends the plain
// line-line miter join arbitrarily far from the original vertex -- outside
// the source polygon, in the worst cases. Past this many times the larger
// of the two offsets, bevel (both offset segment endpoints, joined by a
// straight edge) instead of the single miter point.
constexpr double kMiterLimit = 4.0;

void addMiterOrBevel(const Point& p0_prev, const Point& p1_prev,
    const Point& p0_curr, const Point& p1_curr, const Point& orig,
    double d_prev, double d_curr, LinearRing* ring) {
  Point miter = Point::intersectionOfLines(p0_prev, p1_prev, p0_curr, p1_curr);
  double max_d = std::max(fabs(d_prev), fabs(d_curr));
  if (max_d > 0.0 && miter.distance(orig) > kMiterLimit * max_d) {
    ring->addPoint(p1_prev);
    ring->addPoint(p0_curr);
  } else {
    ring->addPoint(miter);
  }
}
}  // namespace

LinearRing LinearRing::getParallelLine(const std::vector<double>& d) const {
  if (this->size() != d.size() + 1) {
    throw std::invalid_argument(
        "Error LinearRing::getParallelLine. "  \
        "LinearRing::size() and d.size() + 1 are not equal");
  }

  std::vector<Point> ps0, ps1;
  for (size_t i = 0; i < d.size(); ++i) {
    double ang = this->segmentAng(i);
    ps0.emplace_back(at(i).getPointFromAngle(ang + 0.5 * M_PI, d[i]));
    ps1.emplace_back(at(i+1).getPointFromAngle(ang + 0.5 * M_PI, d[i]));
  }

  LinearRing ring;
  addMiterOrBevel(ps0.back(), ps1.back(), ps0[0], ps1[0],
      at(0), d.back(), d[0], &ring);
  for (size_t i = 1; i < ps0.size(); ++i) {
    addMiterOrBevel(ps0[i-1], ps1[i-1], ps0[i], ps1[i],
        at(i), d[i-1], d[i], &ring);
  }
  ring.addPoint(ring[0]);
  return ring;
}

LinearRing LinearRing::getParallelLine(double d) const {
  return getParallelLine(std::vector<double>(this->size()-1, d));
}

LinearRing& LinearRing::bufferOutwards(const std::vector<double>& d) {
  auto ds = d;
  if (!this->isClockwise()) {
    for (auto&& d_val : ds) {
      d_val *= -1.0;
    }
  }
  *this = this->getParallelLine(ds);
  return *this;
}

LinearRing& LinearRing::bufferInwards(const std::vector<double>& d) {
  auto ds = d;
  if (this->isClockwise()) {
    for (auto&& d_val : ds) {
      d_val *= -1.0;
    }
  }
  *this = this->getParallelLine(ds);
  return *this;
}


}  // namespace f2c::types

