//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_HEADLAND_GENERATOR_HEADLAND_GENERATOR_BASE_H_
#define FIELDS2COVER_HEADLAND_GENERATOR_HEADLAND_GENERATOR_BASE_H_

#include "fields2cover/types.h"
#include "fields2cover/objectives/optimization_class.h"

namespace f2c::hg {

/// Base class to generate the headlands.
template <typename T>
class HeadlandGeneratorBase : public obj::OptimizationClass<T> {
 public:
  /// @cond DOXYGEN_SHOULD_SKIP_THIS
  virtual ~HeadlandGeneratorBase() = default;
  /// @endcond

 public:
  /// Get the regions of the field without headlands.
  /// @param _field Original field
  /// @param _dist_headland Width of the headland
  /// @return Field without headlands
  virtual F2CCells generateHeadlands(const F2CCells& field,
      double dist_headland) = 0;
};

}  // namespace f2c::hg

#endif  // FIELDS2COVER_HEADLAND_GENERATOR_HEADLAND_GENERATOR_BASE_H_
