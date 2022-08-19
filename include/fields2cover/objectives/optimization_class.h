//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_OPTIMIZATION_CLASS_H_
#define FIELDS2COVER_OBJECTIVES_OPTIMIZATION_CLASS_H_

#include <memory>
#include <type_traits>
#include "fields2cover/types.h"
#include "fields2cover/objectives/global_objective.h"

namespace f2c::obj {

/// @brief Base class to classes that have a global cost function as objective.
template <typename T>
class OptimizationClass {
  static_assert(std::is_base_of<GlobalObjective, T>::value,
      "T must derive from GlobalObjective");

 public:
  // Avoid using variadic templates. SWIG don't work well with it

  /// Compute the cost returned by the global cost function inherited
  /// (OptimizationClass<T>::objective_).
  /// @param t All the data structures that could be needed by
  /// OptimizationClass<T>::objective_
  /// @return Cost value
  template <typename T1>
  double computeCost(const T1& t1) const {
    return objective_.computeCost(t1);
  }
  template <typename T1, typename T2>
  double computeCost(const T1& t1, const T2& t2) const {
    return objective_.computeCost(t1, t2);
  }
  template <typename T1, typename T2, typename T3>
  double computeCost(
      const T1& t1, const T2& t2, const T3& t3) const {
    return objective_.computeCost(t1, t2, t3);
  }
  template <typename T1, typename T2, typename T3, typename T4>
  double computeCost(
      const T1& t1, const T2& t2, const T3& t3, const T4& t4) const {
    return objective_.computeCost(t1, t2, t3, t4);
  }

  /// Compute the cost returned by the global cost function inherited.
  /// If the OptimizationClass<T>::objective_ is maximizing, the cost is
  /// multiplied by -1.
  /// @param t All the data structures that could be needed by
  /// OptimizationClass<T>::objective_
  /// @return Cost value
  template <typename T1>
  double computeCostWithMinimizingSign(const T1& t1) const {
    return objective_.computeCostWithMinimizingSign(t1);
  }
  template <typename T1, typename T2>
  double computeCostWithMinimizingSign(const T1& t1, const T2& t2) const {
    return objective_.computeCostWithMinimizingSign(t1, t2);
  }
  template <typename T1, typename T2, typename T3>
  double computeCostWithMinimizingSign(
      const T1& t1, const T2& t2, const T3& t3) const {
    return objective_.computeCostWithMinimizingSign(t1, t2, t3);
  }
  template <typename T1, typename T2, typename T3, typename T4>
  double computeCostWithMinimizingSign(
      const T1& t1, const T2& t2, const T3& t3, const T4& t4) const {
    return objective_.computeCostWithMinimizingSign(t1, t2, t3, t4);
  }

 protected:
  /// Objective that defines the global cost function
  /// to minimize (or maximize).
  T objective_;
};

}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_OPTIMIZATION_CLASS_H_
