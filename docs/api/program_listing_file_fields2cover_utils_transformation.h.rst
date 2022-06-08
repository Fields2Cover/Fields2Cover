
.. _program_listing_file_fields2cover_utils_transformation.h:

Program Listing for File transformation.h
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_utils_transformation.h>` (``fields2cover/utils/transformation.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_UTILS_TRANSFORMATION_H_
   #define FIELDS2COVER_UTILS_TRANSFORMATION_H_
   
   #include <gdal/ogr_spatialref.h>
   #include <memory>
   #include <utility>
   #include <string>
   #include "fields2cover/types.h"
   
   namespace f2c {
   
   class Transform {
    public:
     static std::unique_ptr<OGRCoordinateTransformation,
       void(*)(OGRCoordinateTransformation*)>
         generateCoordTransf(
             const std::string& _coord_sys_from, const std::string& _coord_sys_to);
   
     F2CPath transform(const F2CPath& _path, const F2CField& _field,
         const std::string& _coord_sys_to);
   
     static void transform(F2CField& _field, const std::string& _coord_sys_to);
   
     template <class T>
     static T transform(const T& _t, const F2CPoint& _ref_point,
         const std::string& _coord_sys_from, const std::string& _coord_sys_to);
   
     static F2CPoint getRefPointInGPS(const F2CField& _field);
   
     static std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)>
           createSptRef(const std::string& _coord_sys);
   
     static std::unique_ptr<OGRCoordinateTransformation,
       void(*)(OGRCoordinateTransformation*)> createCoordTransf(
       std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)> _in,
       std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)> _out);
   };
   
   template <class T>
   T Transform::transform(const T& _t, const F2CPoint& _ref_point,
       const std::string& _coord_sys_from, const std::string& _coord_sys_to) {
     T new_t = _t + _ref_point;
     new_t->transform(generateCoordTransf(_coord_sys_from, _coord_sys_to).get());
     return new_t;
   }
   
   
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_UTILS_TRANSFORMATION_H_
