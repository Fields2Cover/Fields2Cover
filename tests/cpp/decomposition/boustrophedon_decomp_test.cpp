//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <string>
#include "fields2cover/decomposition/boustrophedon_decomp.h"
#include "fields2cover/objectives/sg_obj/n_swath.h"
#include "fields2cover/utils/random.h"
#include "fields2cover/utils/transformation.h"
#include "fields2cover/types.h"

TEST(fields2cover_decomp_boustrophedon, decompose) {
  f2c::Random rand(42);
  F2CCells non_convex_field {rand.genNonConvexCell(1e3)};
  F2CCells convex_field {rand.genConvexCell(1e2)};
  F2CCells cells = non_convex_field.difference(convex_field);

  f2c::decomp::BoustrophedonDecomp decomp;
  decomp.setSplitAngle(0.5*M_PI);
  auto decomp_lines = decomp.genSplitLines(cells);
  auto decomp_field = decomp.decompose(cells);
  EXPECT_EQ(decomp_field.size(), 4);
  EXPECT_NEAR(decomp_field.area(), cells.area(), 1e-3);
}

namespace {
F2CCell loadWktCell(const std::string& path) {
  std::ifstream f(path);
  if (!f.is_open()) {
    ADD_FAILURE() << "could not open " << path;
    return F2CCell();
  }
  std::stringstream ss;
  ss << f.rdbuf();
  F2CCell cell;
  cell.importFromWkt(ss.str());
  return cell;
}
}  // namespace

TEST(fields2cover_decomp_boustrophedon, doesNotThrowOnAPinchedSplit) {
  // Splitting can leave a piece touching itself at a single point (a
  // pinch). Cells::splitByLine re-inflates each split piece with
  // Cell::buffer, which only accepts a single polygon back -- a positive
  // buffer on a pinched piece can separate it into two, and that used to
  // throw std::invalid_argument instead of decomposing the field.
  F2CCell raw = loadWktCell(std::string(DATA_PATH) + "ee_field_130.wkt");
  ASSERT_GT(raw.area(), 0) << "ee_field_130.wkt did not load a real field";
  F2CField field(F2CCells(raw), "ee_field_130");
  field.setCRS("EPSG:4326");
  f2c::Transform::transformToUTM(field);

  f2c::decomp::BoustrophedonDecomp decomp;
  decomp.setSplitAngle(58.0 * M_PI / 180.0);
  EXPECT_NO_THROW(decomp.decompose(field.getField()));
}


TEST(fields2cover_decomp_boustrophedon, simplifyDropsBorderNoiseBeforeSplitting) {
  // A digitised border turns back on itself on details far smaller than the
  // machine, and the decomposition splits the field on every one of them.
  F2CCell raw = loadWktCell(std::string(DATA_PATH) + "ee_field_130.wkt");
  ASSERT_GT(raw.area(), 0) << "ee_field_130.wkt did not load a real field";
  F2CField field(F2CCells(raw), "ee_field_130");
  field.setCRS("EPSG:4326");
  f2c::Transform::transformToUTM(field);
  const F2CCells cells = field.getField();

  f2c::decomp::BoustrophedonDecomp decomp;
  decomp.setSplitAngle(0.0);
  F2CRobot robot(2.5, 2.5);

  EXPECT_EQ(decomp.decompose(cells).size(), 16);

  // Detail below 0.6 of the coverage width -- 1.5 m for this robot -- is not
  // something it can act on, and four of those sixteen pieces only exist
  // because of it.
  const F2CCells simple =
      f2c::decomp::simplifyForDecomposition(cells, robot);
  EXPECT_EQ(decomp.decompose(simple).size(), 12);

  // The noise goes, the field stays: the area barely moves.
  EXPECT_NEAR(simple.area(), cells.area(), 0.005 * cells.area());
}

TEST(fields2cover_decomp_boustrophedon, simplifyTakesTheShareItIsGiven) {
  // A 100x100 field with a 1 x 0.5 m notch cut out of its top edge, and a
  // robot covering 2.5 m.
  const F2CCells cells {F2CCell(F2CLinearRing({
      F2CPoint(0, 0), F2CPoint(100, 0), F2CPoint(100, 100),
      F2CPoint(50, 100), F2CPoint(50, 99.5), F2CPoint(49, 99.5),
      F2CPoint(49, 100), F2CPoint(0, 100), F2CPoint(0, 0)}))};
  F2CRobot robot(2.5, 2.5);

  // Left to its default, detail below 0.6 of the coverage width -- 1.5 m --
  // goes, and the notch with it.
  EXPECT_NEAR(f2c::decomp::simplifyForDecomposition(cells, robot).area(),
      100 * 100, 1e-6);
  // Asked for a tenth, 0.25 m, the 0.5 m deep notch is detail to keep.
  EXPECT_NEAR(
      f2c::decomp::simplifyForDecomposition(cells, robot, 0.1).area(),
      100 * 100 - 0.5, 1e-6);
}
