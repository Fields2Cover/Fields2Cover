
.. _program_listing_file_fields2cover_path_planning_dubins_curves_cc.h:

Program Listing for File dubins_curves_cc.h
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_path_planning_dubins_curves_cc.h>` (``fields2cover/path_planning/dubins_curves_cc.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_PATH_PLANNING_DUBINS_CURVES_CC_H_
   #define FIELDS2COVER_PATH_PLANNING_DUBINS_CURVES_CC_H_
   
   #include "fields2cover/types.h"
   #include "fields2cover/path_planning/turning_base.h"
   
   namespace f2c {
   namespace pp {
   
   class DubinsCurvesCC : public TurningBase {
    public:
     explicit DubinsCurvesCC(const F2CRobot& _params);
     ~DubinsCurvesCC() = default;
     DubinsCurvesCC(DubinsCurvesCC &&) = default;
     DubinsCurvesCC() = default;
     DubinsCurvesCC(const DubinsCurvesCC&) = default;
     DubinsCurvesCC &operator=(const DubinsCurvesCC&) = default;
     DubinsCurvesCC &operator=(DubinsCurvesCC&&) = default;
   
     F2CPath createSimpleTurn(double _dist_start_pos, double _start_angle,
         double _end_angle) override;
   
    public:
     double discretization_ {0.01};
   };
   
   }  // namespace pp
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_PATH_PLANNING_DUBINS_CURVES_CC_H_
