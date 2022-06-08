
.. _program_listing_file_fields2cover_path_planning_reeds_shepp_curves.h:

Program Listing for File reeds_shepp_curves.h
=============================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_path_planning_reeds_shepp_curves.h>` (``fields2cover/path_planning/reeds_shepp_curves.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_PATH_PLANNING_REEDS_SHEPP_CURVES_H_
   #define FIELDS2COVER_PATH_PLANNING_REEDS_SHEPP_CURVES_H_
   
   #include "fields2cover/types.h"
   #include "fields2cover/path_planning/turning_base.h"
   
   namespace f2c {
   namespace pp {
   
   class ReedsSheppCurves : public TurningBase {
    public:
     explicit ReedsSheppCurves(const F2CRobot& _params);
     ~ReedsSheppCurves() = default;
     ReedsSheppCurves(ReedsSheppCurves &&) = default;
     ReedsSheppCurves() = default;
     ReedsSheppCurves(const ReedsSheppCurves&) = default;
     ReedsSheppCurves &operator=(const ReedsSheppCurves&) = default;
     ReedsSheppCurves &operator=(ReedsSheppCurves&&) = default;
   
     F2CPath createSimpleTurn(double _dist_start_pos, double _start_angle,
         double _end_angle) override;
   
    public:
     double discretization_ {0.01};
   };
   
   }  // namespace pp
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_PATH_PLANNING_REEDS_SHEPP_CURVES_H_
