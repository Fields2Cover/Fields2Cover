//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include <random>
#include "fields2cover/types.h"

TEST(fields2cover_types_swathsByCells, init) {
  const int n = 5;

  F2CSwaths swaths;
  std::vector<F2CSwaths> v_swaths;
  for (int i = n; i >= 1; --i) {
    swaths.emplace_back(F2CLineString({F2CPoint(i, i), F2CPoint(i, i + 1)}), i, i);
  }
  v_swaths.push_back(swaths);
  v_swaths.push_back(swaths);
  F2CSwathsByCells swaths_by_cells1 {swaths};
  F2CSwathsByCells swaths_by_cells2 {v_swaths};
  EXPECT_EQ(swaths_by_cells1.size(), 1);
  EXPECT_EQ(swaths_by_cells2.size(), 2);
  EXPECT_EQ(swaths_by_cells1[0].size(), n);
  EXPECT_EQ(swaths_by_cells2[0].size(), n);

  swaths_by_cells1.emplace_back(swaths);
  swaths_by_cells1.emplace_back(swaths);
  swaths_by_cells2.push_back(swaths);
  EXPECT_EQ(swaths_by_cells1.size(), 3);
  EXPECT_EQ(swaths_by_cells2.size(), 3);
  EXPECT_EQ(swaths_by_cells1[0].size(), n);
  EXPECT_EQ(swaths_by_cells1[1].size(), n);
  EXPECT_EQ(swaths_by_cells2[0].size(), n);
  EXPECT_EQ(swaths_by_cells2[1].size(), n);

  const auto sbc_const = swaths_by_cells2.clone();

  int j = 0;
  for (int i = n; i >= 1; --i) {
    EXPECT_EQ(swaths_by_cells1[0][j], swaths_by_cells2[0][j]);
    EXPECT_EQ(swaths_by_cells1[0][j].getId(), i);
    EXPECT_EQ(swaths_by_cells2[0][j].getId(), i);
    EXPECT_EQ(swaths_by_cells1.at(0)[j].getWidth(), i);
    EXPECT_EQ(swaths_by_cells2.at(0)[j].getWidth(), i);
    EXPECT_EQ(sbc_const.at(0)[j].getWidth(), i);
    EXPECT_EQ(swaths_by_cells1.back()[j].getWidth(), i);
    EXPECT_EQ(swaths_by_cells2.back()[j].getWidth(), i);
    EXPECT_EQ(sbc_const.back()[j].getWidth(), i);
    ++j;
  }

  for (size_t i = 0; i < sbc_const.sizeTotal(); ++i) {
    EXPECT_EQ(sbc_const.getSwath(i).getPath().startPoint(),
        swaths_by_cells2.getSwath(i).getPath().startPoint());
    EXPECT_EQ(sbc_const.getSwath(i).getPath().endPoint(),
        swaths_by_cells2.getSwath(i).getPath().endPoint());
  }
}

TEST(fields2cover_types_swathsByCells, reverse) {
  F2CSwaths swaths1;
  F2CSwaths swaths2;
  F2CSwaths swaths3;
  for (int i = 0; i < 5; ++i) {
    swaths1.emplace_back(F2CLineString(), 1*i+1, 1*i);
    swaths2.emplace_back(F2CLineString(), 2*i+2, 2*i);
    swaths3.emplace_back(F2CLineString(), 3*i+3, 3*i);
  }
  F2CSwathsByCells sbc({swaths1, swaths2, swaths3});
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(2 * sbc[0][i].getWidth(), sbc[1][i].getWidth());
    EXPECT_EQ(3 * sbc[0][i].getWidth(), sbc[2][i].getWidth());
  }
  sbc.reverse();
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(2 * sbc[2][i].getWidth(), sbc[1][i].getWidth());
    EXPECT_EQ(3 * sbc[2][i].getWidth(), sbc[0][i].getWidth());
  }


}




TEST(fields2cover_types_swathsByCells, flattenGivesUniqueIds) {
  F2CSwathsByCells sbc;
  for (int cell = 0; cell < 2; ++cell) {
    F2CSwaths swaths;
    for (int i = 0; i < 3; ++i) {
      swaths.append(F2CLineString(
            {F2CPoint(10 * cell + i, 0), F2CPoint(10 * cell + i, 5)}), 1);
    }
    sbc.emplace_back(swaths);
  }
  // Each cell numbers its own swaths from 0, so the ids repeat across cells.
  EXPECT_EQ(sbc.at(0).at(0).getId(), 0);
  EXPECT_EQ(sbc.at(1).at(0).getId(), 0);

  F2CSwaths flat = sbc.flatten();
  ASSERT_EQ(flat.size(), 6);
  for (size_t i = 0; i < flat.size(); ++i) {
    EXPECT_EQ(flat.at(i).getId(), static_cast<int>(i));
  }
}

TEST(fields2cover_types_swathsByCells, flattenKeepsOrderAndSourceIds) {
  F2CSwathsByCells sbc;
  for (int cell = 0; cell < 2; ++cell) {
    F2CSwaths swaths;
    for (int i = 0; i < 3; ++i) {
      swaths.append(F2CLineString(
            {F2CPoint(10 * cell + i, 0), F2CPoint(10 * cell + i, 5)}), 1);
    }
    sbc.emplace_back(swaths);
  }
  F2CSwaths flat = sbc.flatten();
  ASSERT_EQ(flat.size(), 6);
  size_t k = 0;
  for (size_t c = 0; c < sbc.size(); ++c) {
    for (size_t i = 0; i < sbc.at(c).size(); ++i, ++k) {
      EXPECT_EQ(flat.at(k).startPoint(), sbc.at(c).at(i).startPoint());
      EXPECT_EQ(flat.at(k).endPoint(), sbc.at(c).at(i).endPoint());
    }
  }
  // Renumbering the flattened copy must not touch the source.
  EXPECT_EQ(sbc.at(1).at(0).getId(), 0);
}
