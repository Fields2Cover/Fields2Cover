//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "fields2cover/utils/random.h"
#include "fields2cover/types.h"
#include "fields2cover/objectives/sg_obj/n_swath.h"
#include "fields2cover/objectives/sg_obj/n_swath_modified.h"
#include "fields2cover/swath_generator/brute_force.h"
#include "fields2cover/objectives/sg_obj/n_swath_modified.h"
#include "fields2cover/headland_generator/constant_headland.h"
#include "fields2cover/headland_generator/corridor_headland.h"
#include "fields2cover/decomposition/trapezoidal_decomp.h"
#include "../test_helpers/swath_generator_checker.hpp"


TEST(fields2cover_swath_brute_gen, generate_swaths) {
  f2c::Random rand(4);
  f2c::sg::BruteForce sw_gen;
  auto poly = rand.genConvexCell(1e3);

  for (int i = 0; i < 10; ++i) {
    double ang = rand.getAngleRandom();
    auto swaths = sw_gen.generateSwaths(ang, 1, poly);
    for (auto&& s : swaths) {
      EXPECT_NEAR(s.getInAngle(), ang, 1e-3);
      EXPECT_NEAR(s.getOutAngle(), ang, 1e-3);
    }
  }
}

TEST(fields2cover_swath_brute_gen, best_angle) {
  f2c::Random rand(4);
  f2c::obj::NSwath n_swath;
  f2c::sg::BruteForce sw_gen;

  sw_gen.setStepAngle(1);
  EXPECT_EQ(sw_gen.getStepAngle(), 1.0);
  auto poly = rand.genConvexCell(1e3);
  isSwathGenerationCorrect(sw_gen, n_swath, poly);
  isSwathGenerationCorrect(sw_gen, n_swath, F2CCells(poly));
  isSwathGenerationCorrect(sw_gen, n_swath, F2CCell());
  isSwathGenerationCorrect(sw_gen, n_swath, F2CCells());
}

TEST(fields2cover_swath_brute_gen, bestAngleCoversEveryCoverableCell) {
  // Carving a corridor can leave a cell with a hairline spur. The cheapest
  // angle by cost then covers nothing at all, even though other angles fill
  // the cell, and the cell is silently skipped.
  F2CCell field(F2CLinearRing({
      F2CPoint(0, 0), F2CPoint(120, 0), F2CPoint(120, 90),
      F2CPoint(0, 90), F2CPoint(0, 0)}));
  field.addRing(F2CLinearRing({
      F2CPoint(45, 35), F2CPoint(75, 35), F2CPoint(75, 60),
      F2CPoint(45, 60), F2CPoint(45, 35)}));

  f2c::decomp::TrapezoidalDecomp decomp;
  decomp.setSplitAngle(0.0);
  F2CCells cells = f2c::hg::CorridorHL().generateHeadlands(
      decomp.decompose(f2c::hg::ConstHL().generateHeadlands(F2CCells(field), 3.75)),
      2.5);

  f2c::sg::BruteForce sw_gen;
  f2c::obj::NSwathModified obj;
  for (size_t i = 0; i < cells.size(); ++i) {
    EXPECT_GT(sw_gen.generateBestSwaths(obj, 2.5, cells.getGeometry(i)).size(), 0)
        << "cell " << i << " of area " << cells.getGeometry(i).area()
        << " got no swaths";
  }
}

TEST(fields2cover_swath_brute_gen, bestSwathsCoverACellWithAHairlineSpur) {
  // Build the shape a boolean difference leaves behind: a cell whose outline
  // can run down one edge and back up, enclosing no area. On such a cell the
  // cheapest angle by cost can cover nothing, because the objectives score an
  // angle from the border alone -- and the cell is then skipped without a word.
  F2CCell field(F2CLinearRing({
      F2CPoint(0, 0), F2CPoint(120, 0), F2CPoint(120, 90),
      F2CPoint(0, 90), F2CPoint(0, 0)}));
  field.addRing(F2CLinearRing({
      F2CPoint(45, 35), F2CPoint(75, 35), F2CPoint(75, 60),
      F2CPoint(45, 60), F2CPoint(45, 35)}));
  f2c::decomp::TrapezoidalDecomp decomp;
  decomp.setSplitAngle(0.0);
  F2CCells cells = decomp.decompose(
      f2c::hg::ConstHL().generateHeadlands(F2CCells(field), 3.75));

  F2CCells cell {cells.getGeometry(1)};
  const F2CLinearRing ring = cells.getCellBorder(1);
  for (size_t k = 0; k < cells.size(); ++k) {
    if (k == 1 || ring.length() > cells.getCellBorder(k).length()) {
      continue;
    }
    const F2CCells neighbour = F2CCells::buffer(cells.getGeometry(k), 1e-3);
    for (size_t e = 0; e + 1 < ring.size(); ++e) {
      F2CMultiLineString edge;
      edge.addGeometry(
          F2CLineString({ring.getGeometry(e), ring.getGeometry(e + 1)}));
      const F2CMultiLineString shared = edge.intersection(neighbour);
      for (size_t j = 0; j < shared.size(); ++j) {
        const F2CLineString part = shared.getGeometry(j);
        if (part.size() > 1 && part.length() > 1e-3) {
          cell = cell.difference(F2CCells::buffer(part, 2.5));
        }
      }
    }
  }

  const F2CCell spiky = cell.getGeometry(0);
  f2c::sg::BruteForce sw_gen;
  f2c::obj::NSwathModified obj;
  // Whether the difference actually leaves the spur depends on the GEOS build,
  // so do not require it. Either way a cell of this size has to come back
  // covered: the cheapest angle producing nothing must not end the search.
  EXPECT_GT(sw_gen.generateBestSwaths(obj, 2.5, spiky).size(), 0)
      << "cell of area " << spiky.area() << " got no swaths";
}
