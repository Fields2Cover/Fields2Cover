//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_HEADLAND_GENERATOR_CONSTANT_HEADLAND_H_
#define FIELDS2COVER_HEADLAND_GENERATOR_CONSTANT_HEADLAND_H_

#include "fields2cover/types.h"
#include "fields2cover/headland_generator/headland_generator_base.h"

namespace f2c::hg {

/// Class to generate headlands with equal width in each border.
class ConstHL : public HeadlandGeneratorBase {
 public:
  F2CCells generateHeadlands(const F2CCells& field,
      double dist_headland) override;
};

}  // namespace f2c::hg


#endif  // FIELDS2COVER_HEADLAND_GENERATOR_CONSTANT_HEADLAND_H_
