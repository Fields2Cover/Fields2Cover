//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Cell.h"
#include "fields2cover/types/Cells.h"

namespace f2c::types {

Cell::Cell() {
  data = std::shared_ptr<OGRPolygon>(
    downCast<OGRPolygon*>(OGRGeometryFactory::createGeometry(wkbPolygon)),
    [](OGRPolygon* f) {OGRGeometryFactory::destroyGeometry(f);});
}

Cell::Cell(const OGRGeometry* geom) {
  if (wkbFlatten(geom->getGeometryType()) == OGRwkbGeometryType::wkbPolygon) {
    this->data =
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

void Cell::operator*=(double b) {
  for (auto&& ring : *this) {
    ring *= b;
  }
}

void Cell::getGeometry(size_t i, LinearRing& ring) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }

  ring = LinearRing(((i == 0) ?
    data->getExteriorRing() :
    data->getInteriorRing(i-1)), EmptyDestructor());
}

void Cell::getGeometry(size_t i, LinearRing& ring) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }

  ring = LinearRing(((i == 0) ?
    data->getExteriorRing() :
    data->getInteriorRing(i-1)), EmptyDestructor());
}

LinearRing Cell::getGeometry(size_t i) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }
  return (i == 0) ? getExteriorRing() : getInteriorRing(i-1);
}

const LinearRing Cell::getGeometry(size_t i) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }
  return (i == 0) ? getExteriorRing() : getInteriorRing(i-1);
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
  return isEmpty() ? 0 : (1 + data->getNumInteriorRings());
}


Cell Cell::Buffer(const Cell& geom, double width) {
  auto buffer = geom.OGRBuffer(width);
  Cell cell {buffer->toPolygon()};
  OGRGeometryFactory::destroyGeometry(buffer);
  return cell;
}

Cell Cell::Buffer(const LineString& geom, double width) {
  auto buffer = geom.OGRBuffer(width);
  Cell cell {buffer->toPolygon()};
  OGRGeometryFactory::destroyGeometry(buffer);
  return cell;
}

Cell Cell::Buffer(const LinearRing& geom, double width) {
  auto buffer = geom.OGRBuffer(width);
  Cell cell {buffer->toPolygon()};
  OGRGeometryFactory::destroyGeometry(buffer);
  return cell;
}

Cell Cell::Buffer(const Point& geom, double width) {
  auto buffer = geom->Buffer(width);
  Cell cell {buffer->toPolygon()};
  OGRGeometryFactory::destroyGeometry(buffer);
  return cell;
}

void Cell::addRing(const LinearRing& t) {
  this->data->addRing(t.clone().get());
}

void Cell::addGeometry(const LinearRing& ring) {
  addRing(ring);
}

const LinearRing Cell::getExteriorRing() const {
  return LinearRing(data->getExteriorRing());
}

const LinearRing Cell::getInteriorRing(size_t i) const {
  return LinearRing(data->getInteriorRing(i));
}


bool Cell::isConvex() const {
  if (data->getNumInteriorRings() > 0 || data->get_Area() <= 0) {
    return false;
  }
  auto border = getExteriorRing();
  bool inv = boost::math::constants::pi<double>() >
      f2c::types::Point(border.getX(0) - border.getX(1),
        border.getY(0) - border.getY(1)).getAngleFromPoints(
      f2c::types::Point(border.getX(2) - border.getX(1),
        border.getY(2) - border.getY(1)));
  for (size_t i = 1; i < border.size() - 2; ++i) {
    auto ang =
        f2c::types::Point(border.getX(i) - border.getX(i + 1),
          border.getY(i) - border.getY(i + 1)).getAngleFromPoints(
        f2c::types::Point(border.getX(i + 2) - border.getX(i + 1),
          border.getY(i + 2) - border.getY(i + 1)));
    if (inv != (ang < boost::math::constants::pi<double>())) {
      return false;
    }
  }
  return true;
}

Cell Cell::ConvexHull() const {
  auto convex_hull = data->ConvexHull();
  Cell cell(convex_hull);
  OGRGeometryFactory::destroyGeometry(convex_hull);
  return cell;
}

LineString Cell::getSemiLongCurve(const Point& point, double angle) const {
  return LineString({point,
    point.getPointFromAngle(angle, this->getMinSafeLength())});
}


LineString Cell::getStraightLongCurve(
    const Point& point, double angle) const {
  return LineString({
    point.getPointFromAngle(angle, this->getMinSafeLength()),
    point.getPointFromAngle(
    boost::math::constants::pi<double>() + angle, this->getMinSafeLength())});
}

MultiLineString Cell::getLinesInside(const LineString& line) const {
  return MultiLineString::Intersection(line, *this);
}

MultiLineString Cell::getLinesInside(const MultiLineString& lines) const {
  return lines.Intersection(*this);
}

bool Cell::isPointInBorder(const Point& p) const {
  return p.Touches(*this);
}

bool Cell::isPointIn(const Point& p) const {
  return p.Within(*this);
}

LineString Cell::createLineUntilBorder(
    const f2c::types::Point& p, double ang) const {
  const MultiLineString ray(this->getSemiLongCurve(p, ang));
  const auto intersections(ray.Intersection(*this));
  Point best_point{p};
  double min_dist {std::numeric_limits<double>::max()};
  for (auto&& line : intersections) {
    for (auto&& p_l : line) {
      double dist = p_l.Distance(p);
      if (dist > 1e-5 && min_dist > dist) {
        best_point = p_l;
        min_dist = dist;
      }
    }
  }
  return (best_point == p) ? LineString() : LineString({p, best_point});
}


}  // namespace f2c::types

