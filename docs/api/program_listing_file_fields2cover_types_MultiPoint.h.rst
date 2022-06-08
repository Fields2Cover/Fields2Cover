
.. _program_listing_file_fields2cover_types_MultiPoint.h:

Program Listing for File MultiPoint.h
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_types_MultiPoint.h>` (``fields2cover/types/MultiPoint.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                           BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_TYPES_MULTIPOINT_H_
   #define FIELDS2COVER_TYPES_MULTIPOINT_H_
   
   #include <gdal/ogr_geometry.h>
   #include "fields2cover/types/Geometries.h"
   #include "fields2cover/types/Point.h"
   
   namespace f2c {
   namespace types {
   
   struct MultiPoint :
     public Geometries<MultiPoint, OGRMultiPoint, wkbMultiPoint, Point> {
    public:
     using Geometries<MultiPoint, OGRMultiPoint, wkbMultiPoint, Point>::Geometries;
     MultiPoint();
     MultiPoint(const std::initializer_list<Point>& ps);
   
     size_t size() const;
   
     void getGeometry(int i, Point& point);
   
     void getGeometry(int i, Point& point) const;
   
     Point getGeometry(int i);
   
     const Point getGeometry(int i) const;
   
     void setGeometry(int i, const Point& p);
   
     void addGeometry(const Point& _p);
     void addPoint(const Point& _p);
     void addPoint(double x, double y, double z = 0);
   
     void operator*=(double b);
   };
   
   }  // namespace types
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_TYPES_MULTIPOINT_H_
