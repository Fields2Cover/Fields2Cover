//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_SG_OBJECTIVE_H_
#define FIELDS2COVER_OBJECTIVES_SG_OBJECTIVE_H_

#include "fields2cover/types.h"

namespace f2c::obj {

/// @brief Base class for objective functions of swath generators.
///
/// SG objectives are those objectives that depends on the swaths generation
/// Some of the could be f2c::obj::NSwath or f2c::obj::FieldCoverage
class SGObjective {
 public:
  // Avoid using variadic templates. SWIG don't work well with it

  /// @brief Compute the cost function with minimizing sign.
  ///
  /// If the objective is to maximize the cost function, the cost is
  /// multiplied by -1.
  template <typename T1>
  double computeCostWithMinimizingSign(const T1& t1) const {
    return (isMinimizing() ? 1.0 : -1.0) * computeCost(t1);
  }
  template <typename T1, typename T2>
  double computeCostWithMinimizingSign(const T1& t1, const T2& t2) const {
    return (isMinimizing() ? 1.0 : -1.0) * computeCost(t1, t2);
  }
  template <typename T1, typename T2, typename T3>
  double computeCostWithMinimizingSign(
      const T1& t1, const T2& t2, const T3& t3) const {
    return (isMinimizing() ? 1.0 : -1.0) * computeCost(t1, t2, t3);
  }
  template <typename T1, typename T2, typename T3, typename T4>
  double computeCostWithMinimizingSign(
      const T1& t1, const T2& t2, const T3& t3, const T4& t4) const {
    return (isMinimizing() ? 1.0 : -1.0) * computeCost(t1, t2, t3, t4);
  }

 public:
  /// Return true if the objective is to minimize the cost function
  virtual bool isMinimizing(void) const;
  /// Return true if the objective is to maximize the cost function
  bool isMaximizing(void) const;


 public:
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CSwath&) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CSwaths&) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CSwathsByCells& swaths) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells&) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell&) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CRoute&) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CPath&) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwath& s) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwaths& s) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c,
      const F2CSwathsByCells& swaths) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwath& s) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwaths& s) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c,
      const F2CSwathsByCells& swaths) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CRoute& r, const F2CPath& p) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwath& s,
      const F2CRoute& r) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwath& s,
      const F2CPath& p) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwath& s,
      const F2CRoute& r, const F2CPath& p) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwaths& s,
      const F2CRoute& r) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwaths& s,
      const F2CPath& p) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwaths& s,
      const F2CRoute& r, const F2CPath& p) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwathsByCells& s,
      const F2CRoute& r) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwathsByCells& s,
      const F2CPath& p) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwathsByCells& s,
      const F2CRoute& r, const F2CPath& p) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwath& s,
      const F2CRoute& r) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwath& s,
      const F2CPath& p) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwath& s,
      const F2CRoute& r, const F2CPath& p) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwaths& s,
      const F2CRoute& r) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwaths& s,
      const F2CPath& p) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwaths& s,
      const F2CRoute& r, const F2CPath& p) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwathsByCells& s,
      const F2CRoute& r) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwathsByCells& s,
      const F2CPath& p) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwathsByCells& s,
      const F2CRoute& r, const F2CPath& p) const;
};


}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_SG_OBJECTIVE_H_
