
.. _program_listing_file_fields2cover_types_Path.h:

Program Listing for File Path.h
===============================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_types_Path.h>` (``fields2cover/types/Path.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                           BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_TYPES_PATH_H_
   #define FIELDS2COVER_TYPES_PATH_H_
   
   #include <gdal/ogr_geometry.h>
   #include <fstream>
   #include <vector>
   #include <string>
   #include <limits>
   #include "fields2cover/types/Swath.h"
   #include "fields2cover/types/Point.h"
   
   namespace f2c {
   namespace types {
   
   enum PathSectionType {
     SWATH = 1,
     TURN = 2,
   };
   
   enum PathDirection {
     FORWARD = 1,
     BACKWARD = -1,
     // STOP = 0,
   };
   
   struct Path {
    public:
     std::vector<Point> points;
     std::vector<double> angles;
     std::vector<double> velocities;
     std::vector<double> durations;
     std::vector<PathDirection> directions;
     std::vector<PathSectionType> type;
     double task_time {0.0};
     double measure_error_ {0.1};
   
    public:
     Path& operator+=(const Path& _path);
     Path clone() const;
     size_t size() const;
   
     void moveTo(const Point&);
   
     double length(void) const;
   
     void appendSwath(const Swath& _swath, double cruise_speed);
   
     std::string serializePath(void) const;
     void saveToFile(const std::string& _file) const;
     void loadFile(const std::string& _file);
   
     bool isValid() const;
     Path& populate(int _number_points = 100);
     Path& reduce(double _min_dist_equal = 0.1);
   };
   
   }  // namespace types
   }  // namespace f2c
   
   #endif  //  FIELDS2COVER_TYPES_PATH_H_
