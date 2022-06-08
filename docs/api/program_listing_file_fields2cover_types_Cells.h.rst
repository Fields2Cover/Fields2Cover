
.. _program_listing_file_fields2cover_types_Cells.h:

Program Listing for File Cells.h
================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_types_Cells.h>` (``fields2cover/types/Cells.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                           BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_TYPES_CELLS_H_
   #define FIELDS2COVER_TYPES_CELLS_H_
   
   #include <gdal/ogr_geometry.h>
   #include "fields2cover/types/Geometry.h"
   #include "fields2cover/types/Geometries.h"
   #include "fields2cover/types/Point.h"
   #include "fields2cover/types/LineString.h"
   #include "fields2cover/types/Cell.h"
   
   namespace f2c {
   namespace types {
   
   struct Cells : public Geometries<Cells, OGRMultiPolygon, wkbMultiPolygon,
       Cell> {
    public:
     using Geometries<Cells, OGRMultiPolygon, wkbMultiPolygon, Cell>::Geometries;
     Cells();
     explicit Cells(const OGRGeometry* _geom);
   
     explicit Cells(const Cell& _c);
   
     void operator*=(double b);
   
     void addGeometry(const Cell& _c);
     void addRing(int i, const LinearRing& _ring);
   
     size_t size() const;
   
     void getGeometry(int i, Cell& cell);
     void getGeometry(int i, Cell& cell) const;
   
     Cell getGeometry(int i);
   
     const Cell getGeometry(int i) const;
   
     void setGeometry(int i, const Cell& cell);
   
     Cell getCell(int i) const;
   
     LinearRing getCellBorder(int i) const;
     LinearRing getInteriorRing(int i_cell, int i_ring) const;
   
     bool isConvex() const;
   
     Cells Intersection(const Cells& _c) const;
   
     Cells Difference(const Cells& _c) const;
   
     Cells splitByLine(const LineString& _line) const;
   
     Cells splitByLine(const MultiLineString& _lines) const;
   
     Cells Intersection(const Cell& _c) const;
   
     static Cells Intersection(
         const Cell& cell, const Cell& _c);
   
     LineString getSemiLongCurve(const Point& _point, double _angle) const;
   
     LineString getStraightLongCurve(const Point& _point, double _angle) const;
   
     MultiLineString getLinesInside(const LineString& _line) const;
   
     MultiLineString getLinesInside(const MultiLineString& _lines) const;
   
     Cells getCellsInside(const Cells& _cell) const;
   
     bool isPointInBorder(const Point& p) const;
   
     bool isPointIn(const Point& _p) const;
   
     Cell getCellWherePoint(const Point& p) const;
   
     LineString createLineUntilBorder(
         const f2c::types::Point& _p, double _ang) const;
   
     template <class T, OGRwkbGeometryType R>
     static Cells Buffer(const Geometry<T, R>& _geom, double width);
   };
   
   
   
   template <class T, OGRwkbGeometryType R>
   Cells Cells::Buffer(const Geometry<T, R>& _geom, double width) {
     auto buffer = _geom->Buffer(width);
     Cells cells {buffer};
     OGRGeometryFactory::destroyGeometry(buffer);
     return cells;
   }
   
   
   }  // namespace types
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_TYPES_CELLS_H_
