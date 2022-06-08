//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/objectives/overlaps.h"
#include "fields2cover/types.h"

TEST(fields2cover_obj_overlaps, computeCost_cost) {
  F2CLineString path1, path2, path3;
  double width {2.0};
  path1->addPoint( 0.0, 1.0);
  path1->addPoint( 4.0, 1.0);
  path2->addPoint( 0.0, 3.0);
  path2->addPoint( 4.0, 3.0);
  path3->addPoint( 0.0, 2.0);
  path3->addPoint( 4.0, 2.0);  
  F2CSwath swath1(path1, width);
  F2CSwath swath2(path2, width);
  F2CSwath swath3(path3, width);
  	
  F2CSwaths swaths_no_overlap {swath1,swath2};
  F2CSwaths swaths_overlap_025 {swath1,swath3};
  F2CSwaths swaths_overlap_050_1 {swath1,swath2,swath3};
  F2CSwaths swaths_overlap_050_2 {swath2,swath1,swath3};
  F2CSwaths swaths_overlap_050_3 {swath2,swath2};
  
  F2CCell field;
  F2CCells fields;
  
  F2CLinearRing line;
  line->addPoint(0, 0);
  line->addPoint(4, 0);
  line->addPoint(4, 4);
  line->addPoint(0, 4);
  line->addPoint(0, 0);
  field.addRing(line);
  fields.addGeometry(field);
  
  f2c::obj::Overlaps overlap;
    
  EXPECT_EQ(overlap.computeCost(field, swaths_no_overlap), 0.0);
  EXPECT_EQ(overlap.computeCost(fields, swaths_no_overlap), 0.0);
  EXPECT_EQ(overlap.computeCostWithMinimizingSign(fields, swaths_no_overlap), 0.0);

  EXPECT_EQ(overlap.computeCost(fields, swaths_overlap_025), 0.25);
  EXPECT_EQ(overlap.computeCostWithMinimizingSign(fields, swaths_overlap_025), 0.25);
  
  EXPECT_EQ(overlap.computeCost(fields, swaths_overlap_050_1), 0.5);
  EXPECT_EQ(overlap.computeCostWithMinimizingSign(fields, swaths_overlap_050_1), 0.5);

  EXPECT_EQ(overlap.computeCost(fields, swaths_overlap_050_2), 0.5);
  EXPECT_EQ(overlap.computeCostWithMinimizingSign(fields, swaths_overlap_050_2), 0.5);
  
  EXPECT_EQ(overlap.computeCost(fields, swaths_overlap_050_3), 0.5);
  EXPECT_EQ(overlap.computeCostWithMinimizingSign(fields, swaths_overlap_050_3), 0.5);
}

TEST(fields2cover_obj_overlaps, params_check) {
  f2c::obj::Overlaps objective;

  EXPECT_FALSE(objective.isMaximizing());
  EXPECT_TRUE(objective.isMinimizing());
}

