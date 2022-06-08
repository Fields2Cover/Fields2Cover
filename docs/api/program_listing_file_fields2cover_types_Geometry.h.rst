
.. _program_listing_file_fields2cover_types_Geometry.h:

Program Listing for File Geometry.h
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_types_Geometry.h>` (``fields2cover/types/Geometry.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                           BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_TYPES_GEOMETRY_H_
   #define FIELDS2COVER_TYPES_GEOMETRY_H_
   
   #include <gdal/ogr_geometry.h>
   #include <gdal/ogr_core.h>
   #include <functional>
   #include <memory>
   #include <vector>
   #include <string>
   #include <boost/math/constants/constants.hpp>
   
   namespace f2c {
   namespace types {
   
   class EmptyDestructor {};
   
   template <class T, OGRwkbGeometryType R>
   struct Geometry {
    public:
     Geometry();
     explicit Geometry(const T& g);
     explicit Geometry(std::shared_ptr<T> g);
     explicit Geometry(T* g, EmptyDestructor);
     explicit Geometry(const T* g);
     ~Geometry();
     explicit Geometry(const Geometry& g);
     Geometry(Geometry&& g);
     Geometry& operator=(Geometry&& g);
     Geometry& operator=(const Geometry& g);
   
     std::shared_ptr<T> operator->();
     std::shared_ptr<T> operator->() const;
     T* get() const;
   
     bool operator !=(const Geometry<T, R>& geom2) const;
     bool operator ==(const Geometry<T, R>& geom2) const;
   
     double getDimMinX() const;
   
     double getDimMaxX() const;
   
     double getDimMinY() const;
   
     double getDimMaxY() const;
   
     double getHeight() const;
   
     double getWidth() const;
   
     double getMinSafeLength() const;
   
     template <class T2, OGRwkbGeometryType R2>
     double Distance(const Geometry<T2, R2>& p) const;
   
     template <class T2, OGRwkbGeometryType R2>
     bool Disjoint(const Geometry<T2, R2>& _geom) const;
   
     bool Disjoint(const OGRGeometry* _geom) const;
   
     template <class T2, OGRwkbGeometryType R2>
     bool Crosses(const Geometry<T2, R2>& _geom) const;
   
     bool Crosses(const OGRGeometry* _geom) const;
   
     template <class T2, OGRwkbGeometryType R2>
     bool Touches(const Geometry<T2, R2>& _geom) const;
   
     bool Touches(const OGRGeometry* _geom) const;
   
     template <class T2, OGRwkbGeometryType R2>
     bool Within(const Geometry<T2, R2>& _geom) const;
   
     bool Within(const OGRGeometry* _geom) const;
   
     template <class T2, OGRwkbGeometryType R2>
     bool Intersects(const Geometry<T2, R2>& _geom) const;
   
     bool Intersects(const OGRGeometry* _geom) const;
   
     static double mod_2pi(double val);
   
     static double getAngContinuity(double prev_val, double val);
     static std::vector<double> getAngContinuity(const std::vector<double>& val);
   
     static double getAngleDiffAbs(double a, double b);
   
     bool isEmpty() const;
   
     std::string exportToWkt() const;
     void importFromWkt(const std::string& text);
     std::string exportToGML() const;
     std::string exportToKML() const;
     std::string exportToJson() const;
   
    protected:
     std::shared_ptr<T> data;
   };
   
   }  // namespace types
   }  // namespace f2c
   
   #include "fields2cover/types/Geometry_impl.hpp"
   
   namespace f2c {
   namespace types {
   template struct Geometry<OGRPoint, wkbPoint>;
   template struct Geometry<OGRMultiPoint, wkbMultiPoint>;
   template struct Geometry<OGRLinearRing, wkbLinearRing>;
   template struct Geometry<OGRLineString, wkbLineString>;
   template struct Geometry<OGRMultiLineString, wkbMultiLineString>;
   template struct Geometry<OGRPolygon, wkbPolygon>;
   template struct Geometry<OGRMultiPolygon, wkbMultiPolygon>;
   }  // namespace types
   }  // namespace f2c
   
   
   #endif  // FIELDS2COVER_TYPES_GEOMETRY_H_
