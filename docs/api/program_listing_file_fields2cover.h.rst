
.. _program_listing_file_fields2cover.h:

Program Listing for File fields2cover.h
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover.h>` (``fields2cover.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_H_
   #define FIELDS2COVER_H_
   
   #include "fields2cover/types.h"
   
   
   #include "fields2cover/utils/random.h"
   #include "fields2cover/utils/transformation.h"
   #include "fields2cover/utils/parser.h"
   #include "fields2cover/utils/visualizer.h"
   
   #include "fields2cover/objectives/path_objective.h"
   #include "fields2cover/objectives/direct_dist_path_obj.h"
   #include "fields2cover/objectives/complete_turn_path_obj.h"
   
   #include "fields2cover/objectives/global_objective.h"
   #include "fields2cover/objectives/optimization_class.h"
   #include "fields2cover/objectives/n_swath.h"
   #include "fields2cover/objectives/field_coverage.h"
   #include "fields2cover/objectives/overlaps.h"
   #include "fields2cover/objectives/swath_length.h"
   
   
   #include "fields2cover/headland_generator/headland_generator_base.h"
   #include "fields2cover/headland_generator/constant_headland.h"
   
   #include "fields2cover/swath_generator/swath_generator_base.h"
   #include "fields2cover/swath_generator/brute_force.h"
   
   #include "fields2cover/route_planning/single_cell_swaths_order_base.h"
   #include "fields2cover/route_planning/boustrophedon_order.h"
   #include "fields2cover/route_planning/snake_order.h"
   
   #include "fields2cover/path_planning/turning_base.h"
   #include "fields2cover/path_planning/dubins_curves.h"
   #include "fields2cover/path_planning/dubins_curves_cc.h"
   #include "fields2cover/path_planning/reeds_shepp_curves.h"
   #include "fields2cover/path_planning/reeds_shepp_curves_hc.h"
   #include "fields2cover/path_planning/path_planning.h"
   
   
   namespace f2c {
   
   namespace types {}
   
   namespace sg {}
   
   namespace rp {}
   
   namespace pp {}
   
   namespace obj {}
   
   namespace hg {}
   
   namespace decomp {}
   
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_H_
   
