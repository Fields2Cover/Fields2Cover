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

TEST(DISABLED_fields2cover_rp_route_plan_base, dist_mat) {
  F2CCells cells {F2CCell(F2CLinearRing({{0,0}, {30, 0}, {30, 4}, {2, 4}, {2, 6}, {1,6}, {1, 8}, {0, 8}, {0,0}}))};
  //cells.addRing(0, F2CLinearRing({{.4,.4}, {.4,.6}, {.6,.6}, {.6,.4}, {.4,.4}}));
  //cells.addRing(0, F2CLinearRing({{1.2,1.2}, {1.2,1.6}, {1.6,1.6}, {1.6,1.2}, {1.2,1.2}}));
  //cells *= 6;


  F2CRobot robot(1.0);
  /*
  f2c::types::RobotCapacity cap1;
  cap1.name = "cap A";
  cap1.reload_points.emplace_back(5, 0);
  //cap1.reload_points.emplace_back(40, 0);
  robot.addCapacity(cap1);
  */
  f2c::types::RobotCapacity cap2;
  cap2.name = "cap_B";
  cap2.reload_points.emplace_back(0, 4);
  cap2.type = f2c::types::RobotCapacityType::BY_SWATH_LENGTH;
  cap2.rate = 1;
  cap2.max_capacity = 10e3;
  robot.addCapacity(cap2);

  f2c::sg::BruteForce bf;
  F2CSwathsByCells swaths = bf.generateSwaths(M_PI/2.0, robot.getCovWidth(), cells);


  f2c::rp::RoutePlannerBase route_planner;
  route_planner.setStartPoint(F2CPoint(0, 0));
  route_planner.setEndPoint(F2CPoint(3, 0));

  F2CGraph2D shortest_graph = route_planner.createShortestGraph(cells.getLineSections(), swaths, robot);
  F2CGraph2D cov_graph = route_planner.createCoverageGraph(swaths, robot, shortest_graph);
  auto dist_prev = route_planner.distMatrix(swaths, robot, cov_graph);
  //auto dist = dist_prev;
  auto dist = route_planner.distMatrix2(dist_prev, swaths.sizeTotal());


  /*
  for (int i = 0; i < dist.size(); ++i) {
    std::cerr << "MAT at DIM: " << i << std::endl;
    for (int j = 0; j < dist[i].size(); ++j) {
      for (int k = 0; k < dist[i][j].size(); ++k) {
        if (dist[i][j][k] > 1e7) {
          std::cerr << "-" <<", ";
        } else {
          std::cerr << double(dist[i][j][k]) / 1e3 <<", ";
        }
      }
      std::cerr << std::endl;
      std::cerr << std::endl;
    }
    std::cerr << std::endl << std::endl<< std::endl;
    std::cerr << std::endl << std::endl<< std::endl;
  }
  */

  auto ids = route_planner.computeBestRoute2(dist, swaths, robot, true);


  /*
  std::cerr << std::endl << std::endl<< std::endl;
  std::cerr << "  RUTA   "<< std::endl;
  for (auto id : ids) {
    std::cerr << id<<"  ";
    if (id < 2) {
    } else if (id < 2 + 2 * swaths.sizeTotal()) {
      std::cerr << "Swath "<<(id-2)/2<<",  "<< std::endl;
    } else {
      std::cerr << "Recharge "<<(id-2 - 2 * swaths.sizeTotal())/100<<",  "<< std::endl;
    }
  }
  std::cerr << std::endl << std::endl<< std::endl;
  */

  /*
  for (int i = 0; i < ids.size()-2; ++i) {
    if (((ids[i]-2)/3 == (ids[i+1]-2)/3) && ((ids[i]-2)/3 == (ids[i+2]-2)/3)) {
      std::cerr << "Swath "<<(ids[i]-2)/3<<"  "<< std::endl;
      ++(++i);
    } else {
      std::cerr << ":( "<<(ids[i]-2)/3<<"  "<< "  Swath  " << ids[i]<< std::endl;
    }

  }
  std::cerr << std::endl << std::endl<< std::endl;
  */


  //auto route = route_planner.transformSolutionToRoute2(ids, swaths, cov_graph, shortest_graph);
  F2CRoute route = route_planner.genRoute2(cells.getLineSections(), swaths, robot);

  f2c::Visualizer::show_img(cells, route);

  /*
  F2CRoute route = route_planner.genRoute(hl_swaths[1], swaths);

  EXPECT_FALSE(route.isEmpty());
  EXPECT_GT(route.sizeVectorSwaths(), 1);
  EXPECT_EQ(route.sizeVectorSwaths() + 1, route.sizeConnections());
  */

  /*
  f2c::Visualizer::figure();
  //f2c::Visualizer::plot(cells);
  //f2c::Visualizer::plot(swaths);
  //f2c::Visualizer::plot(no_hl);
  //f2c::Visualizer::plot(hl_swaths[1]);
  //f2c::Visualizer::plot(decomp_cells);
  //for (auto&& c : route.getConnections())
  f2c::Visualizer::plot(route);
  f2c::Visualizer::show();
  */
}

TEST(fields2cover_rp_route_plan_base, capacitated_example) {
  F2CCells cells {F2CCell(F2CLinearRing({{0,0}, {2,0}, {2,2}, {0,2}, {0,0}}))};
  cells.addRing(0, F2CLinearRing({{.4,.4}, {.4,.6}, {.6,.6}, {.6,.4}, {.4,.4}}));
  cells.addRing(0, F2CLinearRing({{1.2,1.2}, {1.2,1.6}, {1.6,1.6}, {1.6,1.2}, {1.2,1.2}}));

  cells *= 3e1;

  F2CRobot robot(3.0);


  f2c::hg::ConstHL const_hl;
  F2CCells no_hl = const_hl.generateHeadlandArea(cells, 1, 3);
  auto hl_swaths = const_hl.generateHeadlandSwaths(cells, 1, 3, false);

  f2c::decomp::BoustrophedonDecomp decomp;
  auto decomp_cells = decomp.decompose(no_hl, M_PI/2.0);

  f2c::sg::BruteForce bf;
  F2CSwathsByCells swaths = bf.generateSwaths(M_PI/2.0, 5, decomp_cells);

  f2c::rp::RoutePlannerBase route_planner;
  route_planner.setStartPoint(F2CPoint(0,0));
  route_planner.setEndPoint(F2CPoint(60,60));
  F2CRoute route = route_planner.genRoute(hl_swaths[1], swaths);

  EXPECT_FALSE(route.isEmpty());
  EXPECT_GT(route.sizeVectorSwaths(), 1);
  EXPECT_EQ(route.sizeVectorSwaths() + 1, route.sizeConnections());

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
  route_planner.setStartPoint(F2CPoint(0,0));
  route_planner.setEndPoint(F2CPoint(60,0));
  F2CRoute route = route_planner.genRoute(hl_swaths[1], swaths);

  EXPECT_FALSE(route.isEmpty());
  EXPECT_GT(route.sizeVectorSwaths(), 1);
  EXPECT_EQ(route.sizeVectorSwaths() + 1, route.sizeConnections());

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
  */
}

TEST(DISABLED_fields2cover_rp_route_plan_base, complete_example) {
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

  //f2c::decomp::BoustrophedonDecomp decomp;
  //decomp.setSplitAngle(M_PI/2.0);
  //no_hl  = decomp.decompose(no_hl);

  f2c::sg::BruteForce bf;
  f2c::obj::SwathLength sg_obj;
  F2CSwathsByCells swaths = bf.generateSwaths(M_PI/2.0, robot.getCovWidth(), no_hl);

  f2c::obj::DirectDistPathObj obj;
  f2c::rp::RoutePlannerBase route_planner;
  F2CRoute route = route_planner.genRoute(mid_hl, swaths, true);

  EXPECT_FALSE(route.isEmpty());
  EXPECT_GT(route.sizeVectorSwaths(), 1);
  EXPECT_EQ(route.sizeVectorSwaths(), route.sizeConnections());

  f2c::pp::PathPlanning path_planner;
  f2c::pp::DubinsCurves dubins;
  F2CPath path_dubins = path_planner.planPath(robot, route, dubins);


  f2c::Visualizer::figure();
  f2c::Visualizer::plot(cells);
  f2c::Visualizer::plot(no_hl);
  //f2c::Visualizer::plot(swaths);
  //f2c::Visualizer::plot(route);
  f2c::Visualizer::plot(path_dubins);
  f2c::Visualizer::show();
  /*
  */
}




