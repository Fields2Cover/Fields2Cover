//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Cell.h"
#include "fields2cover/types/Cells.h"

namespace f2c::types {

Cell::Cell() {
  this->data_ = std::shared_ptr<OGRPolygon>(
    downCast<OGRPolygon*>(OGRGeometryFactory::createGeometry(wkbPolygon)),
    [](OGRPolygon* f) {OGRGeometryFactory::destroyGeometry(f);});
}

Cell::Cell(const OGRGeometry* geom) {
  if (wkbFlatten(geom->getGeometryType()) == OGRwkbGeometryType::wkbPolygon) {
    this->data_ =
      std::shared_ptr<OGRPolygon>(downCast<OGRPolygon*>(geom->clone()),
        [](OGRPolygon* f) {OGRGeometryFactory::destroyGeometry(f);});
  } else {
    throw std::invalid_argument(
        sstr("Cell(const OGRGeometry*): Type of OGRGeometry* is ",
        wkbFlatten(geom->getGeometryType()) , " instead of wkbPolygon(" ,
          OGRwkbGeometryType::wkbPolygon , ")"));
  }
}

Cell::Cell(const f2c::types::LinearRing& ring) {
  this->addRing(ring);
}

void Cell::getGeometry(size_t i, LinearRing& ring) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }
  ring = LinearRing(((i == 0) ?
    this->data_->getExteriorRing() :
    this->data_->getInteriorRing(i-1)), EmptyDestructor());
}

void Cell::getGeometry(size_t i, LinearRing& ring) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }

  ring = LinearRing(((i == 0) ?
    this->data_->getExteriorRing() :
    this->data_->getInteriorRing(i-1)), EmptyDestructor());
}

LinearRing Cell::getGeometry(size_t i) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }
  return (i == 0) ? this->getExteriorRing() : this->getInteriorRing(i-1);
}

const LinearRing Cell::getGeometry(size_t i) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }
  return (i == 0) ? this->getExteriorRing() : this->getInteriorRing(i-1);
}

void Cell::setGeometry(size_t i, const LinearRing& line) {
  auto n = this->size();
  if (i < n) {
    Cell cell;
    for (size_t j = 0; j < n; ++j) {
      cell.addGeometry((i == j) ? line : this->getGeometry(j));
    }
    *this = cell;
    return;
  } else if (i != n) {
    for (size_t j = n; j < i; ++j) {
      this->addGeometry(LinearRing());
    }
  }
  this->addGeometry(line);
}

size_t Cell::size() const {
  return this->isEmpty() ? 0 : (1 + this->data_->getNumInteriorRings());
}

void Cell::operator*=(double b) {
  for (auto&& ring : *this) {
    ring *= b;
  }
}

Cell Cell::buffer(const Cell& geom, double width) {
  return destroyResGeom<Cell>(geom.OGRBuffer(width));
}

Cell Cell::buffer(const LineString& geom, double width) {
  return destroyResGeom<Cell>(geom.OGRBuffer(width));
}

Cell Cell::buffer(const LinearRing& geom, double width) {
  Cell cell(geom);
  Cells b_cell_out {buffer(cell, fabs(width))};
  Cell b_cell_in = buffer(cell, -fabs(width));
  auto diff = b_cell_out.difference(b_cell_in);
  return (diff.size() > 0 ? diff.getGeometry(0) : Cell());
}

Cell Cell::buffer(const Point& geom, double width) {
  return destroyResGeom<Cell>(geom->Buffer(width));
}

Cell Cell::convexHull() const {
  return destroyResGeom<Cell>(this->data_->ConvexHull());
}

void Cell::addRing(const LinearRing& t) {
  auto r = t.clone();
  this->data_->addRing(r.get());
}

void Cell::addGeometry(const LinearRing& ring) {
  this->addRing(ring);
}

const LinearRing Cell::getExteriorRing() const {
  return LinearRing(this->data_->getExteriorRing());
}

const LinearRing Cell::getInteriorRing(size_t i) const {
  return LinearRing(this->data_->getInteriorRing(i));
}


bool Cell::isConvex() const {
  if (this->data_->getNumInteriorRings() > 0 || this->data_->get_Area() <= 0) {
    return false;
  }
  auto border = this->getExteriorRing();
  if (border.size() < 2) {
    return false;
  }
  auto getAng = [&border] (size_t i) {
    return Point::getAngleFromPoints(
        border.getGeometry(i),
        border.getGeometry(i + 1),
        border.getGeometry(i + 2));
  };
  bool inv = M_PI > getAng(0);
  for (size_t i = 1; i < border.size() - 2; ++i) {
    if (inv != (getAng(i) < M_PI)) {
      return false;
    }
  }
  return true;
}

LineString Cell::createSemiLongLine(const Point& point, double angle) const {
  return LineString({point,
    point.getPointFromAngle(angle, this->getMinSafeLength())});
}

LineString Cell::createStraightLongLine(
    const Point& point, double angle) const {
  return LineString({
    point.getPointFromAngle(M_PI + angle, this->getMinSafeLength()),
    point.getPointFromAngle(angle,        this->getMinSafeLength())});
}

MultiLineString Cell::getLinesInside(const LineString& line) const {
  return MultiLineString::intersection(line, *this);
}

MultiLineString Cell::getLinesInside(const MultiLineString& lines) const {
  return lines.intersection(*this);
}

bool Cell::isPointInBorder(const Point& p) const {
  return p.touches(*this);
}

bool Cell::isPointIn(const Point& p) const {
  return p.within(*this);
}

LineString Cell::createLineUntilBorder(
    const f2c::types::Point& p, double ang) const {
  const MultiLineString ray(this->createSemiLongLine(p, ang));
  const auto intersections(ray.intersection(*this));
  Point best_point{p};
  double min_dist {std::numeric_limits<double>::max()};
  for (auto&& line : intersections) {
    for (auto&& p_l : line) {
      double dist = p_l.distance(p);
      if (dist > 1e-5 && min_dist > dist) {
        best_point = p_l;
        min_dist = dist;
      }
    }
  }
  return (best_point == p) ? LineString() : LineString({p, best_point});
}

Point Cell::closestPointOnBorderTo(const Point& p) const {
  std::vector<double> dist;
  std::vector<Point> ps;
  for (auto&& ring : *this) {
    ps.emplace_back(ring.closestPointTo(p));
    dist.emplace_back(ps.back().distance(p));
  }
  return ps[std::min_element(dist.begin(), dist.end()) - dist.begin()];
}


}  // namespace f2c::types

