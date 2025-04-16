//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_SWATH_GENERATOR_OKSANEN_HEURISTIC_H_
#define FIELDS2COVER_SWATH_GENERATOR_OKSANEN_HEURISTIC_H_

#include <vector>
#include "fields2cover/swath_generator/swath_generator_base.h"

namespace f2c::sg {

// \cite{oksanen2007path}
class OksanenHeuristic : public SwathGeneratorBase {
 public:
  using SwathGeneratorBase::generateBestSwaths;
  using SwathGeneratorBase::generateSwaths;
  using SwathGeneratorBase::computeBestAngle;
  using SwathGeneratorBase::computeBestAngles;

  double getStepAngle() const;
  void setStepAngle(double step_ang);
  double getRatioStepAngle() const;
  void setRatioStepAngle(double ratio_step_ang);
  size_t getMaxIt() const;
  void setMaxIt(size_t max_it);
  size_t getNumBestSelected() const;
  void setNumBestSelected(size_t n);

  // Returns the sequence of values of step angle on each iteration.
  std::vector<double> getStepAngleSeq() const;
  double getFinalStepAngle() const;
  // Returns number of times the obj function is computed
  size_t getTimesComp() const;

  double computeBestAngle(const f2c::obj::SGObjective& obj,
      double op_width, const F2CCell& poly,
      SwathOverlapType dist) const override;
  double computeBestAngle(const f2c::obj::SGObjective& obj,
      const std::vector<double>& widths, const F2CCell& poly) const override;

 private:
  double step_angle_ {30.0 * M_PI/180.0};  // radians
  double ratio_step_angle_ {0.5};  // \in (0, 1)
  size_t max_it_ {5};
  size_t n_best_select_ {3};
};


}  // namespace f2c::sg



#endif  // FIELDS2COVER_SWATH_GENERATOR_OKSANEN_HEURISTIC_H_
