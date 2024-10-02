//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <utility>
#include "fields2cover.h"

namespace f2c {

/*
std::unique_ptr<f2c::decomp::DecompositionBase> getDecompPtr(const Options& opt) {
  std::unique_ptr<f2c::decomp::DecompositionBase> ptr;
  switch (opt.decomp_alg) {
    case DecompAlg::TRAPEZOIDAL: {
      decomp::TrapezoidalDecomp trap_decomp;
      trap_decomp.setSplitAngle(opt.decomp_angle);
      ptr = std::make_unique<decomp::TrapezoidalDecomp>(trap_decomp);
      break;
    }
    case DecompAlg::BOUSTROPHEDON: {
      decomp::BoustrophedonDecomp boustrophedon_decomp;
      boustrophedon_decomp.setSplitAngle(opt.decomp_angle);
      ptr = std::make_unique<decomp::BoustrophedonDecomp>(
          boustrophedon_decomp);
      break;
    }
    default:
      break;
  }
  return ptr;
}
*/

std::unique_ptr<obj::SGObjective> getSGObjPtr(const Options& opt) {
  std::unique_ptr<obj::SGObjective> ptr;
  switch (opt.sg_obj) {
    case SGObjFunc::N_SWATH_MOD:
      ptr = std::make_unique<obj::NSwathModified>();
      break;
    case SGObjFunc::N_SWATH:
      ptr = std::make_unique<obj::NSwath>();
      break;
    case SGObjFunc::SWATH_LENGTH:
      ptr = std::make_unique<obj::SwathLength>();
      break;
    case SGObjFunc::FIELD_COV:
      ptr = std::make_unique<obj::FieldCoverage>();
      break;
    case SGObjFunc::OVERLAPS:
      ptr = std::make_unique<obj::Overlaps>();
      break;
  }
  return ptr;
}

std::unique_ptr<f2c::pp::TurningBase> getPPTurningBasePtr(const Options& opt) {
  std::unique_ptr<f2c::pp::TurningBase> turn_planner;
  switch (opt.pp_alg) {
    case PPAlg::DUBINS:
      turn_planner = std::make_unique<f2c::pp::DubinsCurves>();
      break;
    case PPAlg::DUBINS_CC:
      turn_planner = std::make_unique<f2c::pp::DubinsCurvesCC>();
      break;
    case PPAlg::REEDS_SHEPP:
      turn_planner = std::make_unique<f2c::pp::ReedsSheppCurves>();
      break;
    case PPAlg::REEDS_SHEPP_HC:
      turn_planner = std::make_unique<f2c::pp::ReedsSheppCurvesHC>();
      break;
  }
  return turn_planner;
}





F2CRoute planCovRoute(const F2CRobot& robot, const F2CCell& cell,
    const Options& opt) {
  return planCovRoute(robot, F2CCells(cell), opt);
}

F2CRoute planCovRoute(const F2CRobot& robot, const F2CCells& cells,
    const Options& opt) {
  F2CCells mainland;
  F2CCells fast_hl_rings;
  switch (opt.hg_alg) {
    case HGAlg::NONE:
      mainland = cells;
      fast_hl_rings = cells;
      break;
    case HGAlg::CONST: {
      f2c::hg::ConstHL const_hl;
      mainland = const_hl.generateHeadlandArea(
          cells, robot.getWidth(), opt.hg_swaths);
      auto hl_swaths = const_hl.generateHeadlandSwaths(
          cells, robot.getWidth(), opt.hg_swaths);
      fast_hl_rings = hl_swaths[floor(0.5*opt.hg_swaths)];
      break;
    }
  }

  f2c::sg::BruteForce bf;
  F2CSwathsByCells swaths;
  switch (opt.sg_alg) {
    case SGAlg::BRUTE_FORCE:
      swaths = bf.generateBestSwaths(
          *getSGObjPtr(opt), robot.getCovWidth(), mainland);
      break;
    case SGAlg::GIVEN_ANGLE:
      swaths = bf.generateSwaths(
          opt.sg_angle, robot.getCovWidth(), mainland);
  }
  f2c::rp::RoutePlannerBase rp;
  return rp.genRoute(fast_hl_rings, swaths);
}

F2CRoute planCovRoute(const F2CRobot& robot, const F2CField& field,
    bool local_crs) {
  return planCovRoute(robot, field, Options(), local_crs);
}

F2CRoute planCovRoute(const F2CRobot& robot, const F2CField& field,
    const Options& opt, bool local_crs) {
  F2CField field_c = field.clone();
  if (!local_crs) {
    f2c::Transform::transformToUTM(field_c);
  }
  F2CRoute route = planCovRoute(robot, field_c.getField(), opt);
  return local_crs ? route : f2c::Transform::transformToPrevCRS(route, field_c);
}

F2CPath planCovPath(const F2CRobot& robot, const F2CCell& cell,
    const Options& opt) {
  return f2c::pp::PathPlanning::planPath(
      robot, planCovRoute(robot, cell, opt), *getPPTurningBasePtr(opt));
}

F2CPath planCovPath(const F2CRobot& robot, const F2CCells& cells,
    const Options& opt) {
  return f2c::pp::PathPlanning::planPath(
      robot, planCovRoute(robot, cells, opt), *getPPTurningBasePtr(opt));
}

F2CPath planCovPath(const F2CRobot& robot, const F2CField& field,
    bool local_crs) {
  return planCovPath(robot, field, Options(), local_crs);
}


F2CPath planCovPath(const F2CRobot& robot, const F2CField& field,
    const Options& opt, bool local_crs) {
  F2CField field_c = field.clone();
  if (!local_crs) {
    f2c::Transform::transformToUTM(field_c);
  }
  F2CPath path = f2c::pp::PathPlanning::planPath(
      robot, planCovRoute(robot, field_c.getField(), opt),
      *getPPTurningBasePtr(opt));
  return local_crs ? path : f2c::Transform::transformToPrevCRS(path, field_c);
}



}  // namespace f2c

