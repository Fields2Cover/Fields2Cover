//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <memory>
#include <utility>
#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/route_planning/route_generator_base.h"
#include "fields2cover/route_planning/boustrophedon_order.h"
#include "fields2cover/route_planning/snake_order.h"
#include "fields2cover/route_planning/spiral_order.h"
#include "fields2cover/route_planning/custom_order.h"
#include "fields2cover/route_planning/route_planner_base.h"

namespace {

F2CCells genCell() {
  return F2CCells(F2CCell(F2CLinearRing({
        F2CPoint(0, 0), F2CPoint(60, 0), F2CPoint(60, 40), F2CPoint(0, 40),
        F2CPoint(0, 0)})));
}

F2CSwathsByCells genSwaths(const F2CCells& cell) {
  F2CSwaths swaths;
  for (int x = 5; x < 60; x += 10) {
    swaths.append(F2CLineString({F2CPoint(x, 1), F2CPoint(x, 39)}), cell, 10);
  }
  F2CSwathsByCells sbc;
  sbc.emplace_back(swaths);
  return sbc;
}

// Two routes are the same when they draw the same line.
void expectSameRoute(const F2CRoute& a, const F2CRoute& b) {
  ASSERT_EQ(a.sizeVectorSwaths(), b.sizeVectorSwaths());
  ASSERT_EQ(a.sizeConnections(), b.sizeConnections());
  F2CLineString la = a.asLineString();
  F2CLineString lb = b.asLineString();
  ASSERT_EQ(la.size(), lb.size());
  for (size_t i = 0; i < la.size(); ++i) {
    EXPECT_EQ(la.getGeometry(i), lb.getGeometry(i));
  }
}

}  // namespace

TEST(fields2cover_rp_route_generator_base, everyPlannerAnswersTheSameCall) {
  F2CCells cell = genCell();
  F2CSwathsByCells sbc = genSwaths(cell);
  const size_t n_swaths = sbc.flatten().size();
  ASSERT_GT(n_swaths, 1);

  std::vector<size_t> order;
  for (size_t i = 0; i < n_swaths; ++i) { order.emplace_back(i); }

  std::vector<std::unique_ptr<f2c::rp::RouteGeneratorBase>> planners;
  planners.emplace_back(std::make_unique<f2c::rp::BoustrophedonOrder>());
  planners.emplace_back(std::make_unique<f2c::rp::SnakeOrder>());
  planners.emplace_back(std::make_unique<f2c::rp::SpiralOrder>(2));
  planners.emplace_back(std::make_unique<f2c::rp::CustomOrder>(order));
  planners.emplace_back(std::make_unique<f2c::rp::RoutePlannerBase>());

  for (auto&& planner : planners) {
    F2CRoute route = planner->genRoute(cell, sbc);
    EXPECT_FALSE(route.isEmpty());
    EXPECT_EQ(route.sizeVectorSwaths(), route.sizeConnections());
  }
}

TEST(fields2cover_rp_route_generator_base, orderersRouteTheSameThroughTheBase) {
  F2CCells cell = genCell();
  F2CSwathsByCells sbc = genSwaths(cell);

  f2c::rp::SnakeOrder sorter;
  F2CRoute direct = sorter.genRoute(cell, sbc);

  const f2c::rp::RouteGeneratorBase& base = sorter;
  F2CRoute polymorphic = base.genRoute(cell, sbc);

  expectSameRoute(direct, polymorphic);
}

TEST(fields2cover_rp_route_generator_base, tspRoutesTheSameThroughTheBase) {
  F2CCells cell = genCell();
  F2CSwathsByCells sbc = genSwaths(cell);

  f2c::rp::RoutePlannerBase planner;
  F2CRoute direct = planner.genRoute(cell, sbc);

  const f2c::rp::RouteGeneratorBase& base = planner;
  F2CRoute polymorphic = base.genRoute(cell, sbc);

  expectSameRoute(direct, polymorphic);
}

namespace {

bool derived_destructor_ran = false;

class DestructorProbe : public f2c::rp::SnakeOrder {
 public:
  ~DestructorProbe() override { derived_destructor_ran = true; }
};

}  // namespace

TEST(fields2cover_rp_route_generator_base, deletingThroughTheBaseIsSafe) {
  derived_destructor_ran = false;
  f2c::rp::RouteGeneratorBase* planner = new DestructorProbe();
  delete planner;
  EXPECT_TRUE(derived_destructor_ran);
}

TEST(fields2cover_rp_route_generator_base, constPlannersRoute) {
  F2CCells cell = genCell();
  F2CSwathsByCells sbc = genSwaths(cell);

  const f2c::rp::SnakeOrder sorter;
  const f2c::rp::RoutePlannerBase planner;

  F2CRoute sorter_route = sorter.genRoute(cell, sbc);
  F2CRoute planner_route = planner.genRoute(cell, sbc);

  EXPECT_FALSE(sorter_route.isEmpty());
  EXPECT_FALSE(planner_route.isEmpty());

  f2c::rp::SnakeOrder mut_sorter;
  f2c::rp::RoutePlannerBase mut_planner;
  expectSameRoute(sorter_route, mut_sorter.genRoute(cell, sbc));
  expectSameRoute(planner_route, mut_planner.genRoute(cell, sbc));
}
