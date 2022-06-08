//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef SWATH_GENERATOR_CHECKER_HPP_
#define SWATH_GENERATOR_CHECKER_HPP_

#include <gtest/gtest.h>
#include <fstream>
#include "fields2cover.h"

template <typename T>
inline testing::AssertionResult IsSwathGenerationCorrect(
    f2c::sg::SwathGeneratorBase<T>& sw_gen, const F2CCell& poly,
    double swath_width = 1.0, int checks = 100) {
  auto swaths = sw_gen.generateBestSwaths(swath_width, poly);
  if (sw_gen.getBestAngle() == -1e7) {
    return testing::AssertionFailure() <<
      "Error 2000: Best angle not set.";
  } else if (swaths.size() < 1) {
    return testing::AssertionFailure() <<
      "Error 2001: No swaths were generated.";
  } else if (swaths.size() > poly.getArea() / (swath_width * swath_width)) {
    return testing::AssertionFailure() <<
      "Error 2002: " << swaths.size() << " swaths were generated." <<
      "Expected less than " << poly.getArea() / (swath_width * swath_width) <<
      "swaths.";
  }
  f2c::Random rand;
  for(int i = 0; i < checks; ++i) {
    double rand_ang = rand.getAngleRandom();
    auto swaths2 = sw_gen.generateSwaths(rand_ang, swath_width, poly);
    if (sw_gen.computeCostWithMinimizingSign(swaths) >
        sw_gen.computeCostWithMinimizingSign(swaths2)) {
      return testing::AssertionFailure() <<
        "Error 2003: Cost (signed) of the optimal angle ("<<
        sw_gen.getBestAngle() << ") is " << 
        sw_gen.computeCostWithMinimizingSign(swaths) <<
        ", but the cost (signed) of a non-optimal angle (" <<
        rand_ang << ") is " <<
        sw_gen.computeCostWithMinimizingSign(swaths2) << ".";
    } else if (swaths.size() > poly.getArea() / (swath_width * swath_width)) {
      return testing::AssertionFailure() << "Error 2004: " << swaths2.size() <<
        " swaths were generated with angle " << rand_ang <<
        "Expected less than " << poly.getArea() / (swath_width * swath_width) <<
        "swaths.";
    }
  }
  return testing::AssertionSuccess();
}
  

#endif  // SWATH_GENERATOR_CHECKER_HPP_
