
.. _program_listing_file_fields2cover_path_planning_path_planning.h:

Program Listing for File path_planning.h
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_path_planning_path_planning.h>` (``fields2cover/path_planning/path_planning.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_PATH_PLANNING_PATH_PLANNING_H_
   #define FIELDS2COVER_PATH_PLANNING_PATH_PLANNING_H_
   
   #include "fields2cover/types.h"
   #include "fields2cover/objectives/path_objective.h"
   #include "fields2cover/objectives/optimization_class.h"
   #include "fields2cover/path_planning/turning_base.h"
   
   namespace f2c {
   namespace pp {
   
   class PathPlanning {
    public:
     F2CPath searchBestPath(const F2CSwaths& swaths, TurningBase& turn);
   
    public:
     double turn_point_dist {0.1};
   };
   
   }  // namespace pp
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_PATH_PLANNING_PATH_PLANNING_H_
