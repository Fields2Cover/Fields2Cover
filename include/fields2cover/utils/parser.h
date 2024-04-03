//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
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

/// Class to parse files of the OGC Standards to our custom format
class Parser {
 public:
    /// Function to import file with gml extension.
    /// @param file Path to the imported file.
    /// @param fields Set of fields saved on _file.
    static void importGml(const std::string& file, F2CFields& fields);

    static F2CField importFieldGml(
        const std::string& file, bool fail_silently = false);

    /// Function to import file with Json extension.
    /// @param file Path to the imported file.
    /// @param fields Set of fields saved on _file.
    /// @return return 0 if the file was parsed correctly.
    /// Otherwise, return a negative number.
    static int importJson(const std::string& file, F2CFields& fields);

    static F2CCell importCellJson(const std::string& file);
    static F2CSwaths importSwathsJson(const std::string& file);
    static F2CStrips importStripsJson(const std::string& file);
};

}  // namespace f2c

#endif  // FIELDS2COVER_UTILS_PARSER_H_
