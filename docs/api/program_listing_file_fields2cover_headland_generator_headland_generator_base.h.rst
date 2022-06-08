
.. _program_listing_file_fields2cover_headland_generator_headland_generator_base.h:

Program Listing for File headland_generator_base.h
==================================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_headland_generator_headland_generator_base.h>` (``fields2cover/headland_generator/headland_generator_base.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_HEADLAND_GENERATOR_HEADLAND_GENERATOR_BASE_H_
   #define FIELDS2COVER_HEADLAND_GENERATOR_HEADLAND_GENERATOR_BASE_H_
   
   #include "fields2cover/types.h"
   #include "fields2cover/objectives/optimization_class.h"
   
   namespace f2c {
   namespace hg {
   
   template <typename T>
   class HeadlandGeneratorBase : public obj::OptimizationClass<T> {
    public:
     virtual ~HeadlandGeneratorBase() = default;
   
    public:
     virtual F2CCells generateHeadlands(const F2CCells& _field,
         double _dist_headland) = 0;
   };
   
   }  // namespace hg
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_HEADLAND_GENERATOR_HEADLAND_GENERATOR_BASE_H_
