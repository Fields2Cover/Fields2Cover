
.. _program_listing_file_fields2cover_objectives_direct_dist_path_obj.h:

Program Listing for File direct_dist_path_obj.h
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_objectives_direct_dist_path_obj.h>` (``fields2cover/objectives/direct_dist_path_obj.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_OBJECTIVES_DIRECT_DIST_PATH_OBJ_H_
   #define FIELDS2COVER_OBJECTIVES_DIRECT_DIST_PATH_OBJ_H_
   
   #include "fields2cover/types.h"
   #include "fields2cover/objectives/path_objective.h"
   
   namespace f2c {
   namespace obj {
   
   class DirectDistPathObj : public PathObjective {
    public:
     using PathObjective::computeCost;
   
     double computeCost(const F2CPoint& p1, double ang1,
         const F2CPoint& p2, double ang2) override;
   };
   
   
   }  // namespace obj
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_OBJECTIVES_DIRECT_DIST_PATH_OBJ_H_
