//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <math.h>
#include <functional>
#include <vector>
#include <utility>
#include <iostream>
#include <cstring>
#include "fields2cover/utils/solver.h"

namespace f2c::Solver {

double findZeroBisectionMethod(
    const std::function<double(double)>& func,
    double low, double high, double tolerance) {
  while (high - low > tolerance) {
    double mid = 0.5 * (high + low);
    if (func(mid) * func(high) < 0) {
      low = mid;
    } else {
      high = mid;
    }
  }
  return (low + high) * 0.5;
}

double findMinGoldenSection(
    const std::function<double(double)>& func,
    double low, double high, double tolerance) {
  const double goldenRatio = 0.5 * (1.0 + sqrt(5.0));
  double c = high - (high - low) / goldenRatio;
  double d = low + (high - low) / goldenRatio;
  while (fabs(c - d) > tolerance) {
    if (func(c) < func(d)) {
      high = d;
    } else {
      low = c;
    }
    c = high - (high - low) / goldenRatio;
    d = low + (high - low) / goldenRatio;
  }
  return (low + high) * 0.5;
}


std::pair<double, double> findMinFunc(
    const std::function<double(double)>& f, double x0) {
  const double h = 1e-7;
  const double error = 1e-5;
  const size_t n_it = 25;
  double t = x0;
  double f_t;
  for (size_t i = 0; i < n_it; ++i) {
    f_t = f(t);
    double f_th = f(t + h);
    double df = (f_th - f_t)/ h;
    if (std::fabs(df) < error) {
      return {f_t, t};
    }
    t -= df;
  }
  return {f_t, t};
}

std::pair<double, double> findMinFunc3(
  const std::function<double(double)>& f, double a, double b, size_t n_it) {
  double x = (a + b) / 2;
  for (size_t i = 0; i < n_it; ++i) {
    x += ((f(x+1e-5) - f(x) > 0) ? -1.0: 1.0) * ((a + b)/(4 << i));
  }
  return {f(x), x};
}

std::pair<double, double> findMinFunc2(
    const std::function<double(double)>& f, double a, double b, size_t v) {
  double res_min = 1e7;
  double j_min {a};
  if (a > b) {
    a = b;
    b = j_min;
  }
  for (double i = 0.0; i < 1.0; i+=0.05) {
    double j = i * (b - a) + a;
    double res_j = f(j);
    if (res_min >= res_j) {
      res_min = res_j;
      j_min = j;
    }
  }
  return {res_min, j_min};
}

double findSolFunc(const std::function<double(double)>& f,
    double min_x, double max_x, double y_ref) {
  double sol = nonlinearNewton(
      [&f, y_ref] (double t) {return y_ref - f(t);}, 0.5 * (min_x + max_x));
  return std::min(max_x, std::max(min_x, sol));
}

double nonlinearNewton(const std::function<double(double)>& f, double x0) {
  const double h = 1e-5;
  const double error = 1e-5;
  const size_t n_it = 25;
  double t = x0;
  for (size_t i = 0; i < n_it; ++i) {
    double f_t = f(t);
    if (std::fabs(f_t) < error) {
      return t;
    }
    double df = (f(t + h) - f_t)/ h;
    t -= f_t / df;
  }
  return t;
}

std::vector<double> exhaustive_solve(
    const std::vector<double>& x_min,
    const std::vector<double>& x_max,
    std::function<double(const std::vector<double>&)> Ef,
    const std::vector<double>& step_size) {
  std::vector<size_t> N(step_size.size());
  std::vector<size_t> I(step_size.size());
  for (size_t i = 0; i < N.size(); ++i) {
    N[i] = floor((x_max[i] - x_min[i]) / step_size[i]);
  }

  std::vector<double> x(step_size.size());
  std::vector<double> best_x;
  double best_E {1e20};
  while (I.back() <= N.back()) {
    for (size_t i = 0; i < N.size(); ++i) {
      x[i] = x_min[i] + ((x_max[i] - x_min[i]) * I[i])/N[i];
    }
    double E = Ef(x);
    if (E < best_E) {
      best_E = E;
      best_x = x;
    }

    ++I[0];
    for (size_t i = 1; i < N.size(); ++i) {
      if (I[i-1] > N[i-1]) {
        I[i-1] = 0;
        ++I[i];
      }
    }
  }
  if (best_E > 1e9) {
    return x_max;
  }

  return best_x;
}

std::vector<double> siman_solve(
    f2c::Random& r, const std::vector<double>& x0,
    const std::vector<double>& x_min,
    const std::vector<double>& x_max,
    std::function<double(const std::vector<double>&)> Ef,
    int max_iters, double step_size) {
  return siman_solve<std::vector<double>>(r, x0, Ef, [&x_min, &x_max] (
        f2c::Random& r, const std::vector<double>& x, double step_size) {
          std::vector<double> new_x;
          for (size_t i = 0; i < x.size(); ++i) {
            double step = step_size * 0.5 * (x_max[i] - x_min[i]);
            double new_xi = r.getRandomLinear(-step, step) + x[i];
            new_x.emplace_back(std::max(std::min(new_xi, x_max[i]), x_min[i]));
          }
          return new_x;
        }, max_iters, step_size);
}







}  // namespace f2c::Solver


