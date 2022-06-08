
.. _program_listing_file_fields2cover_path_planning_turning_base.h:

Program Listing for File turning_base.h
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_path_planning_turning_base.h>` (``fields2cover/path_planning/turning_base.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_PATH_PLANNING_TURNING_BASE_H_
   #define FIELDS2COVER_PATH_PLANNING_TURNING_BASE_H_
   
   #include <iostream>
   #include <memory>
   #include <map>
   #include <vector>
   #include <limits>
   #include <functional>
   #include "fields2cover/types.h"
   #include "fields2cover/objectives/optimization_class.h"
   #include "fields2cover/utils/random.h"
   
   namespace f2c {
   namespace pp {
   
   class TurningBase {
    public:
     virtual ~TurningBase() = default;
   
   
    public:
     void setRobotParams(const F2CRobot& _params);
   
     F2CPath createTurn(const F2CPoint& _start_pos, double _start_angle,
         const F2CPoint& _end_pos, double _end_angle);
   
     F2CPath createTurnIfNotCached(double _dist_start_pos,
         double _start_angle, double _end_angle);
   
     virtual F2CPath createSimpleTurn(double _dist_start_pos, double _start_angle,
         double _end_angle) = 0;
   
     static std::vector<double> transformToNormalTurn(const F2CPoint& _start_pos,
         double _start_angle, const F2CPoint& _end_pos, double _end_angle);
   
     static bool isTurnValid(const F2CPath& _path, double _dist_start_end,
         double _end_angle, double _max_dist_error = 0.05,
         double _max_rot_error = 0.1);
   
   
    private:
     static void correctPath(F2CPath& _path,
         const F2CPoint& _start_pos,
         const F2CPoint& _end_pos,
         float _max_error_dist = 0.05);
   
    public:
     F2CRobot robot;
     double max_headland_width {std::numeric_limits<double>::max()};
     bool using_cache {true};
   
    protected:
     // To prevent memory consumption and comparative errors because of doubles
     // ints are used multiplied by 1000.
     std::map<std::vector<int>, F2CPath> path_cache_;
     Random rand_;
   };
   
   }  // namespace pp
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_PATH_PLANNING_TURNING_BASE_H_
