
.. _program_listing_file_fields2cover_path_planning_reeds_shepp_curves_hc.h:

Program Listing for File reeds_shepp_curves_hc.h
================================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_path_planning_reeds_shepp_curves_hc.h>` (``fields2cover/path_planning/reeds_shepp_curves_hc.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_PATH_PLANNING_REEDS_SHEPP_CURVES_HC_H_
   #define FIELDS2COVER_PATH_PLANNING_REEDS_SHEPP_CURVES_HC_H_
   
   #include "fields2cover/types.h"
   #include "fields2cover/path_planning/turning_base.h"
   
   namespace f2c {
   namespace pp {
   
   class ReedsSheppCurvesHC : public TurningBase {
    public:
     explicit ReedsSheppCurvesHC(const F2CRobot& _params);
     ~ReedsSheppCurvesHC() = default;
     ReedsSheppCurvesHC(ReedsSheppCurvesHC &&) = default;
     ReedsSheppCurvesHC() = default;
     ReedsSheppCurvesHC(const ReedsSheppCurvesHC&) = default;
     ReedsSheppCurvesHC &operator=(const ReedsSheppCurvesHC&) = default;
     ReedsSheppCurvesHC &operator=(ReedsSheppCurvesHC&&) = default;
   
     F2CPath createSimpleTurn(double _dist_start_pos, double _start_angle,
         double _end_angle) override;
   
    public:
     double discretization_ {0.01};
   };
   
   }  // namespace pp
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_PATH_PLANNING_REEDS_SHEPP_CURVES_HC_H_
