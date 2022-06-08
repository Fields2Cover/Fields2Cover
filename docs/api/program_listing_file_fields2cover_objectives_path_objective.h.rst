
.. _program_listing_file_fields2cover_objectives_path_objective.h:

Program Listing for File path_objective.h
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_objectives_path_objective.h>` (``fields2cover/objectives/path_objective.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_OBJECTIVES_PATH_OBJECTIVE_H_
   #define FIELDS2COVER_OBJECTIVES_PATH_OBJECTIVE_H_
   
   #include <vector>
   #include "fields2cover/types.h"
   
   namespace f2c {
   namespace obj {
   
   
   class PathObjective {
    public:
     virtual ~PathObjective() = default;
     PathObjective() = default;
     PathObjective(const PathObjective& copyFrom) = default;
     PathObjective& operator=(const PathObjective& copyFrom) = default;
     PathObjective(PathObjective &&) = default;
     PathObjective& operator=(PathObjective &&) = default;
   
    public:
     template <typename... Ts>
     double computeCostWithMinimizingSign(const Ts... args) {
       return (isMinimizing() ? 1.0 : -1.0) * computeCost(args...);
     }
   
    public:
     virtual bool isMinimizing(void) const { return true;}
     virtual bool isMaximizing(void) const { return !isMinimizing();}
   
   
    public:
     virtual double computeCost(const F2CPoint& p1, const F2CPoint& p2);
   
     virtual double computeCost(
         const F2CPoint& p1, double ang1, const F2CPoint& p2);
   
     virtual double computeCost(
         const F2CPoint& p1, double ang1, const F2CPoint& p2, double ang2) = 0;
   
     virtual double computeCost(
         const F2CPoint& p1, const F2CPoint& p2, double ang2);
   
     virtual double computeCost(const F2CSwath& s, const F2CPoint& p);
   
     virtual double computeCost(const F2CSwath& s1, const F2CSwath& s2);
   
     virtual double computeCost(const F2CSwath& s, const F2CPoint& p, double ang);
   
     virtual double computeCost(const F2CPoint& p, const F2CSwath& s);
   
     virtual double computeCost(const F2CPoint& p, double ang, const F2CSwath& s);
   
     virtual double computeCost(const std::vector<F2CPoint>& ps);
   
     virtual double computeCost(const F2CMultiPoint& ps);
   
     virtual double computeCost(const F2CSwath& s, const F2CMultiPoint& ps);
   
     virtual double computeCost(const F2CSwaths& s, const F2CMultiPoint& ps);
   
     virtual double computeCost(const F2CMultiPoint& ps, const F2CSwath& s);
   
     virtual double computeCost(const F2CMultiPoint& ps, const F2CSwaths& s);
   
     virtual double computeCost(const F2CSwath& s);
   
     virtual double computeCost(const F2CSwaths& swaths);
   
     virtual double computeCost(const F2CRoute& r);
   
     virtual double computeCost(const F2CPath& p);
   };
   
   
   }  // namespace obj
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_OBJECTIVES_PATH_OBJECTIVE_H_
