//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <utility>
#include "fields2cover/objectives/sg_obj/field_coverage.h"
#include "fields2cover/types.h"

TEST(fields2cover_obj_field_coverage, get_best_value) {
  double width {2.0};

  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 1.0), F2CPoint(4.0, 1.0)}), width);
  F2CSwath swath2(F2CLineString({F2CPoint(0.0, 3.0), F2CPoint(4.0, 3.0)}), width);
  F2CSwath swath3(F2CLineString({F2CPoint(0.0, 2.0), F2CPoint(4.0, 2.0)}), width);
  F2CSwaths swaths {swath1};

  F2CCell field;
  F2CLinearRing line;
  line.addPoint(0, 0);
  line.addPoint(4, 0);
  line.addPoint(4, 4);
  line.addPoint(0, 4);
  line.addPoint(0, 0);
  field.addRing(line);
  F2CCells fields{field};

  f2c::obj::FieldCoverage obj;
  EXPECT_NEAR(obj.computeCost(field, swaths), 0.5, 1e-5);
  EXPECT_NEAR(obj.computeCost(fields, swaths), 0.5, 1e-5);

  swaths.emplace_back(swath3);
  EXPECT_NEAR(obj.computeCost(fields, swaths), 0.75, 1e-5);

  swaths.emplace_back(swath2);
  EXPECT_NEAR(obj.computeCost(fields, swaths), 1.0, 1e-5);
}

TEST(fields2cover_obj_field_coverage, computeCost_cost) {
  double width {2.0};
  F2CLineString path1, path2, path3;
  path1.addPoint( 0.0, 1.0);
  path1.addPoint( 4.0, 1.0);
  path2.addPoint( 0.0, 3.0);
  path2.addPoint( 4.0, 3.0);
  path3.addPoint( 0.0, 2.0);
  path3.addPoint( 4.0, 2.0);
  F2CSwath swath1(path1, width);
  F2CSwath swath2(path2, width);
  F2CSwath swath3(path3, width);

  F2CSwaths swaths_full {swath1,swath2};
  F2CSwaths swaths_overlap {swath1,swath3};
  F2CSwaths swaths_half {swath1};

  F2CCells fields;
  F2CCell field;

  F2CLinearRing line;
  line.addPoint(0, 0);
  line.addPoint(4, 0);
  line.addPoint(4, 4);
  line.addPoint(0, 4);
  line.addPoint(0, 0);
  field.addRing(line);
  fields.addGeometry(field);

  f2c::obj::FieldCoverage coverage;

  EXPECT_EQ(coverage.computeCost(fields, swaths_full), 1.0);
  EXPECT_EQ(coverage.computeCostWithMinimizingSign(fields, swaths_full), -1.0);

  EXPECT_EQ(coverage.computeCost(fields, swaths_overlap), 0.75);
  EXPECT_EQ(coverage.computeCostWithMinimizingSign(fields, swaths_overlap), -0.75);

  EXPECT_EQ(coverage.computeCost(fields, swaths_half), 0.5);
  EXPECT_EQ(coverage.computeCostWithMinimizingSign(fields, swaths_half), -0.5);
}

TEST(fields2cover_obj_field_coverage, params_check) {
  f2c::obj::FieldCoverage objective;
  EXPECT_TRUE(objective.isMaximizing());
  EXPECT_FALSE(objective.isMinimizing());
  EXPECT_FALSE(objective.isFastCompAvailable());
}

