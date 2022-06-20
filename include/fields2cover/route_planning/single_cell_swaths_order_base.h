//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_SINGLE_CELL_SWATHS_ORDER_BASE_H_
#define FIELDS2COVER_ROUTE_PLANNING_SINGLE_CELL_SWATHS_ORDER_BASE_H_

#include "fields2cover/types.h"

namespace f2c {
namespace rp {


class SingleCellSwathsOrderBase {
 public:
  explicit SingleCellSwathsOrderBase(F2CSwaths& swaths);
  virtual ~SingleCellSwathsOrderBase();

  virtual F2CSwaths& genSortedSwaths();

  virtual void setSwaths(F2CSwaths& swaths);
  virtual F2CSwaths& getSwaths() {return swaths_;}
  void setCounter(uint32_t new_count) {counter_ = new_count;}

 protected:
  virtual void sortSwaths() = 0;
  virtual void changeStartPoint();

 protected:
  F2CSwaths swaths_;
  uint32_t counter_ {0};
};



}  // namespace rp
}  // namespace f2c

#endif  // FIELDS2COVER_ROUTE_PLANNING_SINGLE_CELL_SWATHS_ORDER_BASE_H_
