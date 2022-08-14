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
#include "fields2cover/types.h"

namespace f2c {

/// Class to plot Fields2Cover data structures
class Visualizer {
 public:
  /// Plot a vector of points
  /// @param points Plotted points
  /// @param opts Options used to plot (from matplotlib library)
  static void plot(const std::vector<F2CPoint>& points,
      const std::string& opts = "");
  /// Plot a path
  static void plot(const F2CPath& path);
  static void plot(const F2CMultiLineString& lines);
  /// Plot a cell
  static void plot(const F2CCell& cell);
  /// Plot cells
  static void plot(const F2CCells& cells);
  /// Plot swaths
  static void plot(const F2CSwaths& swaths);
  /// Plot a swath
  static void plot(const F2CSwath& swath, std::string opt = "");
  /// Plot a field
  static void plot(const F2CField& field);
  /// Plot a vector of fields
  static void plot(const F2CFields& fields);
  /// Plot starting and end point of a robot
  static void plot(const F2CRobot& robot);
  /// Plot starting and end point of several robots
  static void plot(const F2CRobots& robots);

  static void plot(const std::vector<double>& t,
      const std::vector<double>& d, const std::string& opts = "");
  static void plot(
      const std::vector<double>& d, const std::string& opts = "");

  /// Plot type T
  /// @param opts Options used to plot (from matplotlib library)
  template<class T>
  static void plot(const T& t, const std::string& opts);

  /// Plot vector of type T
  template<class T>
  static void plot(const std::vector<T>& v_t);

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

 private:
  template<class T>
  static std::vector<F2CPoint> data2vector(const T& t);

  static std::vector<F2CPoint> data2vector(const F2CPoint& t);

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
  std::vector<F2CPoint> res;
  for (auto&& i : t) {
    auto v = data2vector(i);
    res.insert(res.end(), v.begin(), v.end());
  }
  return res;
}

template<class T>
void Visualizer::plot(const std::vector<T>& v_t) {
  for (auto&& t : v_t) {
    plot(t);
  }
}

template<class T>
void Visualizer::plot(const T& t, const std::string& opts) {
  plot(data2vector(t), opts);
}

template<class T>
void Visualizer::plot(const T& t) {
  plot(t, "");
}

}  // namespace f2c


#endif  // FIELDS2COVER_UTILS_VISUALIZER_H_
