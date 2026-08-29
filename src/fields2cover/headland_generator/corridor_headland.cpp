//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <algorithm>
#include <cmath>
#include <vector>
#include "fields2cover/headland_generator/corridor_headland.h"

namespace f2c::hg {

namespace {
constexpr double kTol = 1e-3;
constexpr double kSpur = 1e-9;
constexpr double kSameSize = 1e-9;
}  // namespace

std::vector<CorridorShare> CorridorHL::corridorShares(
    const F2CCells& field) const {
  // The corridor comes out of the smaller cell, so the larger neighbour keeps
  // its shape: taking half from each side notches both, and a notched cell
  // costs the swath generator a pass.
  std::vector<CorridorShare> shares;
  for (size_t i = 0; i < field.size(); ++i) {
    const F2CLinearRing ring = field.getCellBorder(i);
    const double perimeter = ring.length();
    for (size_t k = 0; k < field.size(); ++k) {
      if (k == i) {
        continue;
      }
      CorridorShare share;
      share.cell_i = i;
      share.cell_k = k;
      share.perimeter_i = perimeter;
      share.perimeter_k = field.getCellBorder(k).length();
      // Cells that are the same size share the corridor, each giving half.
      // The comparison has to be loose: perimeters that are equal in theory
      // differ in the last bits, and letting that noise pick a winner leaves
      // some borders with a full corridor and others with none.
      share.same_size =
          std::abs(perimeter - share.perimeter_k) <= kSameSize * perimeter;
      share.share = share.same_size ? 0.5 :
          (perimeter < share.perimeter_k ? 1.0 : 0.0);
      // Keep the part of each border edge that the neighbour actually touches.
      const F2CCells neighbour = F2CCells::buffer(field.getGeometry(k), kTol);
      for (size_t e = 0; e + 1 < ring.size(); ++e) {
        F2CMultiLineString edge;
        edge.addGeometry(
            F2CLineString({ring.getGeometry(e), ring.getGeometry(e + 1)}));
        const F2CMultiLineString shared = edge.intersection(neighbour);
        for (size_t j = 0; j < shared.size(); ++j) {
          const F2CLineString part = shared.getGeometry(j);
          if (part.size() > 1 && part.length() > kTol) {
            share.shared_border.addGeometry(part);
            share.shared_length += part.length();
          }
        }
      }
      // Cells that only meet at a corner, or not at all, are not neighbours.
      if (share.shared_length > 0.0) {
        shares.emplace_back(share);
      }
    }
  }
  return shares;
}

F2CCells CorridorHL::generateHeadlands(
    const F2CCells& field, double dist_headland) {
  const std::vector<CorridorShare> shares = corridorShares(field);
  F2CCells carved;
  for (size_t i = 0; i < field.size(); ++i) {
    F2CCells cell {field.getGeometry(i)};
    for (const CorridorShare& share : shares) {
      if (share.cell_i != i || share.share <= 0.0) {
        continue;  // this cell gives no corridor on that border
      }
      for (size_t j = 0; j < share.shared_border.size(); ++j) {
        cell = cell.difference(F2CCells::buffer(
            share.shared_border.getGeometry(j), dist_headland * share.share));
      }
    }
    // The difference can leave a zero-width spur behind. Opening the result by
    // a hair drops it without moving any real edge.
    const F2CCells clean = cell.buffer(-kSpur).buffer(kSpur);
    for (size_t j = 0; j < clean.size(); ++j) {
      carved.addGeometry(clean.getGeometry(j));
    }
  }
  return carved;
}

F2CCells CorridorHL::generateHeadlandArea(
    const F2CCells& field, double swath_width, int n_swaths) {
  return generateHeadlands(field, swath_width * n_swaths);
}

std::vector<F2CCells> CorridorHL::generateHeadlandSwaths(
    const F2CCells& field, double swath_width, int n_swaths, bool dir_out2in) {
  std::vector<F2CCells> hl;
  for (int i = 0; i < n_swaths; ++i) {
    const int n = dir_out2in ? (n_swaths - i) : (i + 1);
    hl.emplace_back(generateHeadlands(field, swath_width * (n - 0.5)));
  }
  return hl;
}


}  // namespace f2c::hg
