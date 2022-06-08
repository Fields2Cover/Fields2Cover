
.. _program_listing_file_fields2cover_utils_random.h:

Program Listing for File random.h
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_utils_random.h>` (``fields2cover/utils/random.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_UTILS_RANDOM_H_
   #define FIELDS2COVER_UTILS_RANDOM_H_
   
   #include <time.h>
   #include <limits>
   #include <boost/math/constants/constants.hpp>
   #include "fields2cover/types/Point.h"
   #include "fields2cover/types/LineString.h"
   #include "fields2cover/types/LinearRing.h"
   #include "fields2cover/types/Field.h"
   #include "fields2cover/types/Cell.h"
   
   namespace f2c {
   
   class Random {
    public:
     explicit Random(uint32_t _seed = static_cast<uint32_t>(time(NULL))) :
       seed_(_seed) {}
   
     ~Random() = default;
     Random(const Random&) = default;
     Random &operator=(const Random&) = default;
     Random(Random&&) = default;
     Random &operator=(Random&&) = default;
   
    public:
     double getRandomDouble(void);
   
     double getRandomLinear(double _min, double _max);
   
     double getRandomExp(double _min, double _max);
   
     double getAngleRandom(void);
   
     f2c::types::Field generateRandField(int _n_sides, double _area,
         double _min_width = 0.5, double _max_width = 1.0);
   
     f2c::types::Field genConvexField(double _area);
   
     f2c::types::Field genNonConvexField(double _area);
   
    private:
     uint32_t seed_;
   };
   
   
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_UTILS_RANDOM_H_
