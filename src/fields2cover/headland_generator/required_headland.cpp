//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <vector>
#include "fields2cover/headland_generator/required_headland.h"

namespace f2c::hg {

F2CCells ReqHL::generateHeadlands(
    const F2CCell& field, const F2CRobot& robot, double track_ang) {
  double max_req_dist = this->maxHLWidthRequired(robot);
  F2CCells inner_cells {F2CCells::buffer(field, -max_req_dist)};

  F2CCells new_cells;
  for (auto&& inner_cell : inner_cells) {
    F2CCell new_cell;
    for (size_t i = 0; i < inner_cell.size(); ++i) {
      auto ring = inner_cell[i];
      auto req_dist = this->requiredHeadlandDist(robot, track_ang, ring);

      // Give each border back what it does not need to turn on. Obstacles
      // are the other way around: their borders grow into the mainland.
      for (auto&& d : req_dist) {
        d = (max_req_dist - d) * (i == 0 ? 1.0 : -1.0);
      }
      new_cell.addGeometry(ring.bufferOutwards(req_dist));
      // Close the hairline spurs offsetting each segment on its own leaves.
      new_cell = F2CCell::buffer(F2CCell::buffer(new_cell, 1e-5), -1e-5);
    }
    new_cells.addGeometry(new_cell);
    new_cells = F2CCells::buffer(F2CCells::buffer(new_cells, 1e-5), -1e-5);
  }
  return new_cells;
}

F2CCells ReqHL::generateHeadlands(
    const F2CCells& field, const F2CRobot& robot,
    const std::vector<double>& track_angs) {
  if (field.size() != track_angs.size()) {
    return generateHeadlands(field, maxHLWidthRequired(robot));
  }
  F2CCells cells;
  for (size_t i = 0; i < field.size(); ++i) {
    auto c_hl = generateHeadlands(field[i], robot, track_angs[i]);
    for (auto&& c : c_hl) {
      cells.addGeometry(c);
    }
  }
  return cells;
}

F2CCells ReqHL::generateHeadlands(
    const F2CCells& field, double dist_headland) {
  return field.buffer(-dist_headland);
}

F2CCells ReqHL::generateHeadlandArea(
    const F2CCells& field, double swath_width, int n_swaths) {
  return field.buffer(-swath_width * n_swaths);
}

std::vector<F2CCells> ReqHL::generateHeadlandSwaths(
    const F2CCells& field, double swath_width, int n_swaths, bool dir_out2in) {
  std::vector<F2CCells> hl;
  if (dir_out2in) {
    for (int i = 0; i < n_swaths; ++i) {
      hl.emplace_back(field.buffer(-swath_width * (i + 0.5)));
    }
  } else {
    hl.emplace_back(field.buffer(-swath_width * (n_swaths - 0.5)));
    for (int i = 1; i < n_swaths; ++i) {
      hl.emplace_back(hl.back().buffer(swath_width));
    }
  }
  return hl;
}

std::vector<double> ReqHL::requiredHeadlandDist(const F2CRobot& robot,
    double track_ang, const F2CLinearRing& ring) const {
  std::vector<double> v_dist;
  for (size_t i = 1; i < ring.size(); ++i) {
    v_dist.emplace_back(this->requiredHeadlandDist(
          robot, track_ang, (ring[i] - ring[i-1]).getAngleFromPoint()));
  }
  return v_dist;
}

double ReqHL::requiredHeadlandDist(const F2CRobot& robot,
    double track_ang, double border_ang) const {
  return (1.0 + fabs(sin(track_ang - border_ang))) *
    robot.getMinTurningRadius() + 0.5 * robot.getWidth();
}

}  // namespace f2c::hg
