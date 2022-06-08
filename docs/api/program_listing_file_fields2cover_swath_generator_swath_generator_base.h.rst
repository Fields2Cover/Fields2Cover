
.. _program_listing_file_fields2cover_swath_generator_swath_generator_base.h:

Program Listing for File swath_generator_base.h
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_swath_generator_swath_generator_base.h>` (``fields2cover/swath_generator/swath_generator_base.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_H_
   #define FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_H_
   
   #include <memory>
   #include <vector>
   #include <cmath>
   #include <algorithm>
   #include <limits>
   #include <numeric>
   #include "fields2cover/types.h"
   #include "fields2cover/objectives/optimization_class.h"
   
   namespace f2c {
   namespace sg {
   
   template <typename T>
   class SwathGeneratorBase : public obj::OptimizationClass<T> {
    public:
     SwathGeneratorBase() = default;
     virtual ~SwathGeneratorBase() = default;
   
     virtual F2CSwaths generateBestSwaths(
         double _op_width, const F2CCell& _poly) = 0;
   
     virtual F2CSwathsByCells generateBestSwaths(
         double _op_width, const F2CCells& _polys);
   
     virtual F2CSwaths generateSwaths(double _angle,
         double _op_width, const F2CCell& _poly);
   
     virtual F2CSwathsByCells generateSwaths(double _angle,
         double _op_width, const F2CCells& _polys);
   
     double getBestAngle() const;
   
    public:
     double best_angle {-1e7};  // radians
   };
   
   }  // namespace sg
   }  // namespace f2c
   
   #include "fields2cover/swath_generator/swath_generator_base_impl.hpp"
   
   
   #endif  // FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_H_
