
.. _program_listing_file_fields2cover_types.h:

Program Listing for File types.h
================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_types.h>` (``fields2cover/types.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_TYPES_H_
   #define FIELDS2COVER_TYPES_H_
   
   #include <gdal/ogr_geometry.h>
   #include <vector>
   
   
   #include "fields2cover/types/Point.h"
   #include "fields2cover/types/MultiPoint.h"
   #include "fields2cover/types/LinearRing.h"
   #include "fields2cover/types/LineString.h"
   #include "fields2cover/types/MultiLineString.h"
   #include "fields2cover/types/Cell.h"
   #include "fields2cover/types/Cells.h"
   
   #include "fields2cover/types/Field.h"
   
   #include "fields2cover/types/Swath.h"
   #include "fields2cover/types/Swaths.h"
   #include "fields2cover/types/Route.h"
   #include "fields2cover/types/Path.h"
   #include "fields2cover/types/OptimizationParams.h"
   #include "fields2cover/types/Robot.h"
   
   typedef f2c::types::Robot F2CRobot;
   typedef f2c::types::OptimizationParams F2COptim;
   
   typedef f2c::types::Point F2CPoint;
   typedef f2c::types::MultiPoint F2CMultiPoint;
   typedef f2c::types::LineString F2CLineString;
   typedef f2c::types::LinearRing F2CLinearRing;
   typedef f2c::types::MultiLineString F2CMultiLineString;
   typedef f2c::types::Cell F2CCell;
   typedef f2c::types::Cells F2CCells;
   typedef f2c::types::Field F2CField;
   typedef f2c::types::Path F2CPath;
   typedef f2c::types::Route F2CRoute;
   typedef f2c::types::Swath F2CSwath;
   typedef f2c::types::Swaths F2CSwaths;
   
   typedef std::vector<F2CRobot> F2CRobots;
   typedef std::vector<F2CField> F2CFields;
   typedef std::vector<F2CSwaths> F2CSwathsByCells;
   
   #endif  //  FIELDS2COVER_TYPES_H_
