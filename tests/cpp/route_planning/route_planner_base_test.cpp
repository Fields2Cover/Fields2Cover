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
  F2CCells cells {F2CCell(F2CLinearRing({{0,0}, {2,0}, {2,2}, {0,2}, {0,0}}))};
  cells.addRing(0, F2CLinearRing({{.4,.4}, {.4,.6}, {.6,.6}, {.6,.4}, {.4,.4}}));
  cells.addRing(0, F2CLinearRing({{1.2,1.2}, {1.2,1.6}, {1.6,1.6}, {1.6,1.2}, {1.2,1.2}}));

  cells *= 3e1;

  f2c::hg::ConstHL const_hl;
  F2CCells no_hl = const_hl.generateHeadlandArea(cells, 1, 3);
  auto hl_swaths = const_hl.generateHeadlandSwaths(cells, 1, 3, false);

  f2c::decomp::BoustrophedonDecomp decomp;
  auto decomp_cells = decomp.decompose(no_hl, M_PI/2.0);

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
  auto decomp_cells = decomp.decompose(no_hl, M_PI/2.0);

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




