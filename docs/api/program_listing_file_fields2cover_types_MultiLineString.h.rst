
.. _program_listing_file_fields2cover_types_MultiLineString.h:

Program Listing for File MultiLineString.h
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_types_MultiLineString.h>` (``fields2cover/types/MultiLineString.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                           BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_TYPES_MULTILINESTRING_H_
   #define FIELDS2COVER_TYPES_MULTILINESTRING_H_
   
   #include <gdal/ogr_geometry.h>
   #include <utility>
   #include "fields2cover/types/Geometries.h"
   #include "fields2cover/types/LineString.h"
   
   namespace f2c {
   namespace types {
   
   struct MultiLineString :
       public Geometries<MultiLineString, OGRMultiLineString,
               wkbMultiLineString, LineString> {
    public:
     using Geometries<MultiLineString, OGRMultiLineString,
               wkbMultiLineString, LineString>::Geometries;
     MultiLineString();
     explicit MultiLineString(const OGRGeometry* _geom);
     explicit MultiLineString(const LineString& _line);
   
     size_t size() const;
   
     double getLength() const;
   
     void operator*=(double b);
   
     void getGeometry(int i, LineString& line);
   
     void getGeometry(int i, LineString& line) const;
   
     LineString getGeometry(int i);
   
     const LineString getGeometry(int i) const;
   
     void setGeometry(int i, const LineString& line);
   
     void append(const OGRGeometry* _geom);
   
     void addGeometry(const LineString& _line);
   
     static MultiLineString getLineSegments(const LineString& _line);
     static MultiLineString getLineSegments(const LinearRing& _line);
   
     template <class T, OGRwkbGeometryType R>
     MultiLineString Intersection(const Geometry<T, R>& _g) const;
   
     template <class T, OGRwkbGeometryType R>
     static MultiLineString Intersection(
         const LineString& line, const Geometry<T, R>& _g);
   };
   
   
   template <class T, OGRwkbGeometryType R>
   MultiLineString MultiLineString::Intersection(const Geometry<T, R>& _g) const {
     auto inter = data->Intersection(_g.get());
     f2c::types::MultiLineString lines(inter);
     OGRGeometryFactory::destroyGeometry(inter);
     return lines;
   }
   
   
   template <class T, OGRwkbGeometryType R>
   MultiLineString MultiLineString::Intersection(
       const LineString& line, const Geometry<T, R>& _g) {
     return std::move(MultiLineString(line.get()).Intersection(_g));
   }
   
   
   }  // namespace types
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_TYPES_MULTILINESTRING_H_
