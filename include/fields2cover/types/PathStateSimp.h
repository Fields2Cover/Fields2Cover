//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_PATHSTATESIMP_H_
#define FIELDS2COVER_TYPES_PATHSTATESIMP_H_

#include <vector>
#include "fields2cover/types/Point.h"
#include "fields2cover/types/PathState.h"

namespace f2c::types {

enum class SectionSimp {
  STRAIGHT = 0,
  CIRCLE = 1,
  CLOTHOID = 2,
  BUFF_CLOTHOID = 3,
};

struct PointState {
  Point p {0.0, 0.0};
  double theta {0.0};
  double kurv {0.0};
};

struct PathStateSimp {
  PointState start;
  SectionSimp type;
  double len {0.0};
  double sigma {0.0};  // linear change kurv
  double dist_cloth {0.0};
  bool using_impl {true};

 public:
  bool isDirFront() const {return len >= 0;}

  PathState toPathState(double t, double len_state, double vel) const;
  std::vector<PathState> toPathStates(double discret_turn, double vel) const;

  PointState at(double norm_length) const;
  PointState atEnd() const;
  void reversePath();
  void reverseVehicleDir();
  void mirrorX();
  void mirrorY();
  double length() const;
};

}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_PATHSTATESIMP_H_
