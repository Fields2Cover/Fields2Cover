//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <ogr_geometry.h>
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
  std::stringstream ss;
  ss << f.rdbuf();
  std::string wkt = ss.str();
  OGRGeometry* geom = nullptr;
  const char* c = wkt.c_str();
  OGRGeometryFactory::createFromWkt(&c, nullptr, &geom);
  F2CCell cell(geom);
  OGRGeometryFactory::destroyGeometry(geom);
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
  F2CField field(F2CCells(raw), "ee_field_130");
  field.setCRS("EPSG:4326");
  f2c::Transform::transformToUTM(field);

  f2c::decomp::BoustrophedonDecomp decomp;
  decomp.setSplitAngle(58.0 * M_PI / 180.0);
  EXPECT_NO_THROW(decomp.decompose(field.getField()));
}

