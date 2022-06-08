
.. _program_listing_file_fields2cover_objectives_optimization_class.h:

Program Listing for File optimization_class.h
=============================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_objectives_optimization_class.h>` (``fields2cover/objectives/optimization_class.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_OBJECTIVES_OPTIMIZATION_CLASS_H_
   #define FIELDS2COVER_OBJECTIVES_OPTIMIZATION_CLASS_H_
   
   #include <memory>
   #include <type_traits>
   #include "fields2cover/types.h"
   #include "fields2cover/objectives/global_objective.h"
   
   namespace f2c {
   namespace obj {
   
   template <typename T>
   class OptimizationClass {
     static_assert(std::is_base_of<GlobalObjective, T>::value,
         "T must derive from GlobalObjective");
   
    public:
     template <typename ...Ts>
     double computeCost(Ts... t) const {
       return objective_.computeCost(t...);
     }
   
     template <typename ...Ts>
     double computeCostWithMinimizingSign(Ts... t) const {
       return objective_.computeCostWithMinimizingSign(t...);
     }
   
    protected:
     T objective_;
   };
   
   }  // namespace obj
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_OBJECTIVES_OPTIMIZATION_CLASS_H_
