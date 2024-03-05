//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_BASE_OBJECTIVE_H_
#define FIELDS2COVER_OBJECTIVES_BASE_OBJECTIVE_H_

#include <vector>
#include "fields2cover/types.h"

namespace f2c::obj {

template <typename T>
class BaseObjective {
 public:
  // Avoid using variadic templates. SWIG don't work well with it

  /// @brief Compute the cost function with minimizing sign.
  ///
  /// If the objective is to maximize the cost function, the cost is
  /// multiplied by -1.
  template <typename T1>
  double computeCostWithMinimizingSign(const T1& t1) {
    return (static_cast<T*>(this)->isMinimizing() ? 1.0 : -1.0) *
      static_cast<T*>(this)->computeCost(t1);
  }
  template <typename T1, typename T2>
  double computeCostWithMinimizingSign(const T1& t1, const T2& t2) {
    return (static_cast<T*>(this)->isMinimizing() ? 1.0 : -1.0) *
      static_cast<T*>(this)->computeCost(t1, t2);
  }
  template <typename T1, typename T2, typename T3>
  double computeCostWithMinimizingSign(
      const T1& t1, const T2& t2, const T3& t3) {
    return (static_cast<T*>(this)->isMinimizing() ? 1.0 : -1.0) *
      static_cast<T*>(this)->computeCost(t1, t2, t3);
  }
  template <typename T1, typename T2, typename T3, typename T4>
  double computeCostWithMinimizingSign(
      const T1& t1, const T2& t2, const T3& t3, const T4& t4) {
    return (static_cast<T*>(this)->isMinimizing() ? 1.0 : -1.0) *
      static_cast<T*>(this)->computeCost(t1, t2, t3, t4);
  }

 public:
  /// Return true if the objective is to minimize the cost function
  virtual bool isMinimizing() const { return true;}
  /// Return true if the objective is to maximize the cost function
  virtual bool isMaximizing() const { return !isMinimizing();}
};


}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_BASE_OBJECTIVE_H_
