//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_BOUSTROPHEDON_ORDER_H_
#define FIELDS2COVER_ROUTE_PLANNING_BOUSTROPHEDON_ORDER_H_

#include "fields2cover/types.h"
#include "fields2cover/route_planning/single_cell_swaths_order_base.h"

namespace f2c::rp {

class BoustrophedonOrder : public SingleCellSwathsOrderBase {
 protected:
  void sortSwaths(F2CSwaths& swaths) const override;
};


}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_BOUSTROPHEDON_ORDER_H_
