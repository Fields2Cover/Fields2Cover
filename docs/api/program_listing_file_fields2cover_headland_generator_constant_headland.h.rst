
.. _program_listing_file_fields2cover_headland_generator_constant_headland.h:

Program Listing for File constant_headland.h
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_headland_generator_constant_headland.h>` (``fields2cover/headland_generator/constant_headland.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_HEADLAND_GENERATOR_CONSTANT_HEADLAND_H_
   #define FIELDS2COVER_HEADLAND_GENERATOR_CONSTANT_HEADLAND_H_
   
   #include "fields2cover/types.h"
   #include "fields2cover/objectives/n_swath.h"
   #include "fields2cover/headland_generator/headland_generator_base.h"
   
   namespace f2c {
   namespace hg {
   
   template <typename T = f2c::obj::NSwath>
   class ConstHL : public HeadlandGeneratorBase<T> {
    public:
     F2CCells generateHeadlands(const F2CCells& _field,
         double _dist_headland) override;
   };
   
   }  // namespace hg
   }  // namespace f2c
   
   
   #include "fields2cover/headland_generator/constant_headland_impl.hpp"
   
   
   #endif  // FIELDS2COVER_HEADLAND_GENERATOR_CONSTANT_HEADLAND_H_
