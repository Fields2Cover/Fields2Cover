//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_H_
#define FIELDS2COVER_H_

#include "fields2cover/types.h"


#include "fields2cover/utils/random.h"
#include "fields2cover/utils/spline.h"
#include "fields2cover/utils/transformation.h"
#include "fields2cover/utils/parser.h"
#include "fields2cover/utils/visualizer.h"


#include "fields2cover/objectives/base_objective.h"

#include "fields2cover/objectives/hg_obj/hg_objective.h"
#include "fields2cover/objectives/hg_obj/rem_area.h"

#include "fields2cover/objectives/sg_obj/sg_objective.h"
#include "fields2cover/objectives/sg_obj/n_swath.h"
#include "fields2cover/objectives/sg_obj/n_swath_modified.h"
#include "fields2cover/objectives/sg_obj/field_coverage.h"
#include "fields2cover/objectives/sg_obj/overlaps.h"
#include "fields2cover/objectives/sg_obj/swath_length.h"

#include "fields2cover/objectives/rp_obj/rp_objective.h"
#include "fields2cover/objectives/rp_obj/direct_dist_path_obj.h"
#include "fields2cover/objectives/rp_obj/complete_turn_path_obj.h"

#include "fields2cover/objectives/pp_obj/pp_objective.h"
#include "fields2cover/objectives/pp_obj/path_length.h"

#include "fields2cover/objectives/decomp_obj/decomp_objective.h"


#include "fields2cover/decomposition/decomposition_base.h"
#include "fields2cover/decomposition/trapezoidal_decomp.h"
#include "fields2cover/decomposition/boustrophedon_decomp.h"

#include "fields2cover/headland_generator/headland_generator_base.h"
#include "fields2cover/headland_generator/constant_headland.h"

#include "fields2cover/swath_generator/swath_generator_base.h"
#include "fields2cover/swath_generator/brute_force.h"


#include "fields2cover/route_planning/single_cell_swaths_order_base.h"
#include "fields2cover/route_planning/boustrophedon_order.h"
#include "fields2cover/route_planning/snake_order.h"
#include "fields2cover/route_planning/spiral_order.h"
#include "fields2cover/route_planning/custom_order.h"

#include "fields2cover/route_planning/route_planner_base.h"

#include "fields2cover/path_planning/turning_base.h"
#include "fields2cover/path_planning/dubins_curves.h"
#include "fields2cover/path_planning/dubins_curves_cc.h"
#include "fields2cover/path_planning/reeds_shepp_curves.h"
#include "fields2cover/path_planning/reeds_shepp_curves_hc.h"
#include "fields2cover/path_planning/path_planning.h"


/// Main namespace of the fields2cover library
namespace f2c {


/// Types used by fields2cover library
namespace types {}

/// Objective algorithms' namespace
namespace obj {}

/// Swath generator algorithms' namespace
namespace sg {}

enum class SGAlg {
  BRUTE_FORCE = 0,  //Default
  GIVEN_ANGLE = 1,
};

enum class SGObjFunc {
  N_SWATH_MOD = 0,  // Default
  N_SWATH = 1,
  SWATH_LENGTH = 2,
  FIELD_COV = 3,
  OVERLAPS = 4,
};

/// Route planning algorithms' namespace
namespace rp {}

enum class RPAlg {
  SHORTEST_ROUTE = 0,  // Default
  //BOUSTROPHEDON = 1,
  //SNAKE = 2,
  //SPIRAL = 3,
  //CUSTOM = 4,
};

/// Path planning algorithms' namespace
namespace pp {}

enum class PPAlg {
  DUBINS = 0,  // Default
  DUBINS_CC = 1,
  REEDS_SHEPP = 2,
  REEDS_SHEPP_HC = 3,
};


/// Headland generator algorithms' namespace
namespace hg {}

enum class HGAlg {
  NONE = 0,
  CONST = 1,  // Default
};


/// Decomposition algorithms' namespace
namespace decomp {}

enum class DecompAlg {
  NONE = 0,  // Default
  // TRAPEZOIDAL = 1,
  // BOUSTROPHEDON = 2,
};

struct Options {
  DecompAlg decomp_alg {DecompAlg::NONE};
  double decomp_angle {0.0};

  HGAlg hg_alg {HGAlg::CONST};
  int hg_swaths {3};

  SGAlg sg_alg {SGAlg::BRUTE_FORCE};
  SGObjFunc sg_obj {SGObjFunc::N_SWATH_MOD};
  double sg_angle {0.0};

  RPAlg rp_alg {RPAlg::SHORTEST_ROUTE};

  PPAlg pp_alg {PPAlg::DUBINS};
};

F2CRoute planCovRoute(const F2CRobot& robot, const F2CCell& cell,
    const Options& opt = Options());
F2CRoute planCovRoute(const F2CRobot& robot, const F2CCells& cells,
    const Options& opt = Options());
F2CRoute planCovRoute(const F2CRobot& robot, const F2CField& field,
    bool local_crs);
F2CRoute planCovRoute(const F2CRobot& robot, const F2CField& field,
    const Options& opt = Options(), bool local_crs = true);
F2CPath planCovPath(const F2CRobot& robot, const F2CCell& cell,
    const Options& opt = Options());
F2CPath planCovPath(const F2CRobot& robot, const F2CCells& cells,
    const Options& opt = Options());
F2CPath planCovPath(const F2CRobot& robot, const F2CField& field,
    bool local_crs);
F2CPath planCovPath(const F2CRobot& robot, const F2CField& field,
    const Options& opt = Options(), bool local_crs = true);

}  // namespace f2c

#endif  // FIELDS2COVER_H_

