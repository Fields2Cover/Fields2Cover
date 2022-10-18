//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
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
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CSwath&);
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CSwaths&);
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CSwathsByCells& swaths);
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwath& s);
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c, const F2CSwaths& s);
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCell& c,
      const F2CSwathsByCells& swaths);
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwath& s);
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c, const F2CSwaths& s);
  /// @brief Compute the cost function.
  virtual double computeCost(const F2CCells& c,
      const F2CSwathsByCells& swaths);
};


}  // namespace f2c::obj

#endif  // FIELDS2COVER_OBJECTIVES_SG_OBJECTIVE_H_
