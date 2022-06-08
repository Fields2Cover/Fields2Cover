//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#ifdef F2C_DEF_MATPLOTLIB

#include <matplotlibcpp.h>
#include "fields2cover/utils/visualizer.h"

namespace plt = matplotlibcpp;

namespace f2c {

std::vector<F2CPoint> Visualizer::data2vector(const F2CPoint& _t) {
  return std::vector<F2CPoint>{_t.clone()};
}

void Visualizer::plot(const std::vector<F2CPoint>& _points,
    const std::string& _opts) {
  auto comp = getComponents(_points);
  plt::plot(comp[0], comp[1], _opts);
}

void Visualizer::plot(const F2CPath& _path) {
  plot(_path.points, "k");
}

void Visualizer::plot(const F2CCell& _cell) {
  plot(_cell, "tab:olive");
}

void Visualizer::plot(const F2CCells& _cells) {
  for (auto&& cell : _cells) {
    plot(cell, "tab:olive");
  }
}


void Visualizer::plot(const F2CSwath& _swath, std::string opt) {
  auto comps = getComponents(data2vector(_swath.getPath()));
  plt::plot(comps[0], comps[1], opt);
  plot(F2CPoint(comps[0][0], comps[1][0]), "og");
  plot(F2CPoint(comps[0].back(), comps[1].back()), "kx");
}

void Visualizer::plot(const F2CSwaths& _swaths) {
  auto colors = color_linspace(
      std::vector({0x00,0xff,0x0}),
      std::vector({0x15, 0x0f,0x0b}),
      _swaths.size());
  for (int i = 0; i < _swaths.size(); ++i) {
    plot(_swaths[i], colors[i]);
  }
}

void Visualizer::plot(const F2CMultiLineString& _lines) {
  for (auto&& line : _lines) {
    plot(line);
  }
}

void Visualizer::plot(const F2CRobots& _robots) {
  for (auto&& r : _robots) {
    plot(r);
  }
}

void Visualizer::plot(const F2CRobot& _robot) {
  plot(*_robot.start_point, "bD");
  plot(*_robot.end_point, "r*");
}

void Visualizer::plot(const F2CField& _field) {
  plot(_field.field, "tab:orange");
}

void Visualizer::plot(const F2CFields& _fields) {
  for (auto&& f : _fields) {
    plot(f);
  }
}

void Visualizer::plot(const std::vector<double>& _t,
    const std::vector<double>& _d, const std::string& _opts) {
  plt::plot(_t, _d, _opts);
}

void Visualizer::plot(const std::vector<double>& _d,
    const std::string& _opts) {
  std::vector<double> t(_d.size());
  std::iota(std::begin(t), std::end(t), 0.0);
  plot(t, _d, _opts);
}

void Visualizer::figure(int _id) {
  plt::figure(_id);
}
void Visualizer::figure(void) {
  plt::figure();
}

void Visualizer::show(bool _block) {
  plt::show(_block);
}

void Visualizer::save(const std::string& _file) {
  plt::save(_file);
}

std::vector<std::vector<double>> Visualizer::getComponents(
    const std::vector<F2CPoint>& _points) {
  std::vector<double> x, y;
  std::transform(_points.begin(), _points.end(), std::back_inserter(x),
      [](const F2CPoint& c) -> double { return c.getX();});
  std::transform(_points.begin(), _points.end(), std::back_inserter(y),
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
#endif  // F2C_DEF_MATPLOTLIB
