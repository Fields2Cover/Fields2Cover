//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_UTILS_VISUALIZER_H_
#define FIELDS2COVER_UTILS_VISUALIZER_H_


#include <vector>
#include <string>
#include <type_traits>
#include "fields2cover/types.h"

namespace f2c {

/// Class to plot Fields2Cover data structures
class Visualizer {
 public:
  static void plot(const std::vector<double>& t,
      const std::vector<double>& d, const std::string& opts = "");

  /// Plot type T
  /// @param opts Options used to plot (from matplotlib library)
  template<class T>
  static void plot(const T& t, const std::string& opts);

  /// Plot vector of type T
  template<class T>
  static void plot(const std::vector<T>& v_t, const std::string& opts = "");

  /// Plot type T
  template<class T>
  static void plot(const T& t);

  /// Create figure to plot on
  static void figure();

  /// Create a figure to plot on
  /// @param id Identifier of the figure
  static void figure(int id);

  /// Show the figure on screen
  /// @param block if true the program will wait until the screen is closed
  /// to continue
  static void show(bool block = true);

  /// Save figure to a file
  /// @param file Name of the file
  static void save(const std::string& file);

  /// Add title to the figure
  /// @param text Title text
  static void title(const std::string& text);

  static void xlim(double min, double max);
  static void ylim(double min, double max);
  static void axis(const std::string& opt);

 private:
  template<class T>
  static std::vector<F2CPoint> data2vector(const T& t);

  static std::vector<std::vector<double>> getComponents(
      const std::vector<F2CPoint>& points);

  static std::vector<double> linspace(double min, double max, size_t N);
  static std::string color2hex(uint32_t r, uint32_t g, uint32_t b);
  static std::vector<std::string> color_linspace(
      const std::vector<int>& min,
      const std::vector<int>& max,
      size_t N);
};

template<class T>
std::vector<F2CPoint> Visualizer::data2vector(const T& t) {
  if constexpr (std::is_same<T, F2CPoint>::value) {
    return std::vector<F2CPoint>{t.clone()};
  } else {
    std::vector<F2CPoint> res;
    for (auto&& i : t) {
      auto v = data2vector(i);
      res.insert(res.end(), v.begin(), v.end());
    }
    return res;
  }
}

template<class T>
void Visualizer::plot(const std::vector<T>& v_t, const std::string& opts) {
  if constexpr (std::is_same<T, F2CPoint>::value) {
    auto comp = getComponents(v_t);
    plot(comp[0], comp[1], opts);
  } else if constexpr (std::is_same<T, double>::value) {
    std::vector<double> t(v_t.size());
    std::iota(std::begin(t), std::end(t), 0.0);
    plot(t, v_t, opts);
  } else if constexpr (std::is_same<T, F2CRobot>::value) {
    for (auto&& t : v_t) {
      plot(t);
    }
  } else {
    for (auto&& t : v_t) {
      plot(t, opts);
    }
  }
}

template<class T>
void Visualizer::plot(const T& t, const std::string& opts) {
  if constexpr (std::is_same<T, F2CCell>::value ||
      std::is_same<T, F2CCells>::value ||
      std::is_same<T, F2CMultiLineString>::value ||
      std::is_same<T, F2CRobots>::value ||
      std::is_same<T, F2CFields>::value) {
    for (auto&& t_i : t) {
      plot(t_i, opts);
    }
  } else if constexpr (std::is_same<T, F2CSwathsByCells>::value ||
      std::is_same<T, F2CRobots>::value) {
    for (auto&& t_i : t) {
      plot(t_i);
    }
  } else if constexpr (std::is_same<T, F2CField>::value) {
    plot(t.field, opts);
  } else if constexpr (std::is_same<T, F2CSwath>::value) {
    auto comps = getComponents(data2vector(t.getPath()));
    plot(comps[0], comps[1], opts);
    plot(F2CPoint(comps[0][0], comps[1][0]), ".g");
    plot(F2CPoint(comps[0].back(), comps[1].back()), ".k");
  } else if constexpr (std::is_same<T, F2CSwaths>::value) {
    auto colors = color_linspace(
        std::vector({0x00, 0xff, 0x0}),
        std::vector({0x15, 0x0f, 0x0b}),
        t.size());
    for (int i = 0; i < t.size(); ++i) {
      plot(t[i], colors[i]);
    }
  } else if constexpr (std::is_same<T, F2CPath>::value) {
    std::vector<F2CPoint> points;
    for (auto&& s : t.states) {
      points.emplace_back(s.point);
    }
    plot(points, "k");
  } else if constexpr (std::is_same<T, F2CRobot>::value) {
    plot(*t.start_point, "bD");
    plot(*t.end_point, "r*");
  } else if constexpr (std::is_same<T, double>::value) {
    // Don't do anything
  } else {
    plot(data2vector(t), opts);
  }
}

template<class T>
void Visualizer::plot(const T& t) {
  plot(t, "");
}

}  // namespace f2c


#endif  // FIELDS2COVER_UTILS_VISUALIZER_H_
