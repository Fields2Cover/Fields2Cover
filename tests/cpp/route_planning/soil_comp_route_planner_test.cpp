//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <random>
#include "fields2cover/types.h"
#include "fields2cover/route_planning/soil_comp_route_planner.h"
#include "fields2cover.h"

TEST(fields2cover_rp_soil_comp_route_plan, simple_example) {
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
  auto swaths = bf.generateSwaths(M_PI/2.0, {2, 5, 3, 5}, decomp_cells);

  f2c::rp::SoilCompRoutePlanner route_planner;
  /*
  auto routes = route_planner.genRoute(hl_swaths[1], swaths);

  EXPECT_FALSE(routes[0].isEmpty());
  EXPECT_GT(routes[0].sizeVectorSwaths(), 1);
  EXPECT_EQ(routes[0].sizeVectorSwaths(), routes[0].sizeConnections());

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  //f2c::Visualizer::plot(swaths);
  //f2c::Visualizer::plot(no_hl);
  //f2c::Visualizer::plot(hl_swaths[1]);
  //f2c::Visualizer::plot(decomp_cells);
  //for (auto&& c : route.getConnections())
  f2c::Visualizer::plot(routes);
  f2c::Visualizer::show();
  */

  std::cerr<<"Swaths size: " << swaths[0].sizeTotal()<<std::endl;
  std::cerr<<"Swaths size: " << swaths[1].sizeTotal()<<std::endl;
}

TEST(DISABLED_fields2cover_rp_soil_comp_route_plan, simple_example2) {
  f2c::Random rand(4);
  F2CRobot robot (2.0, 2.0);
  robot.setMinTurningRadius(2);  // m
  robot.setMaxDiffCurv(1);  // 1/m^2

  f2c::hg::ConstHL const_hl;

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

  cells *= 5e1;

  F2CCells no_hl = const_hl.generateHeadlandArea(cells, robot.getWidth(), 3);
  auto hl_swaths = const_hl.generateHeadlandSwaths(cells, robot.getWidth(), 3, false);

  f2c::decomp::BoustrophedonDecomp decomp;
  auto decomp_cells = decomp.decompose(no_hl, M_PI/2.0);

  f2c::sg::BruteForce bf;
  F2CSwathsByCells swaths = bf.generateSwaths(M_PI/2.0, robot.getCovWidth(), decomp_cells);

  f2c::rp::SoilCompRoutePlanner route_planner;
  F2CRoute route = route_planner.genRoute(hl_swaths[1], swaths);

  EXPECT_FALSE(route.isEmpty());
  EXPECT_GT(route.sizeVectorSwaths(), 1);
  EXPECT_EQ(route.sizeVectorSwaths(), route.sizeConnections());

  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurves dubins;
  F2CPath path_dubins = path_planner.planPath(robot, route, dubins);


  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  f2c::Visualizer::plot(decomp_cells);
  //f2c::Visualizer::plot(route);
  //f2c::Visualizer::plot(swaths);
  f2c::Visualizer::plot(path_dubins);
  f2c::Visualizer::show();
  */
}

TEST(DISABLED_fields2cover_rp_soil_comp_route_plan, complete_example) {
  f2c::Random rand(45);
  F2CRobot robot (2.0, 2.0);
  robot.setMinTurningRadius(2.0);  // m
  robot.setMaxDiffCurv(0.5);  // 1/m^2

  f2c::hg::ConstHL const_hl;
  auto non_convex_field = rand.genNonConvexCell(1e4);
  auto object1 = rand.genConvexCell(1e2) + F2CPoint(0,30);
  auto object2 = rand.genConvexCell(3e2) + F2CPoint(13,-10);
  auto object3 = rand.genConvexCell(2e2) + F2CPoint(-15,0);
  F2CCells cells = F2CCells(non_convex_field)
    .difference(object1)
    .difference(object2)
    .difference(object3);
  cells *= 1.5;

  F2CCells no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.getCovWidth());
  F2CCells mid_hl = const_hl.generateHeadlands(cells, 1.5 * robot.getCovWidth());

  f2c::decomp::BoustrophedonDecomp decomp;
  //no_hl  = decomp.decompose(no_hl, M_PI/2.0);

  f2c::sg::BruteForce bf;
  f2c::obj::SwathLength sg_obj;
  auto w = robot.getCovWidth();
  auto swaths = bf.generateSwaths(M_PI/2.0, {w, w, w, w} , no_hl);

  f2c::obj::DirectDistPathObj obj;
  //f2c::rp::RoutePlannerBase route_planner;
  f2c::rp::SoilCompRoutePlanner route_planner;
  auto route = route_planner.genRoute(mid_hl, swaths, false);

  //f2c::pp::PathPlanning path_planner;
  //f2c::pp::DubinsCurves dubins;
  //F2CPath path_dubins = path_planner.planPath(robot, route, dubins);


  /*
  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  //f2c::Visualizer::plot(swaths);
  f2c::Visualizer::plot(route);
  //f2c::Visualizer::plot(path_dubins);
  f2c::Visualizer::show();
  */
}



