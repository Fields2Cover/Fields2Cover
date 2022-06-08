
.. _program_listing_file_fields2cover_types_Field.h:

Program Listing for File Field.h
================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_types_Field.h>` (``fields2cover/types/Field.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                           BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_TYPES_FIELD_H_
   #define FIELDS2COVER_TYPES_FIELD_H_
   
   #include <string>
   #include <memory>
   #include "fields2cover/types/Point.h"
   #include "fields2cover/types/Cells.h"
   
   namespace f2c {
   namespace types {
   
   struct Field {
    public:
     std::string id {""};
     std::string coord_sys {""};
     Point ref_point;
     Cells field;
   
    public:
     Field();
     explicit Field(const Cells& cells, const std::string& _id = "");
     ~Field();
     Field(const Field&);
     Field(Field&&);
     Field& operator=(Field&&);
     Field& operator=(const Field&);
   
    public:
     Field clone() const;
     double getArea(void) const;
     static bool isCoordSystemUTM(const std::string& _coord_sys);
     bool isCoordSystemUTM() const;
     static std::string getUTMCoordSystem(const std::string& _coord_sys);
     std::string getUTMCoordSystem() const;
     static bool isCoordSystemEPSG(const std::string& _coord_sys);
     bool isCoordSystemEPSG() const;
     static int getEPSGCoordSystem(const std::string& _coord_sys);
     int getEPSGCoordSystem() const;
     void setEPSGCoordSystem(int _epsg);
     void setUTMCoordSystem(const std::string& _utm);
   };
   
   }  // namespace types
   }  // namespace f2c
   
   #endif  //  FIELDS2COVER_TYPES_FIELD_H_
