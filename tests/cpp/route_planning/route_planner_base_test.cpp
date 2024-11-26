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


TEST(fields2cover_rp_route_plan_base, start_and_end_points) {
  F2CCells cells {
    F2CCell(F2CLinearRing({
          F2CPoint(0,0), F2CPoint(60,0),F2CPoint(60,60),F2CPoint(0,60), F2CPoint(0,0)
    }))
  };
  cells.addRing(0, F2CLinearRing({
          F2CPoint(12,12), F2CPoint(12,18),F2CPoint(18,18),F2CPoint(18,12), F2CPoint(12,12)
  }));
  cells.addRing(0, F2CLinearRing({
          F2CPoint(36,36), F2CPoint(36,48),F2CPoint(48,48),F2CPoint(48,36), F2CPoint(36,36)
  }));

  f2c::hg::ConstHL const_hl;
  F2CCells no_hl = const_hl.generateHeadlandArea(cells, 1, 3);
  auto hl_swaths = const_hl.generateHeadlandSwaths(cells, 1, 3, false);

  f2c::decomp::BoustrophedonDecomp decomp;
  decomp.setSplitAngle(M_PI/2.0);
  auto decomp_cells = decomp.decompose(no_hl);

  f2c::sg::BruteForce bf;
  F2CSwathsByCells swaths = bf.generateSwaths(M_PI/2.0, 5, decomp_cells);

  f2c::rp::RoutePlannerBase route_planner;

  EXPECT_GT(swaths.size(), 0);
  EXPECT_GT(swaths[0].size(), 0);
  F2CPoint start = swaths[0][0].startPoint();
  F2CPoint end = swaths[0].back().startPoint();

  route_planner.setStartAndEndPoint(start, end);

  F2CRoute route = route_planner.genRoute(hl_swaths[1], swaths);

  EXPECT_FALSE(route.isEmpty());
  EXPECT_EQ(route.startPoint(), start);
  EXPECT_EQ(route.endPoint(), end);
}



