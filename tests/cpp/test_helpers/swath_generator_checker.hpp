//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef SWATH_GENERATOR_CHECKER_HPP_
#define SWATH_GENERATOR_CHECKER_HPP_

#include <gtest/gtest.h>
#include <fstream>
#include "fields2cover.h"

inline testing::AssertionResult isSwathGenerationCorrect(
    f2c::sg::SwathGeneratorBase& sw_gen, f2c::obj::SGObjective& obj,
    const F2CCell& poly, double swath_width = 1.0, int checks = 100) {
  auto swaths = sw_gen.generateBestSwaths(obj, swath_width, poly);
  if (swaths.size() < 1) {
    return testing::AssertionFailure() <<
      "Error 2001: No swaths were generated.";
  } else if (swaths.size() > poly.area() / (swath_width * swath_width)) {
    return testing::AssertionFailure() <<
      "Error 2002: " << swaths.size() << " swaths were generated." <<
      "Expected less than " << poly.area() / (swath_width * swath_width) <<
      "swaths.";
  }
  f2c::Random rand;
  for(int i = 0; i < checks; ++i) {
    double rand_ang = rand.getAngleRandom();
    auto swaths2 = sw_gen.generateSwaths(rand_ang, swath_width, poly);
    if (obj.computeCostWithMinimizingSign(swaths) >
        obj.computeCostWithMinimizingSign(swaths2)) {
      return testing::AssertionFailure() <<
        "Error 2003: Cost (signed) of the optimal angle is " <<
        obj.computeCostWithMinimizingSign(swaths) <<
        ", but the cost (signed) of a non-optimal angle (" <<
        rand_ang << ") is " <<
        obj.computeCostWithMinimizingSign(swaths2) << ".";
    } else if (swaths.size() > poly.area() / (swath_width * swath_width)) {
      return testing::AssertionFailure() << "Error 2004: " << swaths2.size() <<
        " swaths were generated with angle " << rand_ang <<
        "Expected less than " << poly.area() / (swath_width * swath_width) <<
        "swaths.";
    }
  }
  return testing::AssertionSuccess();
}

inline testing::AssertionResult isSwathGenerationCorrect(
    f2c::sg::SwathGeneratorBase& sw_gen, f2c::obj::SGObjective& obj,
    const F2CCells& poly, double swath_width = 1.0, int checks = 100) {
  auto swaths_by_cells = sw_gen.generateBestSwaths(obj, swath_width, poly);
  EXPECT_EQ(swaths_by_cells.size(), poly.size());

  for (auto&& swaths : swaths_by_cells) {
    if (swaths.size() < 1) {
      return testing::AssertionFailure() <<
        "Error 2001: No swaths were generated.";
    } else if (swaths.size() > poly.area() / (swath_width * swath_width)) {
      return testing::AssertionFailure() <<
        "Error 2002: " << swaths.size() << " swaths were generated." <<
        "Expected less than " << poly.area() / (swath_width * swath_width) <<
        "swaths.";
    }
  }
  f2c::Random rand;
  for(int i = 0; i < checks; ++i) {
    double rand_ang = rand.getAngleRandom();
    auto swaths_by_cells2 = sw_gen.generateSwaths(rand_ang, swath_width, poly);
    EXPECT_EQ(swaths_by_cells2.size(), poly.size());
    for (int j = 0; j < swaths_by_cells2.size(); ++j) {
      if (obj.computeCostWithMinimizingSign(swaths_by_cells[j]) >
          obj.computeCostWithMinimizingSign(swaths_by_cells2[j])) {
        return testing::AssertionFailure() <<
          "Error 2003: Cost (signed) of the optimal angle is " <<
          obj.computeCostWithMinimizingSign(swaths_by_cells[j]) <<
          ", but the cost (signed) of a non-optimal angle (" <<
          rand_ang << ") is " <<
          obj.computeCostWithMinimizingSign(swaths_by_cells2[j]) << ".";
      } else if (swaths_by_cells[j].size() > poly.area() / (swath_width * swath_width)) {
        return testing::AssertionFailure() << "Error 2004: " << swaths_by_cells2[j].size() <<
          " swaths were generated with angle " << rand_ang <<
          "Expected less than " << poly.area() / (swath_width * swath_width) <<
          "swaths.";
      }
    }
  }
  return testing::AssertionSuccess();
}

inline testing::AssertionResult areSwathsCorrectlyGen(
    const F2CSwaths& swaths, const std::vector<double>& swaths_x) {
  if (swaths.size() != swaths_x.size()) {
    return testing::AssertionFailure() <<
      "Error 2005: swaths and swaths_x need to have equal size.";
  }
  std::string str_swaths {"("};
  std::string str_ids {"("};
  bool correct_order {true};
  for (size_t i = 0; i < swaths.size(); ++i) {
    str_swaths += std::to_string(int(swaths[i].startPoint().getX()));
    str_ids += std::to_string(int(swaths_x[i]));
    if (i + 1 != swaths.size()) {
      str_swaths += ", ";
      str_ids += ", ";
    } else {
      str_swaths += "), ";
      str_ids += ")";
    }
    correct_order = int(swaths[i].startPoint().getX()) == int(swaths_x[i]) ? correct_order : false;
  }

  if (!correct_order) {
    return testing::AssertionFailure() <<
      "Error 2006: swaths sequence is: " << str_swaths << " but should be " << str_ids;
  }
  return testing::AssertionSuccess();
}

#endif  // SWATH_GENERATOR_CHECKER_HPP_
