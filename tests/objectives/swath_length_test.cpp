//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/objectives/swath_length.h"
#include "fields2cover/types.h"

TEST(fields2cover_obj_swath_length, computeCost_cost) {
  F2CLineString path1, path2, path3;
  path1->addPoint( 0.0, 1.0);
  path1->addPoint( 4.0, 1.0);
  path2->addPoint( 0.0, 3.0);
  path2->addPoint( 4.0, 3.0);
  path3->addPoint( 0.0, 0.0);  
  path3->addPoint( 1.0, 1.0);
  F2CSwath swath1(path1);
  F2CSwath swath2(path2);
  F2CSwath swath3(path3);
  	
  F2CSwaths swaths1 {swath1};
  F2CSwaths swaths2 {swath3};
  F2CSwaths swaths3 {swath1,swath2};
  F2CSwaths swaths4 {swath1,swath2,swath1,swath2};
  
  f2c::obj::SwathLength length;


  EXPECT_EQ(length.computeCost(swath1), 4.0);

  EXPECT_EQ(length.computeCost(swaths1), 4.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(swaths1), length.computeCost(swaths1));
  
  EXPECT_NEAR(length.computeCost(swaths2)*length.computeCost(swaths2), 2.0, 0.001);
  EXPECT_EQ(length.computeCostWithMinimizingSign(swaths2), length.computeCost(swaths2));
  
  EXPECT_EQ(length.computeCost(swaths3), 8.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(swaths3), length.computeCost(swaths3));

  EXPECT_EQ(length.computeCost(swaths4), 16.0);
  EXPECT_EQ(length.computeCostWithMinimizingSign(swaths4), length.computeCost(swaths4));
}

TEST(fields2cover_obj_swath_length, params_check) {
  f2c::obj::SwathLength objective;

  EXPECT_FALSE(objective.isMaximizing());
  EXPECT_TRUE(objective.isMinimizing());
}

