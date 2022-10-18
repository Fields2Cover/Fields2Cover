//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_H_
#define FIELDS2COVER_H_

#include "fields2cover/types.h"


#include "fields2cover/utils/random.h"
#include "fields2cover/utils/transformation.h"
#include "fields2cover/utils/parser.h"
#include "fields2cover/utils/visualizer.h"


#include "fields2cover/objectives/base_objective.h"

#include "fields2cover/objectives/hg_objective.h"
#include "fields2cover/objectives/rem_area.h"

#include "fields2cover/objectives/sg_objective.h"
#include "fields2cover/objectives/n_swath.h"
#include "fields2cover/objectives/field_coverage.h"
#include "fields2cover/objectives/overlaps.h"
#include "fields2cover/objectives/swath_length.h"

#include "fields2cover/objectives/rp_objective.h"
#include "fields2cover/objectives/direct_dist_path_obj.h"
#include "fields2cover/objectives/complete_turn_path_obj.h"

#include "fields2cover/objectives/pp_objective.h"
#include "fields2cover/objectives/path_length.h"


#include "fields2cover/headland_generator/headland_generator_base.h"
#include "fields2cover/headland_generator/constant_headland.h"

#include "fields2cover/swath_generator/swath_generator_base.h"
#include "fields2cover/swath_generator/brute_force.h"

#include "fields2cover/route_planning/single_cell_swaths_order_base.h"
#include "fields2cover/route_planning/boustrophedon_order.h"
#include "fields2cover/route_planning/snake_order.h"
#include "fields2cover/route_planning/spiral_order.h"
#include "fields2cover/route_planning/custom_order.h"

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

/// Swath generator algorithms' namespace
namespace sg {}

/// Route planning algorithms' namespace
namespace rp {}

/// Path planning algorithms' namespace
namespace pp {}

/// Objective algorithms' namespace
namespace obj {}

/// Headland generator algorithms' namespace
namespace hg {}

/// Decomposition algorithms' namespace
namespace decomp {}

}  // namespace f2c

#endif  // FIELDS2COVER_H_

