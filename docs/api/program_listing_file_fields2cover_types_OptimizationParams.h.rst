
.. _program_listing_file_fields2cover_types_OptimizationParams.h:

Program Listing for File OptimizationParams.h
=============================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_types_OptimizationParams.h>` (``fields2cover/types/OptimizationParams.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                           BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_TYPES_OPTIMIZATIONPARAMS_H_
   #define FIELDS2COVER_TYPES_OPTIMIZATIONPARAMS_H_
   
   namespace f2c {
   namespace types {
   
   struct OptimizationParams {
    public:
     double best_angle;
     double headland_width;
     uint16_t max_turns;
     double cost_if_no_crop;
     double greening_subsidy;
     double increment;
     double penalty_extra_swath;
   };
   }  // namespace types
   }  // namespace f2c
   
   #endif  //  FIELDS2COVER_TYPES_OPTIMIZATIONPARAMS_H_
