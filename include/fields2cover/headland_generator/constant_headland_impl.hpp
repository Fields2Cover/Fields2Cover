//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_HEADLAND_GENERATOR_CONSTANT_HEADLAND_IMPL_HPP_
#define FIELDS2COVER_HEADLAND_GENERATOR_CONSTANT_HEADLAND_IMPL_HPP_

#include <utility>

namespace f2c {
namespace hg {


template <typename T>
F2CCells ConstHL<T>::generateHeadlands(
    const F2CCells& field, double dist_headland) {
  F2CCells red_field = field.clone();
  for (auto&& poly : field) {
    for (auto&& ring : poly) {
      auto lines = F2CMultiLineString::getLineSegments(F2CLineString(ring));
      for (auto&& line : lines) {
        red_field = red_field.Difference(
              F2CCells::Buffer(line, dist_headland));
      }
    }
  }
  return red_field;
}


}  // namespace hg
}  // namespace f2c

#endif  // FIELDS2COVER_HEADLAND_GENERATOR_CONSTANT_HEADLAND_IMPL_HPP_
