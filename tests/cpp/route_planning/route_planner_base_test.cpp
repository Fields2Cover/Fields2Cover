//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <random>
#include "fields2cover/types.h"
#include "fields2cover/objectives/rp_obj/direct_dist_path_obj.h"
#include "fields2cover/route_planning/route_planner_base.h"
#include "fields2cover.h"

TEST(fields2cover_rp_route_plan_base, simple_example) {
  f2c::Random rand(4);
  F2CCells cells {
    F2CCell(F2CLinearRing({
          F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)
    }))
  };
  cells.addRing(0, F2CLinearRing({
          F2CPoint(.4,.4), F2CPoint(.4,.6),F2CPoint(.6,.6),F2CPoint(.6,.4), F2CPoint(.4,.4)
  }));
  cells.addRing(0, F2CLinearRing({
          F2CPoint(1.2,1.2), F2CPoint(1.2,1.6),F2CPoint(1.6,1.6),F2CPoint(1.6,1.2), F2CPoint(1.2,1.2)
  }));

  cells *= 3e1;

  f2c::hg::ConstHL const_hl;
  F2CCells no_hl = const_hl.generateHeadlandArea(cells, 1, 3);
  auto hl_swaths = const_hl.generateHeadlandSwaths(cells, 1, 3, false);

  f2c::decomp::BoustrophedonDecomp decomp;
  decomp.setSplitAngle(M_PI/2.0);
  auto decomp_cells = decomp.decompose(no_hl);

  f2c::sg::BruteForce bf;
  F2CSwathsByCells swaths = bf.generateSwaths(M_PI/2.0, 5, decomp_cells);

  f2c::rp::RoutePlannerBase route_planner;
  F2CRoute route = route_planner.genRoute(hl_swaths[1], swaths);

  EXPECT_FALSE(route.isEmpty());
  EXPECT_GT(route.sizeVectorSwaths(), 1);
  EXPECT_EQ(route.sizeVectorSwaths(), route.sizeConnections());

  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  //f2c::Visualizer::plot(no_hl);
  //f2c::Visualizer::plot(hl_swaths[1]);
  //f2c::Visualizer::plot(decomp_cells);
  //for (auto&& c : route.getConnections())
  f2c::Visualizer::plot(route);
  f2c::Visualizer::show();
  */
}

TEST(fields2cover_rp_route_plan_base, redirect_flag) {
  f2c::Random rand(4);
  F2CCells cells {
    F2CCell(F2CLinearRing({
          F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)
    }))
  };
  cells.addRing(0, F2CLinearRing({
          F2CPoint(.4,.4), F2CPoint(.4,.6),F2CPoint(.6,.6),F2CPoint(.6,.4), F2CPoint(.4,.4)
  }));
  cells.addRing(0, F2CLinearRing({
          F2CPoint(1.2,1.2), F2CPoint(1.2,1.6),F2CPoint(1.6,1.6),F2CPoint(1.6,1.2), F2CPoint(1.2,1.2)
  }));

  cells *= 3e1;

  f2c::hg::ConstHL const_hl;
  F2CCells no_hl = const_hl.generateHeadlandArea(cells, 1, 3);
  auto hl_swaths = const_hl.generateHeadlandSwaths(cells, 1, 3, false);

  f2c::decomp::BoustrophedonDecomp decomp;
  decomp.setSplitAngle(M_PI/2.0);
  auto decomp_cells = decomp.decompose(no_hl);

  f2c::sg::BruteForce bf;
  F2CSwathsByCells swaths = bf.generateSwaths(M_PI/2.0, 5, decomp_cells);

  f2c::rp::RoutePlannerBase route_planner;
  F2CRoute route = route_planner.genRoute(hl_swaths[1], swaths, false, 1e-4, false);
  
  F2CSwaths old_swaths = swaths.flatten();
  F2CSwaths new_swaths;
  for (size_t sbc = 0; sbc < route.sizeVectorSwaths(); ++sbc) {
    new_swaths.append(route.getSwaths(sbc));
  }

  EXPECT_EQ(new_swaths.size(), old_swaths.size());
  
  for (size_t s = 0; s < new_swaths.size(); ++s) {
    F2CSwath old_swath = old_swaths.at(s);
    F2CSwath new_swath = new_swaths.at(s);
    EXPECT_TRUE(new_swath.hasSameDir(old_swath));
  }

  EXPECT_FALSE(route.isEmpty());
  EXPECT_GT(route.sizeVectorSwaths(), 1);
  EXPECT_EQ(route.sizeVectorSwaths(), route.sizeConnections());

  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  //f2c::Visualizer::plot(no_hl);
  //f2c::Visualizer::plot(hl_swaths[1]);
  //f2c::Visualizer::plot(decomp_cells);
  //for (auto&& c : route.getConnections())
  f2c::Visualizer::plot(route);
  f2c::Visualizer::show();
  */
}




TEST(fields2cover_rp_route_plan_base, shortestGraphDoesNotLeaveTheCells) {
  F2CCell cell_a {F2CLinearRing({
      F2CPoint(0,0), F2CPoint(10,0), F2CPoint(10,10), F2CPoint(0,10), F2CPoint(0,0)})};
  F2CCell cell_b {F2CLinearRing({
      F2CPoint(20,0), F2CPoint(30,0), F2CPoint(30,10), F2CPoint(20,10), F2CPoint(20,0)})};

  F2CSwath swath_a {F2CLineString({F2CPoint(2,5), F2CPoint(8,5)}), 1};
  F2CSwath swath_b {F2CLineString({F2CPoint(22,5), F2CPoint(28,5)}), 1};
  F2CSwaths swaths;
  swaths.emplace_back(swath_a);
  swaths.emplace_back(swath_b);
  F2CSwathsByCells swaths_by_cells;
  swaths_by_cells.emplace_back(swaths);

  // Only cell_a is given, so swath_b is outside every cell of the graph.
  F2CCells cells_a {cell_a};
  f2c::rp::RoutePlannerBase route_planner;
  F2CGraph2D g = route_planner.createShortestGraph(cells_a, swaths_by_cells, 1e-4);

  EXPECT_TRUE(g.shortestPath(swath_b.startPoint(), swath_a.startPoint()).empty());
  EXPECT_FALSE(g.shortestPath(swath_a.startPoint(), swath_a.endPoint()).empty());
}
