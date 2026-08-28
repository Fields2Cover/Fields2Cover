//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <algorithm>
#include <cmath>
#include "fields2cover/types/Cells.h"

namespace f2c::types {

Cells::Cells() {
  this->data_ = std::shared_ptr<OGRMultiPolygon>(
    downCast<OGRMultiPolygon*>(
      OGRGeometryFactory::createGeometry(wkbMultiPolygon)),
    [](OGRMultiPolygon* f) {OGRGeometryFactory::destroyGeometry(f);});
}

Cells::Cells(const OGRGeometry* geom) {
  if (wkbFlatten(geom->getGeometryType()) == OGRwkbGeometryType::wkbPolygon) {
    this->data_ = std::shared_ptr<OGRMultiPolygon>(downCast<OGRMultiPolygon*>(
        OGRGeometryFactory::createGeometry(wkbMultiPolygon)),
        [](OGRMultiPolygon* f) {OGRGeometryFactory::destroyGeometry(f);});
    this->data_->addGeometry(geom->toPolygon());
  } else if (wkbFlatten(geom->getGeometryType()) ==
      OGRwkbGeometryType::wkbMultiPolygon) {
    this->data_ = std::shared_ptr<OGRMultiPolygon>(
        downCast<OGRMultiPolygon*>(geom->clone()),
        [](OGRMultiPolygon* f) {OGRGeometryFactory::destroyGeometry(f);});
  } else if (wkbFlatten(geom->getGeometryType()) ==
      OGRwkbGeometryType::wkbGeometryCollection) {
    this->data_ = std::shared_ptr<OGRMultiPolygon>(
        downCast<OGRMultiPolygon*>(
          OGRGeometryFactory::createGeometry(wkbMultiPolygon)),
            [](OGRMultiPolygon* f) {OGRGeometryFactory::destroyGeometry(f);});
    auto* gc = downCast<const OGRGeometryCollection*>(geom);
    for (int i = 0; i < gc->getNumGeometries(); ++i) {
      auto* g_i = gc->getGeometryRef(i);
      if (wkbFlatten(g_i->getGeometryType()) ==
          OGRwkbGeometryType::wkbPolygon) {
        this->data_->addGeometry(g_i);
      }
    }
  } else {
    this->data_ = std::shared_ptr<OGRMultiPolygon>(
        downCast<OGRMultiPolygon*>(
          OGRGeometryFactory::createGeometry(wkbMultiPolygon)),
            [](OGRMultiPolygon* f) {OGRGeometryFactory::destroyGeometry(f);});
  }
}


Cells::Cells(const Cell& c) {
  this->data_->addGeometry(c.get());
}

void Cells::operator*=(double b) {
  for (auto&& poly : *this) {
    poly *= b;
  }
}

void Cells::getGeometry(size_t i, Cell& cell) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }
  cell = Cell(this->data_->getGeometryRef(i), EmptyDestructor());
}

void Cells::getGeometry(size_t i, Cell& cell) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }
  cell = Cell(this->data_->getGeometryRef(i), EmptyDestructor());
}

Cell Cells::getGeometry(size_t i) {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Geometry does not contain point " + std::to_string(i));
  }
  return Cell(this->data_->getGeometryRef(i));
}

const Cell Cells::getGeometry(size_t i) const {
  if (i >= this->size()) {
    throw std::out_of_range(
        "Cells does not contain cell at " + std::to_string(i));
  }
  return Cell(this->data_->getGeometryRef(i));
}

void Cells::setGeometry(size_t i, const Cell& cell) {
  auto n = this->size();
  if (i < n) {
    Cells cells;
    for (size_t j = 0; j < n; ++j) {
      cells.addGeometry((i == j) ? cell : this->getGeometry(j));
    }
    *this = cells;
    return;
  } else if (i != n) {
    for (size_t j = n; j < i; ++j) {
      this->addGeometry(Cell());
    }
  }
  this->addGeometry(cell);
}

const Cell Cells::getCell(size_t i) const {
  return getGeometry(i);
}

const LinearRing Cells::getCellBorder(size_t i) const {
  // Check the index before asking OGR: getGeometryRef() dereferences a null
  // collection entry when the index is out of range.
  if (i >= static_cast<size_t>(this->data_->getNumGeometries())) {
    throw std::out_of_range(
        "Cells does not contain cell at " + std::to_string(i));
  }
  return LinearRing(
      downCast<OGRPolygon*>(this->data_->getGeometryRef(i))->getExteriorRing());
}

const LinearRing Cells::getInteriorRing(size_t i_cell, size_t i_ring) const {
  if (i_cell >= static_cast<size_t>(this->data_->getNumGeometries())) {
    throw std::out_of_range(
        "Cells does not contain cell at " + std::to_string(i_cell));
  }
  return LinearRing(downCast<OGRPolygon*>(this->data_->getGeometryRef(i_cell))
      ->getInteriorRing(i_ring));
}

void Cells::addGeometry(const Cell& c) {
  this->data_->addGeometry(c.get());
}

void Cells::addRing(size_t i, const LinearRing& ring) {
  if (i >= static_cast<size_t>(this->data_->getNumGeometries())) {
    throw std::out_of_range(
        "Cells does not contain cell at " + std::to_string(i));
  }
  downCast<OGRPolygon*>(this->data_->getGeometryRef(i))->addRing(
      ring.clone().get());
}

size_t Cells::size() const {
  return isEmpty() ? 0 : this->data_->getNumGeometries();
}

bool Cells::isConvex() const {
  return (this->size() != 1) ? false :
    getCell(0).isConvex();
}

Cell Cells::convexHull() const {
  return destroyResGeom<Cell>(this->data_->ConvexHull());
}

Cells Cells::intersection(const Cell& cell, const Cell& c) {
  return destroyResGeom<Cells>(cell->Intersection(c.get()));
}

Cells Cells::intersection(const Cell& c) const {
  return destroyResGeom<Cells>(c->Intersection(this->get()));
}

Cells Cells::intersection(const Cells& c) const {
  return destroyResGeom<Cells>(this->data_->Intersection(c.get()));
}

Cells Cells::difference(const Cells& c) const {
  return destroyResGeom<Cells>(this->data_->Difference(c.get()));
}

Cells Cells::difference(const Cell& c) const {
  return destroyResGeom<Cells>(this->data_->Difference(c.get()));
}

Cells Cells::unionOp(const Cells& c) const {
  return destroyResGeom<Cells>(this->data_->Union(c.get()));
}

Cells Cells::unionOp(const Cell& c) const {
  return destroyResGeom<Cells>(this->data_->Union(c.get()));
}

Cells Cells::unionCascaded() const {
  return destroyResGeom<Cells>(this->data_->UnionCascaded());
}

Cells Cells::splitByLine(const LineString& line) const {
  Cells cells = this->difference(this->buffer(line, 1e-8));
  for (auto&& c : cells) {
    c = Cell::buffer(c, 1e-8 * 0.5);
  }
  return cells;
}

Cells Cells::splitByLine(const MultiLineString& lines) const {
  Cells cells{*this};
  for (auto&& line : lines) {
    cells = cells.splitByLine(line);
  }
  return cells;
}

LineString Cells::createSemiLongLine(const Point& point, double angle) const {
  return LineString({point,
    point.getPointFromAngle(angle, this->getMinSafeLength())});
}

LineString Cells::createStraightLongLine(
    const Point& point, double angle) const {
  return LineString({
    point.getPointFromAngle(M_PI + angle, this->getMinSafeLength()),
    point.getPointFromAngle(angle,        this->getMinSafeLength())});
}

MultiLineString Cells::getLinesInside(const LineString& line) const {
  return MultiLineString::intersection(line, *this);
}

MultiLineString Cells::getLinesInside(const MultiLineString& lines) const {
  return lines.intersection(*this);
}

Cells Cells::getCellsInside(const Cells& cell) const {
  return this->intersection(cell);
}

bool Cells::isPointInBorder(const Point& p) const {
  return p.touches(*this);
}

bool Cells::isPointIn(const Point& p) const {
  return p.within(*this);
}

const Cell Cells::getCellWherePoint(const Point& p) const {
  for (auto&& cell : *this) {
    if (p.touches(cell) || p.within(cell)) {
      return cell;
    }
  }
  return Cell();
}

const Cell Cells::getCellWherePoint(const Point& p, double d_tol) const {
  for (auto&& cell : *this) {
    if (p.distance(cell) <= d_tol) {
      return cell;
    }
  }
  return Cell();
}

LineString Cells::createLineUntilBorder(
    const f2c::types::Point& p, double ang) const {
  return this->getCellWherePoint(p).createLineUntilBorder(p, ang);
}

Cells Cells::buffer(double width) const {
  return destroyResGeom<Cells>(this->OGRBuffer(width));
}

Cells Cells::carveSharedBorders(double width) const {
  constexpr double kTol = 1e-3;
  constexpr double kSpur = 1e-9;
  constexpr double kSameSize = 1e-9;
  // Cut along the part of each border edge that a neighbour actually touches.
  // The whole corridor comes out of the cell with the shorter border, so the
  // larger neighbour keeps its shape: taking half from each side notches both,
  // and a notched cell costs the swath generator a pass.
  Cells carved;
  for (size_t i = 0; i < this->size(); ++i) {
    Cells cell {this->getGeometry(i)};
    const LinearRing ring = this->getCellBorder(i);
    const double perimeter = ring.length();
    for (size_t k = 0; k < this->size(); ++k) {
      if (k == i) {
        continue;
      }
      // Cells that are the same size share the corridor, each giving half.
      // The comparison has to be loose: perimeters that are equal in theory
      // differ in the last bits, and letting that noise pick a winner leaves
      // some borders with a full corridor and others with none.
      const double perimeter_k = this->getCellBorder(k).length();
      const bool same_size =
          std::abs(perimeter - perimeter_k) <= kSameSize * perimeter;
      if (!same_size && perimeter > perimeter_k) {
        continue;  // the smaller neighbour gives up this corridor
      }
      const double share = same_size ? 0.5 : 1.0;
      const Cells neighbour = Cells::buffer(this->getGeometry(k), kTol);
      for (size_t e = 0; e + 1 < ring.size(); ++e) {
        MultiLineString edge;
        edge.addGeometry(
            LineString({ring.getGeometry(e), ring.getGeometry(e + 1)}));
        const MultiLineString shared = edge.intersection(neighbour);
        for (size_t j = 0; j < shared.size(); ++j) {
          const LineString part = shared.getGeometry(j);
          if (part.size() > 1 && part.length() > kTol) {
            cell = cell.difference(Cells::buffer(part, width * share));
          }
        }
      }
    }
    // The difference can leave a zero-width spur behind. Opening the result by
    // a hair drops it without moving any real edge.
    const Cells clean = cell.buffer(-kSpur).buffer(kSpur);
    for (size_t j = 0; j < clean.size(); ++j) {
      carved.addGeometry(clean.getGeometry(j));
    }
  }
  return carved;
}

Point Cells::closestPointOnBorderTo(const Point& p) const {
  std::vector<double> dist;
  std::vector<Point> ps;
  for (auto&& cell : *this) {
    ps.emplace_back(cell.closestPointOnBorderTo(p));
    dist.emplace_back(ps.back().distance(p));
  }
  return ps[std::min_element(dist.begin(), dist.end()) - dist.begin()];
}


}  // namespace f2c::types

