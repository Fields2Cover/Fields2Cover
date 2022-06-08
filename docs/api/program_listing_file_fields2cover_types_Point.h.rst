
.. _program_listing_file_fields2cover_types_Point.h:

Program Listing for File Point.h
================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_types_Point.h>` (``fields2cover/types/Point.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                           BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_TYPES_POINT_H_
   #define FIELDS2COVER_TYPES_POINT_H_
   
   #include <gdal/ogr_geometry.h>
   #include <iostream>
   #include <functional>
   #include <memory>
   #include <utility>
   #include <vector>
   #include "fields2cover/types/Geometry.h"
   
   namespace f2c {
   namespace types {
   
   struct Point : public Geometry<OGRPoint, wkbPoint> {
    public:
     using Geometry<OGRPoint, wkbPoint>::Geometry;
     Point();
     Point(double x, double y, double z = 0);
   
    public:
     bool operator==(const Point& b) const;
   
     bool operator<(const Point& b) const;
   
     Point operator+(const Point& b) const;
   
     Point operator-(const Point& b) const;
   
     void operator*=(double b);
   
     Point operator*(double b) const;
   
   
    public:
     Point clone() const;
     double getX() const;
     double getY() const;
     double getZ() const;
     void setX(double x);
     void setY(double y);
     void setZ(double z);
   
     void setPoint(double x, double y, double z = 0);
     void setPoint(const OGRPoint& p);
     void setPoint(const Point& p);
   
     double getAngleFromPoints(const Point& _end) const;
     double getAngleFromPoint() const;
     Point getPointFromAngle(double _angle, double _dist) const;
   
     Point rotateFromPoint(double _angle, const Point& _p_r) const;
   
     template <class T>
     std::vector<T> rotateFromPoint(double _angle, const std::vector<T>& _t) const;
   
     template <class T>
     T rotateFromPoint(double _angle, const T& _t) const;
   };
   
   std::ostream& operator<<(std::ostream& os, const Point& p);
   
   template <class T>
   std::vector<T> Point::rotateFromPoint(
       double _angle, const std::vector<T>& _t) const {
     std::vector<T> res;
     for (auto&& p : _t) {
       res.emplace_back(this->rotateFromPoint(_angle, p));
     }
     return res;
   }
   
   template <class T>
   T Point::rotateFromPoint(double _angle, const T& _t) const {
     T res;
     for (auto&& p : _t) {
       res.addGeometry(this->rotateFromPoint(_angle, p));
     }
     return res;
   }
   
   }  // namespace types
   }  // namespace f2c
   
   inline OGRPoint operator+(const OGRPoint& a, const OGRPoint& b) {
     return std::move(OGRPoint(
         a.getX() + b.getX(),
         a.getY() + b.getY(),
         a.getZ() + b.getZ()));
   }
   
   inline OGRPoint operator+(const OGRPoint& a, const f2c::types::Point& b) {
     return std::move(OGRPoint(
         a.getX() + b->getX(),
         a.getY() + b->getY(),
         a.getZ() + b->getZ()));
   }
   
   inline OGRPoint operator-(const OGRPoint& a, const f2c::types::Point& b) {
     return std::move(OGRPoint(
         a.getX() - b->getX(),
         a.getY() - b->getY(),
         a.getZ() - b->getZ()));
   }
   inline OGRPoint operator*(const OGRPoint& a, double b) {
     return std::move(OGRPoint(
         a.getX() * b,
         a.getY() * b,
         a.getZ() * b));
   }
   
   template <class T>
   inline std::vector<T> operator+(
       const std::vector<T>& _t, const f2c::types::Point& _dir) {
     std::vector<T> res;
     for (auto&& p : _t) {
       res.emplace_back(p + _dir);
     }
     return res;
   }
   
   template <class T>
   inline std::vector<T> operator-(
       const f2c::types::Point& _dir, const std::vector<T>& _t) {
     std::vector<T> res;
     for (auto&& p : _t) {
       res.emplace_back(_dir - p);
     }
     return res;
   }
   
   template <class T>
   inline std::vector<T> operator-(
       const std::vector<T>& _t, const f2c::types::Point& _dir) {
     std::vector<T> res;
     for (auto&& p : _t) {
       res.emplace_back(p - _dir);
     }
     return res;
   }
   
   template <class T>
   inline T operator+(const T& _t, const f2c::types::Point& _dir) {
     T res;
     for (auto&& p : _t) {
       res.addGeometry(p + _dir);
     }
     return res;
   }
   
   template <class T>
   inline T operator-(const f2c::types::Point& _dir, const T& _t) {
     T res;
     for (auto&& p : _t) {
       res.addGeometry(_dir - p);
     }
     return res;
   }
   
   template <class T>
   inline T operator-(const T& _t, const f2c::types::Point& _dir) {
     T res;
     for (auto&& p : _t) {
       res.addGeometry(p - _dir);
     }
     return res;
   }
   
   
   #endif  // FIELDS2COVER_TYPES_POINT_H_
