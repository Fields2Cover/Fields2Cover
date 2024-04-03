//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_route, init) {
  EXPECT_TRUE(F2CRoute().asLineString().isEmpty());
  F2CRoute route;

  EXPECT_EQ(route.startPoint(), F2CPoint());
  route.addConnection(F2CMultiPoint({F2CPoint(-2, -1), F2CPoint(0, -1), F2CPoint(0, 0)}));
  EXPECT_EQ(route.getLastConnection().size(), 3);
  EXPECT_EQ(route.startPoint(), F2CPoint(-2, -1));
  F2CSwaths swaths1;
  swaths1.emplace_back(F2CLineString({F2CPoint(0, 0), F2CPoint(1, 0)}), 4);
  swaths1.emplace_back(F2CLineString({F2CPoint(1, 1), F2CPoint(0, 1)}), 4);
  route.addSwaths(swaths1);
  EXPECT_EQ(route.getLastSwaths().size(), 2);
  route.addConnection(F2CMultiPoint({F2CPoint(0, 1), F2CPoint(0, 2)}));
  EXPECT_EQ(route.getLastConnection().size(), 2);
  F2CSwaths swaths2;
  swaths2.emplace_back(F2CLineString({F2CPoint(0, 2), F2CPoint(1, 2)}), 6);
  swaths2.emplace_back(F2CLineString({F2CPoint(1, 3), F2CPoint(0, 3)}), 6);
  swaths2.emplace_back(F2CLineString({F2CPoint(0, 4), F2CPoint(1, 4)}), 6);
  swaths2.emplace_back(F2CLineString({F2CPoint(1, 5), F2CPoint(0, 5)}), 6);
  route.addSwaths(swaths2);

  const F2CRoute c_route {route};
  EXPECT_EQ(c_route.getLastSwaths().size(), 4);
  EXPECT_EQ(c_route.getLastConnection().size(), 2);

  route.addConnection();
  EXPECT_FALSE(route.isEmpty());
  EXPECT_FALSE(route.asLineString().isEmpty());
  EXPECT_EQ(route.sizeVectorSwaths(), 2);
  EXPECT_EQ(route.sizeConnections(), 3);
  EXPECT_EQ(route.asLineString().length(), 14);
  EXPECT_EQ(route.length(), 14);
  EXPECT_EQ(route.clone().length(), 14);
  EXPECT_EQ(route.getSwaths(0).size(), 2);
  EXPECT_EQ(route.getSwaths(1).size(), 4);

  route.addConnection(F2CMultiPoint({F2CPoint(0, 5), F2CPoint(0, 6)}));
  route.addConnection(F2CMultiPoint({F2CPoint(0, 8), F2CPoint(5, 8)}));
  route.addConnection(F2CMultiPoint({F2CPoint(5, 10), F2CPoint(10, 10)}));
  EXPECT_EQ(route.sizeVectorSwaths(), 2);
  EXPECT_EQ(route.sizeConnections(), 3);
  F2CSwaths swaths3;
  swaths3.emplace_back(F2CLineString({F2CPoint(20, 20), F2CPoint(21, 20)}), 6);

  route.addConnectedSwaths(F2CMultiPoint({F2CPoint(10, 10), F2CPoint(20, 10)}), swaths3);
  EXPECT_EQ(route.sizeVectorSwaths(), 3);
  EXPECT_EQ(route.sizeConnections(), 3);
  EXPECT_EQ(route.length(), 50);

  EXPECT_EQ(route.getConnection(0).size(), 3);
  EXPECT_EQ(route.getSwaths(0).size(),     2);
  EXPECT_EQ(route.getConnection(1).size(), 2);
  EXPECT_EQ(route.getSwaths(1).size(),     4);
  EXPECT_EQ(route.getConnection(2).size(), 8);
  EXPECT_EQ(route.getSwaths(2).size(),     1);

  EXPECT_EQ(route.getLastConnection().size(), 8);
  EXPECT_EQ(route.getLastSwaths().size(),     1);
  EXPECT_EQ(route.getLastConnection().back(), F2CPoint(20, 10));
  EXPECT_EQ(route.getLastSwaths().back().getPath().endPoint(), F2CPoint(21, 20));
  F2CPoint p_rand1 (33, 44), p_rand2 (-22, 11), p_rand3(5, 6);
  route.getLastSwaths().back().setPath(F2CLineString(p_rand1, p_rand2));
  size_t n = route.getLastConnection().size();
  route.getLastConnection().setGeometry(n-1, p_rand3);
  EXPECT_EQ(route.getLastSwaths().back().getPath().endPoint(), p_rand2);
  EXPECT_EQ(route.getLastConnection().back(), p_rand3);

  EXPECT_EQ(route.startPoint(), F2CPoint(-2, -1));


  F2CRoute route2;
  route2.addSwaths(c_route.getLastSwaths());
  route2.addConnection(route.getLastConnection());
  EXPECT_EQ(route2.startPoint(), F2CPoint(0, 2));
  EXPECT_EQ(route2.endPoint(), p_rand3);


}


