
.. _program_listing_file_fields2cover_objectives_global_objective.h:

Program Listing for File global_objective.h
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_objectives_global_objective.h>` (``fields2cover/objectives/global_objective.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_OBJECTIVES_GLOBAL_OBJECTIVE_H_
   #define FIELDS2COVER_OBJECTIVES_GLOBAL_OBJECTIVE_H_
   
   #include "fields2cover/types.h"
   
   namespace f2c {
   namespace obj {
   
   class GlobalObjective {
    public:
     virtual ~GlobalObjective() = default;
     GlobalObjective() = default;
     GlobalObjective(const GlobalObjective& copyFrom) = default;
     GlobalObjective& operator=(const GlobalObjective& copyFrom) = default;
     GlobalObjective(GlobalObjective &&) = default;
     GlobalObjective& operator=(GlobalObjective &&) = default;
   
    public:
     template <typename... Ts>
     double computeCostWithMinimizingSign(const Ts... args) const {
       return (isMinimizing() ? 1.0 : -1.0) * computeCost(args...);
     }
   
    public:
     virtual bool isMinimizing(void) const;
     bool isMaximizing(void) const;
   
   
    public:
     virtual double computeCost(const F2CSwath&) const;
     virtual double computeCost(const F2CSwaths&) const;
     virtual double computeCost(const F2CSwathsByCells& swaths) const;
     virtual double computeCost(const F2CCells&) const;
     virtual double computeCost(const F2CCell&) const;
     virtual double computeCost(const F2CRoute&) const;
     virtual double computeCost(const F2CPath&) const;
     virtual double computeCost(const F2CCell& c, const F2CSwath& s) const;
     virtual double computeCost(const F2CCell& c, const F2CSwaths& s) const;
     virtual double computeCost(const F2CCell& c,
         const F2CSwathsByCells& swaths) const;
     virtual double computeCost(const F2CCells& c, const F2CSwath& s) const;
     virtual double computeCost(const F2CCells& c, const F2CSwaths& s) const;
     virtual double computeCost(const F2CCells& c,
         const F2CSwathsByCells& swaths) const;
     virtual double computeCost(const F2CRoute& r, const F2CPath& p) const;
     virtual double computeCost(const F2CCell& c, const F2CSwath& s,
         const F2CRoute& r) const;
     virtual double computeCost(const F2CCell& c, const F2CSwath& s,
         const F2CPath& p) const;
     virtual double computeCost(const F2CCell& c, const F2CSwath& s,
         const F2CRoute& r, const F2CPath& p) const;
     virtual double computeCost(const F2CCell& c, const F2CSwaths& s,
         const F2CRoute& r) const;
     virtual double computeCost(const F2CCell& c, const F2CSwaths& s,
         const F2CPath& p) const;
     virtual double computeCost(const F2CCell& c, const F2CSwaths& s,
         const F2CRoute& r, const F2CPath& p) const;
     virtual double computeCost(const F2CCell& c, const F2CSwathsByCells& s,
         const F2CRoute& r) const;
     virtual double computeCost(const F2CCell& c, const F2CSwathsByCells& s,
         const F2CPath& p) const;
     virtual double computeCost(const F2CCell& c, const F2CSwathsByCells& s,
         const F2CRoute& r, const F2CPath& p) const;
     virtual double computeCost(const F2CCells& c, const F2CSwath& s,
         const F2CRoute& r) const;
     virtual double computeCost(const F2CCells& c, const F2CSwath& s,
         const F2CPath& p) const;
     virtual double computeCost(const F2CCells& c, const F2CSwath& s,
         const F2CRoute& r, const F2CPath& p) const;
     virtual double computeCost(const F2CCells& c, const F2CSwaths& s,
         const F2CRoute& r) const;
     virtual double computeCost(const F2CCells& c, const F2CSwaths& s,
         const F2CPath& p) const;
     virtual double computeCost(const F2CCells& c, const F2CSwaths& s,
         const F2CRoute& r, const F2CPath& p) const;
     virtual double computeCost(const F2CCells& c, const F2CSwathsByCells& s,
         const F2CRoute& r) const;
     virtual double computeCost(const F2CCells& c, const F2CSwathsByCells& s,
         const F2CPath& p) const;
     virtual double computeCost(const F2CCells& c, const F2CSwathsByCells& s,
         const F2CRoute& r, const F2CPath& p) const;
   };
   
   
   }  // namespace obj
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_OBJECTIVES_GLOBAL_OBJECTIVE_H_
