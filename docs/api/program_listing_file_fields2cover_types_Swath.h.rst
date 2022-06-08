
.. _program_listing_file_fields2cover_types_Swath.h:

Program Listing for File Swath.h
================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_types_Swath.h>` (``fields2cover/types/Swath.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                           BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_TYPES_SWATH_H_
   #define FIELDS2COVER_TYPES_SWATH_H_
   
   #include <gdal/ogr_geometry.h>
   
   #include <algorithm>
   #include <memory>
   #include <utility>
   #include "fields2cover/types/LineString.h"
   #include "fields2cover/types/Cells.h"
   
   namespace f2c {
   namespace types {
   
   struct Swath {
    public:
     Swath();
     explicit Swath(const LineString& _path);
     explicit Swath(double _width);
     Swath(const LineString& _path, double _width, int _id = 0);
     Swath(const Swath&);
     virtual ~Swath();
     virtual Swath& operator=(Swath&&);
     virtual Swath& operator=(const Swath&);
     bool operator!=(const Swath& s) const;
     bool operator==(const Swath& s) const;
     bool operator>=(const Swath& s) const;
     bool operator<=(const Swath& s) const;
     bool operator>(const Swath& s) const;
     bool operator<(const Swath& s) const;
   
    public:
     void setId(int _id);
     int getId() const;
   
     void setPath(const LineString& _path);
     LineString getPath() const;
   
     void setWidth(double _width);
     double getWidth() const;
   
     bool getCreationDir() const;
     void setCreationDir(bool _creation_dir);
   
    public:
     double getLength(void) const;
   
     double getArea(void) const;
     double getArea(const Cells& _polys) const;
   
     void reverse(void);
   
     double getInAngle(void) const;
   
     double getOutAngle(void) const;
   
     Point startPoint(void) const;
   
     Point endPoint(void) const;
   
     Cells computeAreaCovered(void) const;
     Cells computeAreaCovered(const Cells& _polys) const;
     Swath clone() const;
   
     size_t getNumPoints() const;
   
     Point getPoint(int i);
     const Point getPoint(int i) const;
   
   
     bool hasSameDir(const Swath& s) const;
   
     void targetSameDirAs(const Swath& s);
   
     void targetOppositeDirAs(const Swath& s);
   
    private:
     int id_;
     LineString path_;
     double width_ {0.0};
     bool creation_dir_ {true};
   };
   
   
   }  // namespace types
   }  // namespace f2c
   
   #endif  //  FIELDS2COVER_TYPES_SWATH_H_
