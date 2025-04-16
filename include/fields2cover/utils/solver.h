//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_UTILS_SOLVER_H_
#define FIELDS2COVER_UTILS_SOLVER_H_

#include <functional>
#include <utility>
#include <tuple>
#include <vector>
#include "fields2cover/utils/random.h"

namespace f2c::Solver {

double findZeroBisectionMethod(
  const std::function<double(double)>& func,
  double low, double high, double tolerance);
double findMinGoldenSection(
  const std::function<double(double)>& func,
  double low, double high, double tolerance = 1e-5);

std::pair<double, double> findMinFunc(
  const std::function<double(double)>& f, double x0);

std::pair<double, double> findMinFunc3(
  const std::function<double(double)>& f, double a, double b, size_t n_it);

std::pair<double, double> findMinFunc2(
    const std::function<double(double)>& f, double a, double b, size_t v);

double findSolFunc(const std::function<double(double)>& f,
    double min_x, double max_x, double y_ref);

double nonlinearNewton(const std::function<double(double)>& f, double x0);


std::vector<double> exhaustive_solve(
    const std::vector<double>& x_min,
    const std::vector<double>& x_max,
    std::function<double(const std::vector<double>&)> Ef,
    const std::vector<double>& step_size);

std::vector<double> siman_solve(
    f2c::Random& r, const std::vector<double>& x0,
    const std::vector<double>& x_min,
    const std::vector<double>& x_max,
    std::function<double(const std::vector<double>&)> Ef,
    int max_iters = 1000, double step_size = 1.0);

template <class T, typename TEf, typename TSf>
T siman_solve(f2c::Random& r, const T& x0, TEf&& Ef, TSf&& Sf,
    int max_iters, double step_size = 1.0,
    double k = 1.0, int iters_fixed_t = 100,  double t_initial = 0.008,
    double mu_t = 1.003, double t_min = 2e-6);

}  // namespace f2c::Solver


// Code converted to C++ from https://github.com/ampl/gsl/blob/master/siman/siman.c
template <class T, typename TEf, typename TSf>
T f2c::Solver::siman_solve(
    f2c::Random& r,
    const T& x0,
    TEf&& Ef,  // Energy function
    TSf&& Sf,  // Next step
    int max_iters, double step_size, double k, int iters_fixed_t,
    double t_initial, double mu_t, double t_min) {
  T      x {x0};     T      best_x {x0}; T      new_x;
  double E {Ef(x0)}; double best_E {E};  double new_E;

  double t = t_initial;
  double t_factor = 1.0 / mu_t;

  int iters {0};

  while (t >= t_min) {
    if (iters > max_iters) { break; }
    for (int i = 0; i < iters_fixed_t; ++i) {
      if (iters > max_iters) { break; }
      ++iters;

      new_x = Sf(r, x, step_size);
      new_E = Ef(new_x);
      if (new_E <= best_E) {
        x = best_x = new_x;
        E = best_E = new_E;
      } else {
        double delta = new_E - E;
        if (delta > 10.0 * t) {continue;}
        double rand = r.getRandomLinear(0.0, 1.0);
        if (rand < exp(-delta / (k * t))) {
          x = new_x;
        }
      }
    }
    t *= t_factor;
  }
  return best_x;
}

#endif  // FIELDS2COVER_UTILS_SOLVER_H_
