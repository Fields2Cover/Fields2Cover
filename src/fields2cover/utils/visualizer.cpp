//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include <matplot/matplot.h>
#include "fields2cover/utils/visualizer.h"

namespace plt = matplot;

namespace f2c {

void Visualizer::plot(
    const std::vector<double>& t, const std::vector<double>& d,
    const std::vector<double>&  color) {
  auto a = plt::plot(t, d);
  a->line_width(getLineWidth());
  a->color(plt::to_array(color));
}

void Visualizer::plot(double x, double y, const std::vector<double>& color) {
  auto s = plt::scatter(std::vector<double>({x}), std::vector<double>({y}));
  if (color.size() >= 3) {
    std::array<float, 3> fc;
    for (int i = 0; i < color.size(); ++i) {
      fc[i] = color[i];
    }
    s->marker_color(fc);
  }
}

void Visualizer::plot(const F2CPoint& p, const std::vector<double>& color) {
  plot(p.getX(), p.getY(), color);
}

void Visualizer::plot(
    const F2CSwath& s, const std::vector<double>& color) {
  F2CLineString line = s.getPath();
  size_t n = line.size();
  if (n < 2) {
    return;
  }
  plot(line, color);
  auto a = plt::arrow(
      line.getX(n - 2), line.getY(n - 2),
      line.getX(n - 1), line.getY(n - 1));
  a->line_width(getLineWidth());
  a->color(plt::to_array(color));
}

void Visualizer::plot(
    const F2CSwaths& s, const std::vector<std::vector<double>>& color) {
  auto colors = color;
  if (color.empty()) {
    colors = color_linspace(
        std::vector({0x00, 0xff, 0x0}),
        std::vector({0x15, 0x0f, 0x0b}),
        s.size());
  }
  for (int i = 0; i < s.size(); ++i) {
    plot(s[i], colors[i]);
  }
}

void Visualizer::plot(
    const F2CSwathsByCells& s, const std::vector<std::vector<double>>& color) {
  auto colors = color;
  if (colors.empty()) {
    colors = color_linspace(
        std::vector({0x00, 0xff, 0x0}),
        std::vector({0x15, 0x0f, 0x0b}),
        s.sizeTotal());
  }
  for (int i = 0; i < s.sizeTotal(); ++i) {
    plot(s.getSwath(i), colors[i]);
  }
}

void Visualizer::plot(const std::vector<F2CSwathsByCells>& vs,
    const std::vector<std::vector<double>>& color) {
  std::vector<std::vector<int>> color_palette {
      std::vector({0x00, 0xff, 0x00}), std::vector({0x0f, 0x15, 0x0b}),
      std::vector({0xff, 0x00, 0x00}), std::vector({0x15, 0x0f, 0x0b}),
      std::vector({0x00, 0x00, 0xff}), std::vector({0x0b, 0x0f, 0x15}),
      std::vector({0xff, 0x00, 0xff}), std::vector({0x15, 0x0f, 0x15}),
      std::vector({0x00, 0xff, 0xff}), std::vector({0x00, 0x15, 0x15}),
      std::vector({0xff, 0xff, 0x00}), std::vector({0x15, 0x15, 0x00})
  };
  const size_t CS = color_palette.size();
  for (int i = 0; i < vs.size(); ++i) {
    auto colors = color_linspace(
        color_palette[(2*i)%CS], color_palette[((2*i)+1)%CS],
        vs[i].sizeTotal());
    plot(vs[i], colors);
  }
}

void Visualizer::plot(
    const F2CLineString& line, const std::vector<double>& color) {
  auto comps = getComponents(data2vector(line));
  auto l = plt::plot(comps[0], comps[1]);
  if (color.size() >= 3) {
    l->color(plt::to_array(color));
  }
  l->line_width(getLineWidth());
}

void Visualizer::plot(
    const F2CMultiPoint& multipoint, const std::vector<double>& color) {
  for (size_t i = 0; i < multipoint.size(); ++i) {
    plot(multipoint.getGeometry(i), color);
  }
}

void Visualizer::plot(
    const F2CMultiLineString& multiline, const std::vector<double>& color) {
  for (size_t i = 0; i < multiline.size(); ++i) {
    plot(multiline.getGeometry(i), color);
  }
}

void Visualizer::plot(
    const F2CLinearRing& ring, const std::vector<double>& color) {
  plot(F2CLineString(ring), color);
}

void Visualizer::plot(
    const F2CCell& cell, const std::vector<double>& color) {
  for (auto&& r : cell) {
    plot(r, color);
  }
}

void Visualizer::plot(
    const F2CCells& cells, const std::vector<double>& color) {
  for (auto&& c : cells) {
    plot(c, color);
  }
}


void Visualizer::plot(const std::vector<F2CRoute>& route,
    const std::vector<std::vector<double>>& color) {
  std::vector<F2CSwathsByCells> swaths;
  for (auto&& r : route) {
    plot(r.asLineString());
    swaths.emplace_back(r.getVectorSwaths());
  }
  plot(swaths, color);
}
void Visualizer::plot(const F2CRoute& route,
    const std::vector<std::vector<double>>& color) {
  plot(route.asLineString());
  plot(F2CSwathsByCells(route.getVectorSwaths()), color);
}

void Visualizer::plot(
    const F2CPath& path, const std::vector<std::vector<double>>& color) {
  if (path.size() <= 1) {
    return;
  }

  std::vector<std::vector<double>> X(1), Y(1);
  auto prev_state = path[0].type;
  auto prev_dir = path[0].dir;
  for (auto&& s : path) {
    X.back().emplace_back(s.point.getX());
    Y.back().emplace_back(s.point.getY());
    X.back().emplace_back(s.atEnd().getX());
    Y.back().emplace_back(s.atEnd().getY());
    if (s.type != prev_state || s.dir != prev_dir) {
      X.emplace_back();
      Y.emplace_back();
      prev_state = s.type;
      prev_dir = s.dir;
    }
  }

  auto colors = color;
  if (color.empty()) {
    colors = color_linspace(
        std::vector({0x00, 0xff, 0x0}),
        std::vector({0x15, 0x0f, 0x0b}),
        X.size());
  }

  for (size_t i = 0; i < X.size(); ++i) {
    auto a = plt::plot(X[i], Y[i]);
    a->color(plt::to_array(colors[i]));
    a->line_width(getLineWidth());
  }
}

void Visualizer::plot(const F2CField& field,
    const std::vector<double>& color) {
  plot(field.getField(), color);
}
void Visualizer::plotFilled(const F2CField& field,
    const std::vector<double>& color) {
  plot(field.getField(), color);
}


void Visualizer::plotFilled(const F2CCells& cells,
    const std::vector<double>& poly_color) {
  for (auto&& c : cells) {
    plotFilled(c, poly_color, {1.0, 1.0, 1.0});
  }
}

void Visualizer::plotFilled(const F2CCells& cells,
    const std::vector<double>& poly_color,
    const std::vector<double>& holes_color) {
  for (auto&& c : cells) {
    plotFilled(c, poly_color, holes_color);
  }
}

void Visualizer::plotFilled(const F2CCell& cell,
    const std::vector<double>& poly_color) {
  plotFilled(cell.getExteriorRing(), poly_color);
}

void Visualizer::plotFilled(const F2CCell& cell,
    const std::vector<double>& poly_color,
    const std::vector<double>& holes_color) {
  if (cell.size() < 1) {
    return;
  }
  plotFilled(cell.getExteriorRing(), poly_color);
  for (size_t i = 0; i < cell.size() - 1; ++i) {
    plotFilled(cell.getInteriorRing(i), holes_color);
  }
}

void Visualizer::plotFilled(const F2CLinearRing& ring,
    const std::vector<double>& color) {
  std::vector<double> b_x, b_y;
  for (auto&& p : ring) {
    b_x.emplace_back(p.getX());
    b_y.emplace_back(p.getY());
  }
  auto l = plt::polygon(b_x, b_y)->color(plt::to_array(color));
  l.line_width(getLineWidth());
}


void Visualizer::axis_equal() {
  plt::axis(plt::equal);
}

void Visualizer::figure() {
  plt::figure(true);
  plt::hold(plt::on);
  axis_equal();
}

void Visualizer::figure_size(const unsigned int width, const unsigned int height) {
  auto cf = plt::gcf(true);
  if (cf != nullptr) {
    cf->width(width);
    cf->height(height);
  }
}

void Visualizer::show() {
  plt::show();
}

void Visualizer::save(const std::string& file) {
  plt::save(file);
}

void Visualizer::title(const std::string& text) {
  plt::title(text);
}

void Visualizer::xlim(double min, double max) {
  plt::xlim({min, max});
}

void Visualizer::ylim(double min, double max) {
  plt::ylim({min, max});
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

std::vector<std::vector<double>> Visualizer::color_linspace(
    const std::vector<int>& min, const std::vector<int>& max,
    size_t N) {
  auto v_r = linspace(min[0], max[0], N);
  auto v_g = linspace(min[1], max[1], N);
  auto v_b = linspace(min[2], max[2], N);
  std::vector<std::vector<double>> v;
  for (size_t i = 0; i < N; ++i) {
    v.emplace_back(
        std::vector<double>({v_r[i]/255.0, v_g[i]/255.0, v_b[i]/255.0}));
  }
  return v;
}


}  // namespace f2c
