
.. _program_listing_file_fields2cover_types_Swaths.h:

Program Listing for File Swaths.h
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_types_Swaths.h>` (``fields2cover/types/Swaths.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                           BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_TYPES_SWATHS_H_
   #define FIELDS2COVER_TYPES_SWATHS_H_
   
   #include <vector>
   #include "fields2cover/types/LineString.h"
   #include "fields2cover/types/MultiLineString.h"
   #include "fields2cover/types/Cell.h"
   #include "fields2cover/types/Cells.h"
   #include "fields2cover/types/Swath.h"
   
   namespace f2c {
   namespace types {
   
   struct Swaths {
     Swaths();
     explicit Swaths(int i);
     Swaths(const std::initializer_list<Swath>& _s);
     ~Swaths();
   
     void emplace_back(const Swath& _s);
   
     void emplace_back(const LineString& _l, double _w, int _id = 0);
   
     void push_back(const Swath& _s);
   
     std::vector<Swath>::iterator begin();
     std::vector<Swath>::iterator end();
     std::vector<Swath>::const_iterator begin() const;
     std::vector<Swath>::const_iterator end() const;
     void reverse();
   
     Swath& back();
     const Swath& back() const;
   
     Swath& at(size_t i);
   
     size_t size() const;
   
     Swath& operator[] (int i);
   
     const Swath& operator[] (int i) const;
   
     void append(const LineString& _line, double _width = 0);
     void append(const MultiLineString& _line, double _width = 0);
     void append(const LineString& _line, const Cell& _poly, double _width = 0);
     void append(const MultiLineString& _lines, const Cell& _poly,
         double _width = 0);
     void append(const LineString& _line, const Cells& _polys,
         double _width = 0);
     void append(const MultiLineString& _lines, const Cells& _poly,
         double _width = 0);
     void sort();
     void reverseDirOddSwaths();
   
     Swaths clone() const;
   
    private:
     std::vector<Swath> data;
   };
   
   
   }  // namespace types
   }  // namespace f2c
   
   #endif  //  FIELDS2COVER_TYPES_SWATHS_H_
