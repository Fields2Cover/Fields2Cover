//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <algorithm>
#ifdef ALLOW_PARALLELIZATION
#include <execution>
#endif
#include <vector>
#include <utility>
#include <limits>
#include "fields2cover/swath_generator/oksanen_heuristic.h"

namespace f2c::sg {

double OksanenHeuristic::getStepAngle() const {
  return this->step_angle_;
}

void OksanenHeuristic::setStepAngle(double step_angle) {
  this->step_angle_ = step_angle;
}

double OksanenHeuristic::getRatioStepAngle() const {
  return this->ratio_step_angle_;
}

void OksanenHeuristic::setRatioStepAngle(double ratio_step_angle) {
  this->ratio_step_angle_ = ratio_step_angle;
}

size_t OksanenHeuristic::getMaxIt() const {
  return this->max_it_;
}

void OksanenHeuristic::setMaxIt(size_t max_it) {
  this->max_it_ = max_it;
}

size_t OksanenHeuristic::getNumBestSelected() const {
  return this->n_best_select_;
}

void OksanenHeuristic::setNumBestSelected(size_t n) {
  this->n_best_select_ = n;
}

std::vector<double> OksanenHeuristic::getStepAngleSeq() const {
  std::vector<double> seq;
  double a = this->step_angle_;
  for (size_t i = 0; i < max_it_; ++i) {
    seq.emplace_back(a);
    a *= this->ratio_step_angle_;
  }
  return seq;
}

double OksanenHeuristic::getFinalStepAngle() const {
  return this->step_angle_ * std::pow(this->ratio_step_angle_, max_it_ - 1);
}

size_t OksanenHeuristic::getTimesComp() const {
  return floor(M_PI/this->step_angle_) + 2 * n_best_select_ * (max_it_ - 1);
}


double OksanenHeuristic::computeBestAngle(const f2c::obj::SGObjective& obj,
    double op_width, const F2CCell& poly, SwathOverlapType dist) const {
  auto getCostSwaths = [this, op_width, &poly, &obj, dist] (double ang) {
    return std::make_pair(
        ang, this->computeCostOfAngle(obj, ang, op_width, poly, dist));
  };
  std::vector<std::pair<double, double>> angs;
  for (double a = 0.0; a < M_PI; a += this->step_angle_) {
    angs.emplace_back(getCostSwaths(a));
  }
  double step_ang = this->ratio_step_angle_ * this->step_angle_;
  for (size_t i = 0; i < this->max_it_; ++i) {
    const size_t n_sel = (n_best_select_ < angs.size()) ?
        (this->n_best_select_ > 0 ?
             this->n_best_select_ : 1) :
        (angs.size() - 1);
    std::nth_element(angs.begin(), angs.begin() + n_sel-1, angs.end(),
        [] (const std::pair<double, double>& a,
            const std::pair<double, double>& b) {
          return a.second < b.second;
        });
    angs.erase(angs.begin() + n_sel, angs.end());
    step_ang *= this->ratio_step_angle_;
    for (auto&& ang : angs) {
      angs.emplace_back(getCostSwaths(ang.first - step_ang));
      angs.emplace_back(getCostSwaths(ang.first + step_ang));
    }
  }
  std::nth_element(angs.begin(), angs.begin() + 1, angs.end(),
      [] (const std::pair<double, double>& a,
          const std::pair<double, double>& b) {
        return a.second < b.second;
      });
  return angs[0].first;
}

double OksanenHeuristic::computeBestAngle(const f2c::obj::SGObjective& obj,
    const std::vector<double>& widths, const F2CCell& poly) const {
  auto getCostSwaths = [this, widths, &poly, &obj] (double ang) {
    return std::make_pair(
        ang, this->computeCostOfAngle(obj, ang, widths, poly));
  };
  std::vector<std::pair<double, double>> angs;
  for (double a = 0.0; a < M_PI; a += this->step_angle_) {
    angs.emplace_back(getCostSwaths(a));
  }
  double step_ang = this->ratio_step_angle_ * this->step_angle_;
  for (size_t i = 0; i < this->max_it_; ++i) {
    const size_t n_sel = (n_best_select_ < angs.size()) ?
        (this->n_best_select_ > 0 ?
             this->n_best_select_ : 1) :
        (angs.size() - 1);
    std::nth_element(angs.begin(), angs.begin() + n_sel-1, angs.end(),
        [] (const std::pair<double, double>& a,
            const std::pair<double, double>& b) {
          return a.second < b.second;
        });
    angs.erase(angs.begin() + n_sel, angs.end());
    step_ang *= this->ratio_step_angle_;
    for (auto&& ang : angs) {
      angs.emplace_back(getCostSwaths(ang.first - step_ang));
      angs.emplace_back(getCostSwaths(ang.first + step_ang));
    }
  }
  std::nth_element(angs.begin(), angs.begin() + 1, angs.end(),
      [] (const std::pair<double, double>& a,
          const std::pair<double, double>& b) {
        return a.second < b.second;
      });
  return angs[0].first;
}



}  // namespace f2c::sg

