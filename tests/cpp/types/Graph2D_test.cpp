//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_graph2d, addEdges) {
  F2CPoint p1 {1, -1}, p2 {2, -2}, p3 {-3, 3}, p4 {5, 5};
  F2CGraph2D g;
  g.addDirectedEdge(p1, p4, 1);
  g.addDirectedEdge(p1, p3, 2);
  g.addEdge(p3, p2, 6);
  g.addEdge(p2, p1, 6);
  g.addEdge(p4, p1, 6);

  EXPECT_EQ(g.numNodes(), 4);
  EXPECT_EQ(g.numEdges(), 7);
}

TEST(fields2cover_types_graph2d, allPathsBetween) {
  F2CPoint p1 {1, -1}, p2 {2, -2}, p3 {-3, 3}, p4 {5, 5};
  std::vector<F2CPoint> ps {p1, p2, p3, p4};

  F2CGraph2D g;
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

TEST(fields2cover_types_graph2d, shortestPaths) {
  F2CPoint p1 {1, -1}, p2 {2, -2}, p3 {-3, 3}, p4 {5, 5};
  std::vector<F2CPoint> ps {p1, p2, p3, p4};

  F2CGraph2D g;
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

  F2CPoint p_far1 {10, 10}, p_far2 {11, 12};
  g.addEdge(p_far1, p_far2);
  paths = g.shortestPathsAndCosts();
  EXPECT_EQ(paths.size(), 6);
  for (auto& path : paths) {
    EXPECT_EQ(path.size(), 6);
  }
  EXPECT_EQ(paths[5][4].second, 2236);
  EXPECT_GT(paths[0][5].second, 1e5);
  EXPECT_GT(paths[5][0].second, 1e5);
  EXPECT_GT(paths[0][4].second, 1e5);
  EXPECT_GT(paths[4][0].second, 1e5);
}

TEST(fields2cover_types_graph2d, shortestPaths_only_swaths) {
  F2CPoint a {2,4}, b {1,3}, c {1,2}, d {2,1},
      e {3,2}, f{3,3},ai {2, 3.5},  bs {1.5,3},
      cs {1.5, 2}, di {2, 1.5}, es {2.5, 2},
      fs {2.5,3}, is1{1.75, 2.5}, is2 {2.25, 2.5};

  F2CGraph2D g;
  // add external hexagon points
  g.addEdge(a, b, 1);  // #0 #1
  g.addEdge(b, c, 2);  // #1 #2
  g.addEdge(c, d, 4);
  g.addEdge(d, e, 8);
  g.addEdge(e, f, 16);
  g.addEdge(f, a, 32);  // #5 #0

  // add internal hexagon points
  g.addEdge(ai, bs, 3); // #6 #7
  g.addEdge(bs, cs, 9);
  g.addEdge(cs, di, 27); // #8 #9
  g.addEdge(di, es, 81);
  g.addEdge(es, fs, 243);
  g.addEdge(fs, ai, 729);  //#11 #0

  // add swaths
  g.addSwathEdge(b, bs, 10'000);  // #1 #7
  g.addSwathEdge(es, e, 20'000); // #10 #4

  // add unreachable edge
  g.addEdge(is1, is2, 8'000); // #12 #13

  g.onlyPathsOfSwaths(false);
  auto paths_all = g.shortestPathsAndCosts();

  g.onlyPathsOfSwaths(true);
  auto paths_only_swaths = g.shortestPathsAndCosts();


  EXPECT_EQ(paths_only_swaths.size(), 14);
  EXPECT_EQ(paths_all.size(), 14);

  uint16_t memory_of_paths = 0;
  uint16_t memory_of_only_swaths = 0;

  for (int i = 0; i < paths_only_swaths.size(); ++i) {
    for (int j = 0; j < paths_only_swaths.size(); ++j) {
      if (i==j) {
        EXPECT_EQ(paths_only_swaths[i][i].second,0);
        EXPECT_EQ(paths_only_swaths[i][i].first.size(),0);
      }
      size_t full = paths_all[i][j].first.size();
      size_t osw = paths_only_swaths[i][j].first.size();
      memory_of_paths += full;
      memory_of_only_swaths += osw;
      // looping through all the paths we expect sometimes to find
      // a smaller length of paths, never higher
      EXPECT_LE(osw,full);
      // if the path is shorter it must be 0-length
      if (osw<full) {
        EXPECT_EQ(osw, 0);
      } else { // else we expect to find the same nodes list
        for (int m = 0; m < full; ++m) {
          EXPECT_EQ(paths_all[i][j].first[m], paths_only_swaths[i][j].first[m]);
        }
      }
    }
  }
  // overall we expect at least one to be less, which means strictly smaller
  EXPECT_LT(memory_of_only_swaths,memory_of_paths);

  // check paths of swaths extremities
  EXPECT_EQ(paths_only_swaths[1][4].second, 2+4+8); // b -> e
  EXPECT_EQ(paths_only_swaths[1][4].second, paths_all[1][4].second); // b -> e

  EXPECT_EQ(paths_only_swaths[1][10].second, 10'000+9+27+81); // b -> es
  EXPECT_EQ(paths_only_swaths[1][10].second, paths_all[1][10].second); // b -> es
  EXPECT_EQ(paths_only_swaths[1][10], paths_all[1][10]); // b -> es


  // never calculated -> can't check the cost, only the path length
  EXPECT_EQ(paths_only_swaths[13][1].first.size(), 0); // a <-> is2
  EXPECT_EQ(paths_only_swaths[13][12].first.size(), 0); //is2 <-> is1


}
