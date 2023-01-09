//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include <matplotlibcpp.h>
#include "fields2cover/utils/visualizer.h"

namespace plt = matplotlibcpp;

namespace f2c {

void Visualizer::plot(const std::vector<double>& t,
    const std::vector<double>& d, const std::string& opts) {
  plt::plot(t, d, opts);
}

void Visualizer::figure(int id) {
  plt::figure(id);
}

void Visualizer::figure() {
  plt::figure();
}

void Visualizer::show(bool block) {
  plt::show(block);
}

void Visualizer::save(const std::string& file) {
  plt::save(file);
  plt::close();
}


void Visualizer::title(const std::string& text) {
  plt::title(text);
}

void Visualizer::xlim(double min, double max) {
  plt::xlim(min, max);
}

void Visualizer::ylim(double min, double max) {
  plt::ylim(min, max);
}

void Visualizer::axis(const std::string& opt) {
  plt::axis(opt);
}



std::vector<std::vector<double>> Visualizer::getComponents(
    const std::vector<F2CPoint>& points) {
  std::vector<double> x, y;
  std::transform(points.begin(), points.end(), std::back_inserter(x),
      [](const F2CPoint& c) -> double { return c.getX();});
  std::transform(points.begin(), points.end(), std::back_inserter(y),
      [](const F2CPoint& c) -> double { return c.getY();});
  return {x, y};
}

std::vector<double> Visualizer::linspace(double min, double max, size_t N) {
  double h = (max - min) / (N - 1.0);
    std::vector<double> x(N);
    for (size_t i = 0; i < N; ++i) {
      x[i] = min + i * h;
    }
    return x;
}

std::string Visualizer::color2hex(uint32_t r, uint32_t g, uint32_t b) {
  char hex[16];
  snprintf(hex, sizeof hex, "#%02x%02x%02x", r, g, b);
  return std::string(hex);
}

std::vector<std::string> Visualizer::color_linspace(
    const std::vector<int>& min, const std::vector<int>& max,
    size_t N) {
  auto v_r = linspace(min[0], max[0], N);
  auto v_g = linspace(min[1], max[1], N);
  auto v_b = linspace(min[2], max[2], N);
  std::vector<std::string> hexs;
  for (size_t i = 0; i < N; ++i) {
    hexs.push_back(color2hex(v_r[i], v_g[i], v_b[i]));
  }
  return hexs;
}


}  // namespace f2c
