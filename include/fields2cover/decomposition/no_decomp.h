//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_DECOMPOSITION_NO_DECOMP_H_
#define FIELDS2COVER_DECOMPOSITION_NO_DECOMP_H_

#include "fields2cover/types.h"
#include "fields2cover/decomposition/decomposition_base.h"

namespace f2c::decomp {

class NoDecomp : public DecompositionBase {
 public:
  F2CMultiLineString genSplitLines(const F2CCells& cells,
      const obj::DecompObjective& obj = obj::DecompObjective()) const override;
};

}  // namespace f2c::decomp


#endif  // FIELDS2COVER_DECOMPOSITION_NO_DECOMP_H_
