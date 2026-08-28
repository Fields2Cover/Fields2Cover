//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <cmath>
#include "fields2cover/types.h"
#include "fields2cover/decomposition/trapezoidal_decomp.h"
#include "fields2cover/headland_generator/constant_headland.h"
#include "fields2cover/headland_generator/corridor_headland.h"

TEST(fields2cover_hl_corridor_gen, onlyBetweenTouchingCells) {
  f2c::hg::CorridorHL corridor;
  // Two 50x100 cells sharing the x=50 border: a 2 m corridor over a 100 m
  // border removes 200 m2, and both cells stay in one piece.
  F2CCells cells;
  cells.addGeometry(F2CCell(F2CLinearRing({
      F2CPoint(0,0), F2CPoint(50,0), F2CPoint(50,100), F2CPoint(0,100), F2CPoint(0,0)})));
  cells.addGeometry(F2CCell(F2CLinearRing({
      F2CPoint(50,0), F2CPoint(100,0), F2CPoint(100,100),
      F2CPoint(50,100), F2CPoint(50,0)})));

  // Both cells are the same size, so they share the corridor evenly.
  F2CCells carved = corridor.generateHeadlands(cells, 2.0);
  EXPECT_EQ(carved.size(), 2);
  EXPECT_NEAR(cells.area() - carved.area(), 200, 1e-3);
  EXPECT_NEAR(carved.getGeometry(0).area(), 49 * 100, 1e-3);
  EXPECT_NEAR(carved.getGeometry(1).area(), 49 * 100, 1e-3);
}

TEST(fields2cover_hl_corridor_gen, leavesLoneCellsAlone) {
  f2c::hg::CorridorHL corridor;
  F2CCell left {F2CLinearRing({
      F2CPoint(0,0), F2CPoint(50,0), F2CPoint(50,100), F2CPoint(0,100), F2CPoint(0,0)})};

  // Cells across a gap wider than the corridor share no border.
  F2CCells apart;
  apart.addGeometry(left);
  apart.addGeometry(F2CCell(F2CLinearRing({
      F2CPoint(60,0), F2CPoint(110,0), F2CPoint(110,100),
      F2CPoint(60,100), F2CPoint(60,0)})));
  EXPECT_NEAR(corridor.generateHeadlands(apart, 2.0).area(), apart.area(), 1e-3);

  // A single cell has no neighbour at all.
  F2CCells one {left};
  EXPECT_NEAR(corridor.generateHeadlands(one, 2.0).area(), one.area(), 1e-3);
}

TEST(fields2cover_hl_corridor_gen, ignoresCornerContact) {
  f2c::hg::CorridorHL corridor;
  // Cells that meet at a single corner share no border, so nothing is carved.
  F2CCells cells;
  cells.addGeometry(F2CCell(F2CLinearRing({
      F2CPoint(0,0), F2CPoint(10,0), F2CPoint(10,10), F2CPoint(0,10), F2CPoint(0,0)})));
  cells.addGeometry(F2CCell(F2CLinearRing({
      F2CPoint(10,10), F2CPoint(20,10), F2CPoint(20,20),
      F2CPoint(10,20), F2CPoint(10,10)})));

  EXPECT_NEAR(corridor.generateHeadlands(cells, 2.0).area(), cells.area(), 1e-6);
}

TEST(fields2cover_hl_corridor_gen, onPartiallyTouchingEdge) {
  f2c::hg::CorridorHL corridor;
  // Only the right half of the lower cell's top edge is shared. An edge test
  // that reads a whole edge at a time misses this and carves nothing.
  F2CCells cells;
  cells.addGeometry(F2CCell(F2CLinearRing({
      F2CPoint(0,0), F2CPoint(20,0), F2CPoint(20,10), F2CPoint(0,10), F2CPoint(0,0)})));
  cells.addGeometry(F2CCell(F2CLinearRing({
      F2CPoint(10,10), F2CPoint(20,10), F2CPoint(20,20),
      F2CPoint(10,20), F2CPoint(10,10)})));

  // Only the shared part is cut: 10 m of border x 2 m = 20 m2, nothing more.
  // The smaller cell gives it up, so the larger one is untouched.
  F2CCells carved = corridor.generateHeadlands(cells, 2.0);
  EXPECT_EQ(carved.size(), 2);
  EXPECT_NEAR(cells.area() - carved.area(), 20, 1e-2);
  EXPECT_NEAR(carved.getGeometry(0).area(), 200, 1e-2);
  EXPECT_NEAR(carved.getGeometry(1).area(), 80, 1e-2);
}

TEST(fields2cover_hl_corridor_gen, keepsBothHalvesOfASplitCell) {
  f2c::hg::CorridorHL corridor;
  // The corridor cuts through the bridge of the U, leaving two arms. Both must
  // survive: dropping the extra piece would silently lose half the cell.
  F2CCells cells;
  cells.addGeometry(F2CCell(F2CLinearRing({
      F2CPoint(0,0), F2CPoint(20,0), F2CPoint(20,10), F2CPoint(13,10),
      F2CPoint(13,2), F2CPoint(7,2), F2CPoint(7,10), F2CPoint(0,10), F2CPoint(0,0)})));
  cells.addGeometry(F2CCell(F2CLinearRing({
      F2CPoint(7,2), F2CPoint(13,2), F2CPoint(13,100), F2CPoint(7,100), F2CPoint(7,2)})));

  // The U has the shorter border, so it gives up the corridor and is split.
  F2CCells carved = corridor.generateHeadlands(cells, 6.0);
  EXPECT_EQ(carved.size(), 3);
  EXPECT_NEAR(carved.getGeometry(0).area(), 22, 1e-2);
  EXPECT_NEAR(carved.getGeometry(1).area(), 22, 1e-2);
  EXPECT_NEAR(carved.getGeometry(2).area(), 588, 1e-2);
}

TEST(fields2cover_hl_corridor_gen, leavesNoHairlineSpur) {
  f2c::hg::CorridorHL corridor;
  // A boolean difference can leave a zero-width spur on the outline. It keeps
  // the area right but derails whatever reads the shape next -- the swath
  // generator picks a skewed angle -- so carving must not hand one back:
  // opening the result by a hair must not change its outline.
  F2CCell field(F2CLinearRing({
      F2CPoint(0,0), F2CPoint(120,0), F2CPoint(120,90), F2CPoint(0,90), F2CPoint(0,0)}));
  field.addRing(F2CLinearRing({
      F2CPoint(45,35), F2CPoint(75,35), F2CPoint(75,60), F2CPoint(45,60), F2CPoint(45,35)}));

  f2c::decomp::TrapezoidalDecomp decomp;
  decomp.setSplitAngle(0.0);
  F2CCells carved = corridor.generateHeadlands(decomp.decompose(
      f2c::hg::ConstHL().generateHeadlands(F2CCells(field), 3.75)), 2.5);

  for (size_t i = 0; i < carved.size(); ++i) {
    const F2CCell cell = carved.getGeometry(i);
    F2CCells opened = F2CCells(cell).buffer(-1e-9).buffer(1e-9);
    ASSERT_EQ(opened.size(), 1);
    EXPECT_EQ(opened.getGeometry(0).getGeometry(0).size(),
              cell.getGeometry(0).size()) << "cell " << i << " has a spur";
  }
}

TEST(fields2cover_hl_corridor_gen, treatsEquallySizedCellsAlike) {
  f2c::hg::CorridorHL corridor;
  // Five equal slices meeting at the centre. Their perimeters are equal in
  // theory but differ in the last few bits, so a strict comparison lets that
  // noise pick which cell gives up the corridor -- and some borders end up
  // with no corridor at all while others get a full one.
  const double r = 50.0;
  F2CCells pie;
  for (int k = 0; k < 5; ++k) {
    const double a0 = 2 * M_PI * k / 5, a1 = 2 * M_PI * (k + 1) / 5;
    pie.addGeometry(F2CCell(F2CLinearRing({
        F2CPoint(0, 0), F2CPoint(r * cos(a0), r * sin(a0)),
        F2CPoint(r * cos((a0 + a1) / 2) * 1.02, r * sin((a0 + a1) / 2) * 1.02),
        F2CPoint(r * cos(a1), r * sin(a1)), F2CPoint(0, 0)})));
  }

  F2CCells carved = corridor.generateHeadlands(pie, 4.0);
  ASSERT_EQ(carved.size(), 5);
  for (size_t i = 1; i < carved.size(); ++i) {
    EXPECT_NEAR(carved.getGeometry(i).area(), carved.getGeometry(0).area(), 1e-3)
        << "slice " << i << " lost a different amount than slice 0";
  }
}
