//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_RP_OBJECTIVE_H_
#define FIELDS2COVER_OBJECTIVES_RP_OBJECTIVE_H_

#include <vector>
#include "fields2cover/types.h"
#include "fields2cover/objectives/base_objective.h"

namespace f2c::obj {

/// @brief Base class for objective functions of route planners.
class RPObjective : public BaseObjective<RPObjective> {
 public:
  /// Return the cost of going from point p1 to point p2
  /// @param p1 Start point
  /// @param p2 End point
  /// @return Cost value
  virtual double computeCost(const F2CPoint& p1, const F2CPoint& p2);

  /// Return the cost of going from point p1 to point p2
  /// @param p1 Start point
  /// @param ang1 Angle of the robot in p1
  /// @param p2 End point
  /// @return Cost value
  virtual double computeCost(
      const F2CPoint& p1, double ang1, const F2CPoint& p2);

  /// Return the cost of going from point p1 to point p2
  /// @param p1 Start point
  /// @param ang1 Angle of the robot in p1
  /// @param p2 End point
  /// @param ang2 Angle of the robot in p2
  /// @return Cost value
  virtual double computeCost(
      const F2CPoint& p1, double ang1, const F2CPoint& p2, double ang2);  // = 0

  /// Return the cost of going from point p1 to point p2
  /// @param p1 Start point
  /// @param p2 End point
  /// @param ang2 Angle of the robot in p2
  /// @return Cost value
  virtual double computeCost(
      const F2CPoint& p1, const F2CPoint& p2, double ang2);

  /// Return the cost of going from swath s to point p
  /// @param s Start point (end of the swath)
  /// @param p End point
  /// @return Cost value
  virtual double computeCost(const F2CSwath& s, const F2CPoint& p);

  /// Return the cost of going from swath s to point p
  /// @param s1 Start point (end of the swath)
  /// @param s2 End point (start of the swath)
  /// @return Cost value
  virtual double computeCost(const F2CSwath& s1, const F2CSwath& s2);

  /// Return the cost of going from swath s to point p
  /// @param s Start point (end of the swath)
  /// @param p End point
  /// @param ang Angle of the robot in p
  /// @return Cost value
  virtual double computeCost(const F2CSwath& s, const F2CPoint& p, double ang);

  /// Return the cost of going from point p to swath s
  /// @param p Start point
  /// @param s End point (start of the swath)
  /// @return Cost value
  virtual double computeCost(const F2CPoint& p, const F2CSwath& s);

  /// Return the cost of going from point p to swath s
  /// @param p Start point
  /// @param ang Angle of the robot in p
  /// @param s End point (start of the swath)
  /// @return Cost value
  virtual double computeCost(const F2CPoint& p, double ang, const F2CSwath& s);

  /// Return the cost of covering all the points of ps
  /// @param ps vector of points
  /// @return Cost value
  virtual double computeCost(const std::vector<F2CPoint>& ps);

  /// Return the cost of covering all the points of ps
  /// @param ps vector of points
  /// @return Cost value
  virtual double computeCost(const F2CMultiPoint& ps);

  /// Return the cost of going from swath s to the first point of ps
  /// @param s Start point (end of the swath)
  /// @param ps vector of points
  /// @return Cost value
  virtual double computeCost(const F2CSwath& s, const F2CMultiPoint& ps);

  /// Return the cost of going from the last swath of s to the first point of ps
  /// @param s Start point (end of the last swath)
  /// @param ps vector of points (first point)
  /// @return Cost value
  virtual double computeCost(const F2CSwaths& s, const F2CMultiPoint& ps);

  /// Return the cost of going from the last point of ps to the swath s
  /// @param ps vector of points (end point)
  /// @param s Start point (start of the swath)
  /// @return Cost value
  virtual double computeCost(const F2CMultiPoint& ps, const F2CSwath& s);

  /// Return the cost of going from the last point of ps to the first swath of s
  /// @param ps vector of points (end point)
  /// @param s Start point (start of the first swath)
  /// @return Cost value
  virtual double computeCost(const F2CMultiPoint& ps, const F2CSwaths& s);

  /// Return the cost of covering a swath
  /// @param s Swath
  /// @return Cost value
  virtual double computeCost(const F2CSwath& s);

  /// @brief Return the cost of covering a vector of swaths.
  ///
  /// Costs of each swath + Cost of going from one to another. The order
  /// may affect the cost.
  /// @param s Swaths
  /// @return Cost value
  virtual double computeCost(const F2CSwaths& swaths);

  /// @brief Return the cost of covering a Route.
  ///
  /// Costs of each swath + Cost of going from one to another.
  /// @param r Route
  /// @return Cost value
  virtual double computeCost(const F2CRoute& r);
};


}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_RP_OBJECTIVE_H_
