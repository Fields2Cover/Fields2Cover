
.. _program_listing_file_fields2cover_objectives_swath_length.h:

Program Listing for File swath_length.h
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_objectives_swath_length.h>` (``fields2cover/objectives/swath_length.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_OBJECTIVES_SWATH_LENGTH_H_
   #define FIELDS2COVER_OBJECTIVES_SWATH_LENGTH_H_
   
   #include <numeric>
   #include "fields2cover/types.h"
   #include "fields2cover/objectives/global_objective.h"
   
   namespace f2c {
   namespace obj {
   
   class SwathLength : public GlobalObjective {
    public:
     using GlobalObjective::computeCost;
     double computeCost(const F2CSwath& s) const override;
     double computeCost(const F2CSwaths& swaths) const override;
   };
   
   }  // namespace obj
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_OBJECTIVES_SWATH_LENGTH_H_
