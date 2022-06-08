//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_UTILS_VISUALIZER_H_
#define FIELDS2COVER_UTILS_VISUALIZER_H_

#ifdef F2C_DEF_MATPLOTLIB

#include <vector>
#include <string>
#include "fields2cover/types.h"

namespace f2c {

/// Class to plot Fields2Cover data structures
class Visualizer {
 public:
  /// Plot a vector of points
  /// @param _points Plotted points
  /// @param _opts Options used to plot (from matplotlib library)
  static void plot(const std::vector<F2CPoint>& _points,
      const std::string& _opts = "");
  /// Plot a path
  static void plot(const F2CPath& _path);
  static void plot(const F2CMultiLineString& _lines);
  /// Plot a cell
  static void plot(const F2CCell& _cell);
  /// Plot cells
  static void plot(const F2CCells& _cells);
  /// Plot swaths
  static void plot(const F2CSwaths& _swaths);
  /// Plot a swath
  static void plot(const F2CSwath& _swath, std::string opt = "");
  /// Plot a field
  static void plot(const F2CField& _field);
  /// Plot a vector of fields
  static void plot(const F2CFields& _fields);
  /// Plot starting and end point of a robot
  static void plot(const F2CRobot& _robot);
  /// Plot starting and end point of several robots
  static void plot(const F2CRobots& _robots);

  static void plot(const std::vector<double>& _t,
      const std::vector<double>& _d, const std::string& _opts = "");
  static void plot(
      const std::vector<double>& _d, const std::string& _opts = "");

  /// Plot type T
  /// @param _opts Options used to plot (from matplotlib library)
  template<class T>
  static void plot(const T& _t, const std::string& _opts);

  /// Plot vector of type T
  template<class T>
  static void plot(const std::vector<T>& _t);

  /// Plot type T
  template<class T>
  static void plot(const T& _t);

  /// Create figure to plot on
  static void figure(void);

  /// Create a figure to plot on
  /// @param _id Identifier of the figure
  static void figure(int _id);

  /// Show the figure on screen
  /// @param _block if true the program will wait until the screen is closed
  /// to continue
  static void show(bool _block = true);

  /// Save figure to a file
  /// @param _file Name of the file
  static void save(const std::string& _file);

 private:
  template<class T>
  static std::vector<F2CPoint> data2vector(const T& _t);

  static std::vector<F2CPoint> data2vector(const F2CPoint& _t);

  static std::vector<std::vector<double>> getComponents(
      const std::vector<F2CPoint>& _points);

  static std::vector<double> linspace(double min, double max, size_t N);
  static std::string color2hex(uint32_t r, uint32_t g, uint32_t b);
  static std::vector<std::string> color_linspace(
      const std::vector<int>& min,
      const std::vector<int>& max,
      size_t N);
};

template<class T>
std::vector<F2CPoint> Visualizer::data2vector(const T& _t) {
  std::vector<F2CPoint> res;
  for (auto&& i : _t) {
    auto v = data2vector(i);
    res.insert(res.end(), v.begin(), v.end());
  }
  return res;
}

template<class T>
void Visualizer::plot(const std::vector<T>& _t) {
  for (auto&& t : _t) {
    plot(t);
  }
}

template<class T>
void Visualizer::plot(const T& _t, const std::string& _opts) {
  plot(data2vector(_t), _opts);
}

template<class T>
void Visualizer::plot(const T& _t) {
  plot(_t, "");
}

}  // namespace f2c


#endif  // F2C_DEF_MATPLOTLIB
#endif  // FIELDS2COVER_UTILS_VISUALIZER_H_
