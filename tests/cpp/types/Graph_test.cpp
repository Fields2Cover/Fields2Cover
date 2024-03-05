//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types/Graph.h"

TEST(fields2cover_types_graph, addEdges) {
  size_t p1 {1}, p2 {3}, p3 {2}, p4 {4};
  f2c::types::Graph g;
  g.addDirectedEdge(p1, p4, 1);
  g.addDirectedEdge(p1, p3, 2);
  g.addEdge(p3, p2, 6);
  g.addEdge(p2, p1, 6);
  g.addEdge(p4, p1, 6);

  EXPECT_EQ(g.numNodes(), 4);
  EXPECT_EQ(g.numEdges(), 7);
}

TEST(fields2cover_types_graph, allPathsBetween) {
  size_t p1 {1}, p2 {2}, p3 {3}, p4 {4};
  std::vector<size_t> ps {p1, p2, p3, p4};

  f2c::types::Graph g;
  g.addDirectedEdge(p1, p4, 1);
  g.addDirectedEdge(p1, p3, 2);
  g.addEdge(p3, p2, 6);
  g.addEdge(p2, p1, 6);
  g.addEdge(p4, p1, 6);

  auto dfs = g.allPathsBetween(p4, p2);
  EXPECT_EQ(dfs.size(), 2);
  for (auto r : dfs) {
    EXPECT_EQ(r[0], p4);
    EXPECT_EQ(r.back(), p2);
  }

  for (auto pi : ps) {
    for (auto pj : ps) {
      auto dfs = g.allPathsBetween(pi, pj);
      EXPECT_GT(dfs.size(), 0);
      for (auto r : dfs) {
        EXPECT_EQ(r[0], pi);
        EXPECT_EQ(r.back(), pj);
      }
    }
  }
}

TEST(fields2cover_types_graph, shortestPaths) {
  size_t p1 {0}, p2 {1}, p3 {2}, p4 {3};
  std::vector<size_t> ps {p1, p2, p3, p4};

  f2c::types::Graph g;
  g.addEdge(p1, p2, 6);
  g.addDirectedEdge(p3, p4, 1);
  g.addDirectedEdge(p1, p3, 2);
  g.addEdge(p3, p2, 20);
  g.addEdge(p4, p1, 5);

  auto paths = g.shortestPathsAndCosts();

  EXPECT_EQ(paths.size(), 4);
  for (auto& path : paths) {
    EXPECT_EQ(path.size(), 4);
  }
  EXPECT_EQ(paths[0][1].first.size(), 2);
  EXPECT_EQ(paths[0][1].second, 6);
  EXPECT_EQ(paths[0][3].first.size(), 3);

  EXPECT_EQ(paths[0][3].second, 3);

  for (int i = 0; i < paths.size(); ++i) {
    EXPECT_EQ(paths[i][i].first.size(), 0);
    EXPECT_EQ(paths[i][i].second, 0);
  }
  for (int i = 0; i < paths.size(); ++i) {
    for (int j = 0; j < paths[i].size(); ++j) {
      if (i != j) {
        EXPECT_GT(paths[i][j].first.size(), 0);
        EXPECT_EQ(paths[i][j].first[0], i);
        EXPECT_EQ(paths[i][j].first.back(), j);
      }
    }
  }

  size_t p_far1 {4}, p_far2 {5};
  g.addEdge(p_far1, p_far2, 3);
  paths = g.shortestPathsAndCosts();
  EXPECT_EQ(paths.size(), 6);
  for (auto& path : paths) {
    EXPECT_EQ(path.size(), 6);
  }
  EXPECT_EQ(paths[5][4].second, 3);
  EXPECT_GT(paths[0][5].second, 1e5);
  EXPECT_GT(paths[5][0].second, 1e5);
  EXPECT_GT(paths[0][4].second, 1e5);
  EXPECT_GT(paths[4][0].second, 1e5);
}


