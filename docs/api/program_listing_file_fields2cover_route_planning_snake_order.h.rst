
.. _program_listing_file_fields2cover_route_planning_snake_order.h:

Program Listing for File snake_order.h
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_route_planning_snake_order.h>` (``fields2cover/route_planning/snake_order.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_ROUTE_PLANNING_SNAKE_ORDER_H_
   #define FIELDS2COVER_ROUTE_PLANNING_SNAKE_ORDER_H_
   
   #include "fields2cover/types.h"
   #include "fields2cover/route_planning/single_cell_swaths_order_base.h"
   
   namespace f2c {
   namespace rp {
   
   class SnakeOrder : public SingleCellSwathsOrderBase {
    public:
     using SingleCellSwathsOrderBase::SingleCellSwathsOrderBase;
   
    private:
     void sortSwaths() override;
   };
   
   
   
   }  // namespace rp
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_ROUTE_PLANNING_SNAKE_ORDER_H_
