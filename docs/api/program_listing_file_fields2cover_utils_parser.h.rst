
.. _program_listing_file_fields2cover_utils_parser.h:

Program Listing for File parser.h
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_utils_parser.h>` (``fields2cover/utils/parser.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                        BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_UTILS_PARSER_H_
   #define FIELDS2COVER_UTILS_PARSER_H_
   
   #include <string>
   #include <algorithm>
   #include <vector>
   #include "fields2cover/types.h"
   #include "fields2cover/utils/transformation.h"
   
   namespace f2c {
   
   class Parser {
    public:
       static int importGml(const std::string& _file, F2CFields& _fields);
   
       static int importJson(const std::string& _file, F2CFields& _fields);
   };
   
   }  // namespace f2c
   
   #endif  // FIELDS2COVER_UTILS_PARSER_H_
