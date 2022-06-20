//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Cell.h"

namespace f2c {
namespace types {

Cell::Cell() {
  data = std::shared_ptr<OGRPolygon>(
    static_cast<OGRPolygon*>(OGRGeometryFactory::createGeometry(wkbPolygon)),
    [](OGRPolygon* f) {OGRGeometryFactory::destroyGeometry(f);});
}

Cell::Cell(const OGRGeometry* geom) {
  if (wkbFlatten(geom->getGeometryType()) == OGRwkbGeometryType::wkbPolygon) {
    this->data = std::shared_ptr<OGRPolygon>(geom->clone()->toPolygon(),
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

void Cell::getGeometry(int i, LinearRing& ring) {
  ring = LinearRing(((i == 0) ?
    data->getExteriorRing() :
    data->getInteriorRing(i-1)), EmptyDestructor());
}

void Cell::getGeometry(int i, LinearRing& ring) const {
  ring = LinearRing(((i == 0) ?
    data->getExteriorRing() :
    data->getInteriorRing(i-1)), EmptyDestructor());
}

LinearRing Cell::getGeometry(int i) {
  return (i == 0) ? getExteriorRing() : getInteriorRing(i-1);
}

LinearRing Cell::getGeometry(int i) const {
  return (i == 0) ? getExteriorRing() : getInteriorRing(i-1);
}

void Cell::setGeometry(int i, const LinearRing& line) {
  auto n = this->size();
  if (i < n) {
    auto* ref = this->getGeometry(i).get();
    *ref = *line.get();
    return;
  } else if (i > n) {
    for (int j = i; j < n; ++j) {
      this->addGeometry(LinearRing());
    }
  }
  this->addGeometry(line);
}

size_t Cell::size() const {
  return isEmpty() ? 0 : (1 + data->getNumInteriorRings());
}


Cell Cell::Buffer(const Cell& geom, double width) {
  auto buffer = geom->Buffer(width);
  Cell cell {buffer->toPolygon()};
  OGRGeometryFactory::destroyGeometry(buffer);
  return cell;
}

Cell Cell::Buffer(const LineString& geom, double width) {
  auto buffer = MultiLineString::getLineSegments(geom)->Buffer(width);
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
  this->data->addRing(t.get());
}

void Cell::addGeometry(const LinearRing& ring) {
  addRing(ring);
}

LinearRing Cell::getExteriorRing() const {
  return LinearRing(data->getExteriorRing());
}

LinearRing Cell::getInteriorRing(int i) const {
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
  for (auto&& ring : *this) {
    if (p.Within(MultiLineString::getLineSegments(LineString(ring)))) {
      return true;
    }
  }
  return false;
}

bool Cell::isPointIn(const Point& p) const {
  return p.Touches(*this);
}

LineString Cell::createLineUntilBorder(
    const f2c::types::Point& p, double ang) {
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

Cell Cell::Intersection(const Cell& c) const {
  auto inter = data->Intersection(c.get());
  if (wkbFlatten(inter->getGeometryType()) ==
        OGRwkbGeometryType::wkbPolygon) {
    Cell cell(inter->toMultiPolygon());
    OGRGeometryFactory::destroyGeometry(inter);
    return cell;
  } else {
    OGRGeometryFactory::destroyGeometry(inter);
    return Cell();
  }
}

}  // namespace types
}  // namespace f2c
