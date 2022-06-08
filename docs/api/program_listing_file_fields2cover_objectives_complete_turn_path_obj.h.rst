
.. _program_listing_file_fields2cover_objectives_complete_turn_path_obj.h:

Program Listing for File complete_turn_path_obj.h
=================================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_objectives_complete_turn_path_obj.h>` (``fields2cover/objectives/complete_turn_path_obj.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_OBJECTIVES_COMPLETE_TURN_PATH_OBJ_H_
   #define FIELDS2COVER_OBJECTIVES_COMPLETE_TURN_PATH_OBJ_H_
   
   #include "fields2cover/types.h"
   #include "fields2cover/objectives/path_objective.h"
   #include "fields2cover/objectives/direct_dist_path_obj.h"
   #include "fields2cover/path_planning/turning_base.h"
   
   namespace f2c {
   namespace obj {
   
   template <class T, class R = DirectDistPathObj>
   class CompleteTurnPathObj : public PathObjective {
     static_assert(std::is_base_of<pp::TurningBase, T>::value,
         "T must derive from f2c::pp::TurningBase");
   
    public:
     using PathObjective::PathObjective;
     explicit CompleteTurnPathObj(const F2CRobot& _params);
   
    public:
     using PathObjective::computeCost;
   
     double computeCost(
         const F2CPoint& p1, double ang1,
         const F2CPoint& p2, double ang2) override;
   
    public:
     T turn_planner_;
   
    private:
     R objective_;
   };
   
   
   template <class T, class R>
   CompleteTurnPathObj<T, R>::CompleteTurnPathObj(const F2CRobot& _robot) {
     turn_planner_.setRobotParams(_robot);
   }
   
   
   template <class T, class R>
   double CompleteTurnPathObj<T, R>::computeCost(
       const F2CPoint& p1, double ang1,
       const F2CPoint& p2, double ang2) {
     return objective_.computeCost(turn_planner_.createTurn(p1, ang1, p2, ang2));
   }
   
   
   }  // namespace obj
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_OBJECTIVES_COMPLETE_TURN_PATH_OBJ_H_
