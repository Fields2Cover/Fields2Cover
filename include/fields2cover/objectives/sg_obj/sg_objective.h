//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_OBJECTIVES_SG_OBJECTIVE_H_
#define FIELDS2COVER_OBJECTIVES_SG_OBJECTIVE_H_

#include "fields2cover/types.h"
#include "fields2cover/objectives/base_objective.h"


namespace f2c::obj {

/// @brief Base class for objective functions of swath generators.
///
/// SG objectives are those objectives that depends on the swaths generation
/// Some of the could be f2c::obj::NSwath or f2c::obj::FieldCoverage
class SGObjective : public BaseObjective<SGObjective> {
 public:
  virtual bool isFastCompAvailable() const {return false;}

  /// @brief Compute the cost function.
  virtual double computeCost(const F2CSwath&) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CSwaths&) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CSwathsByCells& swaths) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell&) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells&) const;
  /// @brief Compute the cost function.
  virtual double computeCost(double ang, double op_width,
      const F2CCell& cell) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwath& s) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwaths& s) const;
  virtual double computeCost(const F2CCell& c, const std::vector<F2CSwaths>& s) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c,
      const F2CSwathsByCells& swaths) const;
  virtual double computeCost(const F2CCell& c,
      const std::vector<F2CSwathsByCells>& swaths) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwath& s) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwaths& s) const;
  virtual double computeCost(const F2CCells& c,
      const std::vector<F2CSwaths>& s) const;
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c,
      const F2CSwathsByCells& swaths) const;
  virtual double computeCost(const F2CCells& c,
      const std::vector<F2CSwathsByCells>& swaths) const;
};


}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_SG_OBJECTIVE_H_
