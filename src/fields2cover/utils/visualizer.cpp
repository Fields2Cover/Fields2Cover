//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include <matplotlibcpp.h>
#include "fields2cover/utils/visualizer.h"

namespace plt = matplotlibcpp;

namespace f2c {

std::vector<F2CPoint> Visualizer::data2vector(const F2CPoint& t) {
  return std::vector<F2CPoint>{t.clone()};
}

void Visualizer::plot(const std::vector<F2CPoint>& points,
    const std::string& opts) {
  auto comp = getComponents(points);
  plt::plot(comp[0], comp[1], opts);
}

void Visualizer::plot(const F2CPath& path) {
  plot(path.points, "k");
}

void Visualizer::plot(const F2CCell& cell) {
  plot(cell, "tab:olive");
}

void Visualizer::plot(const F2CCells& cells) {
  for (auto&& cell : cells) {
    plot(cell, "tab:olive");
  }
}


void Visualizer::plot(const F2CSwath& swath, std::string opt) {
  auto comps = getComponents(data2vector(swath.getPath()));
  plt::plot(comps[0], comps[1], opt);
  plot(F2CPoint(comps[0][0], comps[1][0]), "og");
  plot(F2CPoint(comps[0].back(), comps[1].back()), "kx");
}

void Visualizer::plot(const F2CSwathsByCells& swaths) {
  for (int i = 0; i < swaths.size(); ++i) {
    plot(swaths[i]);
  }
}

void Visualizer::plot(const F2CSwaths& swaths) {
  auto colors = color_linspace(
      std::vector({0x00, 0xff, 0x0}),
      std::vector({0x15, 0x0f, 0x0b}),
      swaths.size());
  for (int i = 0; i < swaths.size(); ++i) {
    plot(swaths[i], colors[i]);
  }
}

void Visualizer::plot(const F2CMultiLineString& lines) {
  for (auto&& line : lines) {
    plot(line);
  }
}

void Visualizer::plot(const F2CRobots& robots) {
  for (auto&& r : robots) {
    plot(r);
  }
}

void Visualizer::plot(const F2CRobot& robot) {
  plot(*robot.start_point, "bD");
  plot(*robot.end_point, "r*");
}

void Visualizer::plot(const F2CField& field) {
  plot(field.field, "tab:orange");
}

void Visualizer::plot(const F2CFields& fields) {
  for (auto&& f : fields) {
    plot(f);
  }
}

void Visualizer::plot(const std::vector<double>& t,
    const std::vector<double>& d, const std::string& opts) {
  plt::plot(t, d, opts);
}

void Visualizer::plot(const std::vector<double>& d,
    const std::string& opts) {
  std::vector<double> t(d.size());
  std::iota(std::begin(t), std::end(t), 0.0);
  plot(t, d, opts);
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
