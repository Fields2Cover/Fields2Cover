//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types/Graph.h"


std::vector<size_t> createTestPoints() {
  size_t pA {112}, pB {223}, pC {334}, pD {445},
         pE {556}, pF {667}, pG {778}, pH {889};
  return {pA, pB, pC, pD, pE, pF, pG, pH};
}


// Creates the following graph:
//        A ----1----> B->-|    G
//        |            |   |    |
//        |2           |7  |    v
//        |            |   |   999
//        C ----5----- D   |    |
//        |                v    H
//       10               100
//        ^                |
//        |                |
//        E ----4----- F   |
//        ^                |
//        |-------<--------|
//

f2c::types::Graph createTestGraph() {
  std::vector<size_t> v_ps = createTestPoints();
  size_t pA {v_ps[0]}, pB {v_ps[1]}, pC {v_ps[2]}, pD {v_ps[3]},
         pE {v_ps[4]}, pF {v_ps[5]}, pG {v_ps[6]}, pH {v_ps[7]};
  f2c::types::Graph g;
  g.addDirectedEdge(pA, pB, 1);
  g.addDirectedEdge(pB, pE, 100);
  g.addDirectedEdge(pE, pC, 10);
  g.addDirectedEdge(pG, pH, 999);
  g.addEdge(pB, pD, 7);
  g.addEdge(pC, pD, 5);
  g.addEdge(pC, pA, 2);
  g.addEdge(pE, pF, 4);
  return g;
}

TEST(fields2cover_types_graph, addEdges_removeEdges) {
  std::vector<size_t> v_ps = createTestPoints();
  size_t pA {v_ps[0]}, pB {v_ps[1]}, pC {v_ps[2]}, pD {v_ps[3]},
         pE {v_ps[4]}, pF {v_ps[5]}, pG {v_ps[6]}, pH {v_ps[7]};
  f2c::types::Graph g = createTestGraph();

  EXPECT_EQ(g.numNodes(), 8);
  EXPECT_EQ(g.numEdges(), 12);

  g.removeEdge(pA, pC);
  EXPECT_EQ(g.numNodes(), 8);
  EXPECT_EQ(g.numEdges(), 10);

  g.removeEdge(pC, pA);
  EXPECT_EQ(g.numNodes(), 8);
  EXPECT_EQ(g.numEdges(), 10);

  g.removeEdge(pB, pE);
  EXPECT_EQ(g.numNodes(), 8);
  EXPECT_EQ(g.numEdges(), 9);

  g.removeDirectedEdge(pG, pH);
  EXPECT_EQ(g.numNodes(), 7);
  EXPECT_EQ(g.numEdges(), 8);

  g.removeDirectedEdge(pH, pG);
  EXPECT_EQ(g.numNodes(), 7);
  EXPECT_EQ(g.numEdges(), 8);
}

TEST(fields2cover_types_graph, getEdgesFrom) {
  std::vector<size_t> v_ps = createTestPoints();
  size_t pA {v_ps[0]}, pB {v_ps[1]}, pC {v_ps[2]}, pD {v_ps[3]},
         pE {v_ps[4]}, pF {v_ps[5]}, pG {v_ps[6]}, pH {v_ps[7]};
  f2c::types::Graph g = createTestGraph();

  auto edges_from_A = g.getEdgesFrom(pA);
  EXPECT_EQ(edges_from_A.size(), 2);
  EXPECT_EQ(edges_from_A[0], pC);
  EXPECT_EQ(edges_from_A[1], pB);

  g.removeEdge(pG, pH);
  EXPECT_EQ(g.getEdgesFrom(pG).size(), 0);

  auto edges_from_B = g.getEdgesFrom(pB);
  EXPECT_EQ(edges_from_B.size(), 2);
  EXPECT_EQ(edges_from_B[0], pD);
  EXPECT_EQ(edges_from_B[1], pE);

  auto edges_from_G = g.getEdgesFrom(pG);
  EXPECT_EQ(edges_from_G.size(), 0);

  auto all_edges = g.getEdges();
  EXPECT_EQ(all_edges.size(), g.numNodes());
  EXPECT_EQ(all_edges[pA].size(), edges_from_A.size());
  EXPECT_EQ(all_edges[pB].size(), edges_from_B.size());
  EXPECT_EQ(all_edges[pG].size(), edges_from_G.size());

  f2c::types::Graph g1;
  EXPECT_EQ(g1.getEdgesFrom(0).size(), 0);
}

TEST(fields2cover_types_graph, getCostFromEdge) {
  std::vector<size_t> v_ps = createTestPoints();
  size_t pA {v_ps[0]}, pB {v_ps[1]}, pC {v_ps[2]}, pD {v_ps[3]},
         pE {v_ps[4]}, pF {v_ps[5]}, pG {v_ps[6]}, pH {v_ps[7]};
  f2c::types::Graph g = createTestGraph();

  EXPECT_EQ(g.getCostFromEdge(pA, pB), 1);
  EXPECT_EQ(g.getCostFromEdge(pB, pE), 100);
  EXPECT_EQ(g.getCostFromEdge(pE, pC), 10);
  EXPECT_EQ(g.getCostFromEdge(pB, pD), 7);
  EXPECT_EQ(g.getCostFromEdge(pC, pD), 5);
  EXPECT_EQ(g.getCostFromEdge(pC, pA), 2);
  EXPECT_EQ(g.getCostFromEdge(pE, pF), 4);
  EXPECT_EQ(g.getCostFromEdge(pG, pH), 999);

  EXPECT_GT(g.getCostFromEdge(pB, pA), 1e5);
  EXPECT_GT(g.getCostFromEdge(pE, pB), 1e5);
  EXPECT_GT(g.getCostFromEdge(pC, pF), 1e5);
}

TEST(fields2cover_types_graph, allPathsBetween) {
  std::vector<size_t> v_ps = createTestPoints();
  size_t pA {v_ps[0]}, pB {v_ps[1]}, pC {v_ps[2]}, pD {v_ps[3]},
         pE {v_ps[4]}, pF {v_ps[5]}, pG {v_ps[6]}, pH {v_ps[7]};
  f2c::types::Graph g = createTestGraph();

  auto dfs = g.allPathsBetween(pB, pA);
  EXPECT_EQ(dfs.size(), 2);
  for (auto r : dfs) {
    EXPECT_EQ(r[0], pB);
    EXPECT_EQ(r.back(), pA);
  }
  EXPECT_EQ(dfs[0].size(), 4);
  EXPECT_EQ(dfs[0][0],pB);
  EXPECT_EQ(dfs[0][1],pD);
  EXPECT_EQ(dfs[0][2],pC);
  EXPECT_EQ(dfs[0][3],pA);
  EXPECT_EQ(dfs[1].size(), 4);
  EXPECT_EQ(dfs[1][0],pB);
  EXPECT_EQ(dfs[1][1],pE);
  EXPECT_EQ(dfs[1][2],pC);
  EXPECT_EQ(dfs[1][3],pA);

  auto dfs_1 = g.allPathsBetween(pB, pG);
  EXPECT_EQ(dfs_1.size(), 0);
}

TEST(fields2cover_types_graph, shortestPath) {
  std::vector<size_t> v_ps = createTestPoints();
  size_t pA {v_ps[0]}, pB {v_ps[1]}, pC {v_ps[2]}, pD {v_ps[3]},
         pE {v_ps[4]}, pF {v_ps[5]}, pG {v_ps[6]}, pH {v_ps[7]};
  f2c::types::Graph g = createTestGraph();

  auto v_AB_t = g.shortestPath(pA, pB, 1e5, true);
  EXPECT_EQ(v_AB_t, (std::vector<size_t>{pA, pB}));
  EXPECT_EQ(g.shortestPathCost(pA, pB, 1e5, true), 1);

  auto v_AB_f = g.shortestPath(pA, pB, 1e5, false);
  EXPECT_EQ(v_AB_f, (std::vector<size_t>{pA, pB}));
  EXPECT_EQ(g.shortestPathCost(pA, pB, 1e5, false), 1);

  auto v_BA_t = g.shortestPath(pB, pA, 1e5, true);
  EXPECT_EQ(v_BA_t, (std::vector<size_t>{pB, pD, pC, pA}));
  EXPECT_EQ(g.shortestPathCost(pB, pA, 1e5, true), 14);
  auto v_BA_f = g.shortestPath(pB, pA, 1e5, false);
  EXPECT_EQ(v_BA_f, (std::vector<size_t>{pB, pD, pC, pA}));
  EXPECT_EQ(g.shortestPathCost(pB, pA, 1e5, false), 14);


  g.removeEdge(pC, pD);
  auto v_BA_f2 = g.shortestPath(pB, pA, 1e5, false);
  EXPECT_EQ(v_BA_f2, (std::vector<size_t>{pB, pE, pC, pA}));
  EXPECT_EQ(g.shortestPathCost(pB, pA, 1e5, false), 112);


  g.addEdge(pA, pF, 1);
  EXPECT_EQ(g.shortestPathCost(pB, pA, 1e5, true), 105);
  auto v_BA_t2 = g.shortestPath(pB, pA, 1e5, true);
  EXPECT_EQ(v_BA_t2, (std::vector<size_t>{pB, pE, pF, pA}));


  EXPECT_EQ(g.shortestPathCost(pD, pG, 1e5, false), 1e5);
  auto v_not_con = g.shortestPath(pD, pG, 1e5, false);
  EXPECT_EQ(v_not_con, (std::vector<size_t>{}));


  auto v_not_con2 = g.shortestPath(pH, pF, 1e5, false);
  EXPECT_EQ(v_not_con2, (std::vector<size_t>{}));
  EXPECT_EQ(g.shortestPathCost(pH, pF, 1e5, false), 1e5);
}



