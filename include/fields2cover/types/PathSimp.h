//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_PATHSIMP_H_
#define FIELDS2COVER_TYPES_PATHSIMP_H_

#include <vector>
#include "fields2cover/types/Point.h"
#include "fields2cover/types/PathStateSimp.h"
#include "fields2cover/types/Path.h"

namespace f2c::types {

struct PathSimp {
  std::vector<PathStateSimp> states;

 public:
  const PathStateSimp& operator[](size_t idx) const;
  PathStateSimp& operator[](size_t idx);

  PathSimp& operator+=(const PathSimp& path);
  Path toPath(double discret = 0.05, double vel = 1.0) const;
  PathSimp& appendStraightLine(double length, bool using_impl = true);
  PathSimp& prependStraightLine(double length, bool using_impl = true);
  PathSimp& appendCircle(double length, bool using_impl = true);
  PathSimp& appendCircleKurv(double length, double max_kurv,
      bool using_impl = true);
  PathSimp& appendClothoid(
      double length, double sigma, bool using_impl = true);
  PathSimp& appendBuffCloth(
      double length, double sigma, double buff_cloth, bool using_impl = true);

  // This function assumes that initial kurvature is 0.
  // Max kurvature is modified to reduce the length of the segment.
  PathSimp& appendSmoothCircularSegment(
      double ang_diff, double max_kurv, double max_sigma,
      double buff_cloth = 0.0, bool using_impl = true);
  PathSimp& appendStartHalfSmoothCircularSegment(
      double ang_diff, double max_kurv, double max_sigma,
      double buff_cloth = 0.0, bool using_impl = true);
  PathSimp& appendEndHalfSmoothCircularSegment(
      double ang_diff, double max_kurv, double max_sigma,
      double buff_cloth = 0.0, bool using_impl = true);
  PathSimp& appendClassicSmoothCircularSegment(
      double ang_diff, double max_kurv, double max_sigma,
      bool using_impl = true);

  PathSimp& moveTo(const Point& new_p_start);
  PathSimp& rotate(const Point& rot_point, double ang);
  void reverseVehicleDir();
  void mirrorX();
  void mirrorY();
  // void setFordwardMove();
  // void setBackwardMove();
  void setImplOn();
  void setImplOff();

  size_t size() const;

  PointState atStart() const;
  PointState atEnd() const;
  PointState at(double t) const;
  double length() const;
};


}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_PATHSIMP_H_
