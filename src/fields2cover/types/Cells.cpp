//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Cells.h"

namespace f2c {
namespace types {

Cells::Cells() {
  data = std::shared_ptr<OGRMultiPolygon>(
    static_cast<OGRMultiPolygon*>(
      OGRGeometryFactory::createGeometry(wkbMultiPolygon)),
    [](OGRMultiPolygon* f) {OGRGeometryFactory::destroyGeometry(f);});
}

Cells::Cells(const OGRGeometry* _geom) {
  if (wkbFlatten(_geom->getGeometryType()) == OGRwkbGeometryType::wkbPolygon) {
    this->data = std::shared_ptr<OGRMultiPolygon>(static_cast<OGRMultiPolygon*>(
        OGRGeometryFactory::createGeometry(wkbMultiPolygon)),
        [](OGRMultiPolygon* f) {OGRGeometryFactory::destroyGeometry(f);});
    this->data->addGeometry(_geom->toPolygon());
  } else if (wkbFlatten(_geom->getGeometryType()) ==
    OGRwkbGeometryType::wkbMultiPolygon) {
    this->data = std::shared_ptr<OGRMultiPolygon>(
        _geom->clone()->toMultiPolygon(),
        [](OGRMultiPolygon* f) {OGRGeometryFactory::destroyGeometry(f);});
  } else {
    throw std::invalid_argument(sstr(
        "Cells(const OGRGeometry*): Type of OGRGeometry* is " ,
        wkbFlatten(_geom->getGeometryType()) , " instead of wkbPolygon(",
          OGRwkbGeometryType::wkbPolygon , ") or wkbMultiPolygon(",
          OGRwkbGeometryType::wkbMultiPolygon , ")"));
  }
}


Cells::Cells(const Cell& _c) {
  data->addGeometry(_c.get());
}

void Cells::operator*=(double b) {
  for (auto&& poly : *this) {
    poly *= b;
  }
}

void Cells::getGeometry(int i, Cell& cell) {
  cell = Cell(data->getGeometryRef(i), EmptyDestructor());
}

void Cells::getGeometry(int i, Cell& cell) const {
  cell = Cell(data->getGeometryRef(i), EmptyDestructor());
}

Cell Cells::getGeometry(int i) {
  return Cell(data->getGeometryRef(i));
}

const Cell Cells::getGeometry(int i) const {
  return Cell(data->getGeometryRef(i));
}

void Cells::setGeometry(int i, const Cell& cell) {
  auto n = this->size();
  if (i < n) {
    auto* ref = this->getGeometry(i).get();
    *ref = *cell.get();
    return;
  } else if (i > n) {
    for (int j = i; j < n; ++j) {
      this->addGeometry(Cell());
    }
  }
  this->addGeometry(cell);
}

void Cells::addGeometry(const Cell& _c) {
  this->data->addGeometry(_c.get());
}

void Cells::addRing(int i, const LinearRing& _ring) {
  this->data->getGeometryRef(i)->addRing(_ring.get());
}

size_t Cells::size() const {
  return isEmpty() ? 0 : data->getNumGeometries();
}

Cell Cells::getCell(int i) const {
  return Cell(data->getGeometryRef(i));
}

LinearRing Cells::getCellBorder(int i) const {
  return LinearRing(
      data->getGeometryRef(i)->getExteriorRing());
}

LinearRing Cells::getInteriorRing(int i_cell, int i_ring) const {
  return LinearRing(
      data->getGeometryRef(i_cell)->getInteriorRing(i_ring));
}

bool Cells::isConvex() const {
  return (isEmpty() || data->getNumGeometries() != 1)? false :
    getCell(0).isConvex();
}

Cells Cells::Intersection(const Cells& _c) const {
  auto inter = data->Intersection(_c.get());
  if (wkbFlatten(inter->getGeometryType()) ==
        OGRwkbGeometryType::wkbPolygon ||
      wkbFlatten(inter->getGeometryType()) ==
        OGRwkbGeometryType::wkbMultiPolygon) {
    Cells cells(inter->toMultiPolygon());
    OGRGeometryFactory::destroyGeometry(inter);
    return cells;
  } else {
    OGRGeometryFactory::destroyGeometry(inter);
    return Cells();
  }
}

Cells Cells::Difference(const Cells& _c) const {
  auto diff = data->Difference(_c.get());
  Cells cells(diff);
  OGRGeometryFactory::destroyGeometry(diff);
  return cells;
}

Cells Cells::splitByLine(const LineString& _line) const {
  // This trick could be faster using Polygonize
  return this->Difference(Buffer(_line, 1e-8));
}

Cells Cells::splitByLine(const MultiLineString& _lines) const {
  Cells cells{*this};
  for (auto&& line : _lines) {
    cells = cells.splitByLine(line);
  }
  return cells;
}

Cells Cells::Intersection(const Cell& _c) const {
  return Cells(_c).Intersection(*this);
}

Cells Cells::Intersection(
    const Cell& cell, const Cell& _c) {
  return Cells(cell).Intersection(_c);
}

LineString Cells::getSemiLongCurve(const Point& _point, double _angle) const {
  return LineString({_point,
    _point.getPointFromAngle(_angle, this->getMinSafeLength())});
}

LineString Cells::getStraightLongCurve(
    const Point& _point, double _angle) const {
  return LineString({
    _point.getPointFromAngle(_angle, this->getMinSafeLength()),
    _point.getPointFromAngle(
    boost::math::constants::pi<double>() + _angle, this->getMinSafeLength())});
}

MultiLineString Cells::getLinesInside(const LineString& _line) const {
  return MultiLineString::Intersection(_line, *this);
}

MultiLineString Cells::getLinesInside(const MultiLineString& _lines) const {
  return _lines.Intersection(*this);
}

Cells Cells::getCellsInside(const Cells& _cell) const {
  return this->Intersection(_cell);
}

bool Cells::isPointInBorder(const Point& p) const {
  for (auto&& cell : *this) {
    if (Cell(cell).isPointInBorder(p)) {
      return true;
    }
  }
  return false;
}

bool Cells::isPointIn(const Point& _p) const {
  return _p.Touches(*this);
}

Cell Cells::getCellWherePoint(const Point& p) const {
  for (auto&& cell : *this) {
    if (p.Touches(cell) || p.Within(cell)) {
      return cell;
    }
  }
  return Cell();
}

LineString Cells::createLineUntilBorder(
    const f2c::types::Point& _p, double _ang) const {
  return this->getCellWherePoint(_p).createLineUntilBorder(_p, _ang);
}


}  // namespace types
}  // namespace f2c
