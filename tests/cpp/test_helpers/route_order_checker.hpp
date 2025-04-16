//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef ROUTE_ORDER_CHECKER_HPP_
#define ROUTE_ORDER_CHECKER_HPP_

#include <gtest/gtest.h>
#include <fstream>
#include "fields2cover.h"

inline F2CSwaths genSwathsTest(int n) {
  F2CSwaths swaths;
  for (int i = 1; i < n; ++i) {
    swaths.emplace_back(F2CLineString({F2CPoint(0, i), F2CPoint(1, i)}), i, i);
  }
  return swaths;
}



inline testing::AssertionResult isRouteOrderCorrect(
    const F2CSwaths& swaths, const std::vector<size_t>& ids) {
  if (swaths.size() != ids.size()) {
    return testing::AssertionFailure() <<
      "Error 4001: swaths and ids need to have equal size.";
  }
  std::string str_swaths {"("};
  std::string str_ids {"("};
  bool correct_order {true};
  for (size_t i = 0; i < swaths.size(); ++i) {
    str_swaths += std::to_string(int(swaths[i].getWidth()));
    str_ids += std::to_string(int(ids[i]));
    if (i + 1 != ids.size()) {
      str_swaths += ", ";
      str_ids += ", ";
    } else {
      str_swaths += "), ";
      str_ids += ")";
    }
    correct_order = int(swaths[i].getWidth()) == int(ids[i]) ? correct_order : false;
  }

  if (!correct_order) {
    return testing::AssertionFailure() <<
      "Error 4002: swaths sequence is: " << str_swaths << " but should be " << str_ids;
  }

  return testing::AssertionSuccess();
}

#endif  // ROUTE_ORDER_CHECKER_HPP_
