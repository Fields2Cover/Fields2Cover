
.. _program_listing_file_fields2cover_types_Cell.h:

Program Listing for File Cell.h
===============================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_types_Cell.h>` (``fields2cover/types/Cell.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                           BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_TYPES_CELL_H_
   #define FIELDS2COVER_TYPES_CELL_H_
   
   #include <gdal/ogr_geometry.h>
   #include <string>
   #include <boost/math/constants/constants.hpp>
   #include "fields2cover/types/Geometries.h"
   #include "fields2cover/types/Point.h"
   #include "fields2cover/types/LinearRing.h"
   #include "fields2cover/types/LineString.h"
   #include "fields2cover/types/MultiLineString.h"
   
   namespace f2c {
   namespace types {
   
   // Function to concat string easily with numbers
   template < typename... Args >
   inline std::string sstr(Args &&... args) {
     std::ostringstream sstr;
     (sstr << std::dec << ... << args);
     return sstr.str();
   }
   
   
   struct Cell : public Geometries<Cell, OGRPolygon, wkbPolygon, LinearRing> {
    public:
     using Geometries<Cell, OGRPolygon, wkbPolygon, LinearRing>::Geometries;
     Cell();
     explicit Cell(const OGRGeometry* _geom);
   
     explicit Cell(const f2c::types::LinearRing& ring);
   
     void getGeometry(int i, LinearRing& ring);
   
     void getGeometry(int i, LinearRing& ring) const;
   
     LinearRing getGeometry(int i);
   
     LinearRing getGeometry(int i) const;
   
     void setGeometry(int i, const LinearRing& ring);
   
     size_t size() const;
   
   
     void operator*=(double b);
   
     static Cell Buffer(const Cell& _geom, double width);
   
     static Cell Buffer(const LineString& _geom, double width);
   
     static Cell Buffer(const Point& _geom, double width);
   
     Cell Intersection(const Cell& _c) const;
   
     void addRing(const LinearRing& _ring);
     void addGeometry(const LinearRing& _ring);
   
     LinearRing getExteriorRing() const;
     LinearRing getInteriorRing(int i_ring) const;
   
     bool isConvex() const;
   
     LineString getSemiLongCurve(const Point& _point, double _angle) const;
   
     LineString getStraightLongCurve(const Point& _point, double _angle) const;
   
     MultiLineString getLinesInside(const LineString& _line) const;
   
     MultiLineString getLinesInside(const MultiLineString& _lines) const;
   
     bool isPointInBorder(const Point& _p) const;
   
     bool isPointIn(const Point& _p) const;
   
     LineString createLineUntilBorder(const f2c::types::Point& _p, double _ang);
   };
   
   }  // namespace types
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_TYPES_CELL_H_
