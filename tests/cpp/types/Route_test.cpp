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

namespace {

F2CRoute createRouteToReverse() {
  F2CRoute route;
  route.addConnection(F2CMultiPoint({F2CPoint(-2, 0), F2CPoint(0, 0)}));
  F2CSwaths swaths1;
  swaths1.emplace_back(F2CLineString({F2CPoint(0, 0), F2CPoint(2, 0)}), 1);
  route.addSwaths(swaths1);
  route.addConnection(F2CMultiPoint({F2CPoint(2, 0), F2CPoint(2, 1)}));
  F2CSwaths swaths2;
  swaths2.emplace_back(F2CLineString({F2CPoint(2, 1), F2CPoint(0, 1)}), 1);
  swaths2.emplace_back(F2CLineString({F2CPoint(0, 2), F2CPoint(2, 2)}), 1);
  route.addSwaths(swaths2);
  return route;
}

void expectSameLine(const F2CLineString& a, const F2CLineString& b) {
  ASSERT_EQ(a.size(), b.size());
  for (size_t i = 0; i < a.size(); ++i) {
    EXPECT_EQ(a[i], b[i]);
  }
}

}  // namespace

TEST(fields2cover_types_route, reverse) {
  F2CRoute route = createRouteToReverse();
  const F2CPoint start = route.startPoint();
  const F2CPoint end = route.endPoint();
  const double length = route.length();
  const F2CLineString line = route.asLineString();
  const size_t n_connections = route.sizeConnections();

  route.reverse();
  EXPECT_EQ(route.startPoint(), end);
  EXPECT_EQ(route.endPoint(), start);
  EXPECT_NEAR(route.length(), length, 1e-7);

  // Swaths are driven in the opposite direction too.
  EXPECT_EQ(route.getSwaths(0)[0].startPoint(), F2CPoint(2, 2));
  EXPECT_EQ(route.getSwaths(0)[0].endPoint(), F2CPoint(0, 2));

  route.reverse();
  expectSameLine(route.asLineString(), line);
  EXPECT_EQ(route.sizeConnections(), n_connections);
}

TEST(fields2cover_types_route, reverseWithLastConnection) {
  F2CRoute route = createRouteToReverse();
  route.addConnection(F2CMultiPoint({F2CPoint(2, 2), F2CPoint(4, 2)}));
  const F2CPoint start = route.startPoint();
  const F2CPoint end = route.endPoint();
  const F2CLineString line = route.asLineString();
  const size_t n_connections = route.sizeConnections();

  route.reverse();
  EXPECT_EQ(route.startPoint(), end);
  EXPECT_EQ(route.endPoint(), start);
  EXPECT_EQ(route.sizeConnections(), n_connections);

  route.reverse();
  expectSameLine(route.asLineString(), line);
}


