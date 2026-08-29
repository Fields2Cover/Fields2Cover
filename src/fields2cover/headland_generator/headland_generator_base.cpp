//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <vector>
#include "fields2cover/headland_generator/headland_generator_base.h"

namespace f2c::hg {

F2CCells HeadlandGeneratorBase::generateHeadlands(
    const F2CCell& field, const F2CRobot& robot, double track_ang) {
  return this->generateHeadlands(F2CCells(field), maxHLWidthRequired(robot));
}

F2CCells HeadlandGeneratorBase::generateHeadlands(
    const F2CCells& field, const F2CRobot& robot,
    const std::vector<double>& track_angs) {
  return this->generateHeadlands(field, maxHLWidthRequired(robot));
}

double HeadlandGeneratorBase::maxHLWidthRequired(const F2CRobot& robot) const {
  return 2.0 * robot.getMinTurningRadius() + 0.5 * robot.getWidth();
}

}  // namespace f2c::hg
