//=============================================================================
//    Copyright (C) 2021-2023 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_route, init) {
  EXPECT_TRUE(F2CRoute().getRouteAsLine().isEmpty());
  F2CRoute route;
  route.connections.emplace_back(F2CMultiPoint({F2CPoint(0, 0)}));
  F2CSwaths swaths1;
  swaths1.emplace_back(F2CLineString({F2CPoint(0, 0), F2CPoint(1, 0)}), 4);
  route.v_swaths.emplace_back(swaths1);
  route.connections.emplace_back(F2CMultiPoint({F2CPoint(1, 0), F2CPoint(1, 1)}));
  F2CSwaths swaths2;
  swaths2.emplace_back(F2CLineString({F2CPoint(1, 1), F2CPoint(0, 1)}), 4);
  route.v_swaths.emplace_back(swaths2);
  route.connections.emplace_back(F2CMultiPoint());
  route.type = f2c::types::RouteType::R_START_END;
  EXPECT_FALSE(route.getRouteAsLine().isEmpty());
  EXPECT_EQ(route.getRouteAsLine().getLength(), 3);
  EXPECT_EQ(route.getLength(), 3);
  EXPECT_EQ(route.clone().getLength(), 3);
}


