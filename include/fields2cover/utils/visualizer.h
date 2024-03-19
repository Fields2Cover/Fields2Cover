//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
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
  static double getLineWidth() {return 1.0;}

  static void plot(double x, double y, const std::vector<double>& color = {});
  static void plot(const F2CPoint& p, const std::vector<double>& color = {});
  static void plot(
      const F2CSwath& s, const std::vector<double>& color = {});
  static void plot(const F2CSwaths& s,
      const std::vector<std::vector<double>>& color = {});
  static void plot(const F2CSwathsByCells& s,
      const std::vector<std::vector<double>>& color = {});
  static void plot(const std::vector<F2CSwathsByCells>& vs,
      const std::vector<std::vector<double>>& color = {});
  static void plot(const F2CLineString& line,
      const std::vector<double>& color = {});
  static void plot(const F2CLinearRing& ring,
      const std::vector<double>& color = {});
  static void plot(const F2CMultiPoint& multipoint,
      const std::vector<double>& color = {});
  static void plot(const F2CMultiLineString& multiline,
      const std::vector<double>& color = {});
  static void plot(const F2CCell& cell,
      const std::vector<double>& color = {});
  static void plot(const F2CCells& cells,
      const std::vector<double>& color = {});
  static void plot(const F2CRoute& route,
      const std::vector<std::vector<double>>& color = {});
  static void plot(const std::vector<F2CRoute>& route,
      const std::vector<std::vector<double>>& color = {});
  static void plot(const F2CPath& path,
      const std::vector<std::vector<double>>& color = {});
  static void plot(const F2CField& field,
      const std::vector<double>& color = {});

  static void plotFilled(const F2CField& field,
      const std::vector<double>& color = {});
  static void plotFilled(
      const F2CLinearRing& ring, const std::vector<double>& color);
  static void plotFilled(const F2CCell& cell,
      const std::vector<double>& poly_color,
      const std::vector<double>& holes_color);
  static void plotFilled(const F2CCells& cells,
      const std::vector<double>& poly_color,
      const std::vector<double>& holes_color);
  static void plotFilled(const F2CCell& cell,
      const std::vector<double>& color);
  static void plotFilled(const F2CCells& cells,
      const std::vector<double>& poly_color);

  static void plot(const std::vector<double>& t,
      const std::vector<double>& d,  const std::vector<double>& color);

  /// Plot vector of type T
  template<class T>
  static void plot(const std::vector<T>& v_t, const std::vector<double>& color);
  template<class T>
  static void plot(
      const std::vector<T>& v_t,
      const std::vector<std::vector<double>>& color = {});

  /// Create figure to plot on
  static void figure();

  static void axis_equal();

  /// Show the figure on screen
  /// @param block if true the program will wait until the screen is closed
  /// to continue
  static void show();

  /// Save figure to a file
  /// @param file Name of the file
  static void save(const std::string& file);

  /// Add title to the figure
  /// @param text Title text
  static void title(const std::string& text);

  static void xlim(double min, double max);
  static void ylim(double min, double max);

 private:
  template<class T>
  static std::vector<F2CPoint> data2vector(const T& t);

  static std::vector<std::vector<double>> getComponents(
      const std::vector<F2CPoint>& points);

  static std::vector<double> linspace(double min, double max, size_t N);
  static std::vector<std::vector<double>> color_linspace(
      const std::vector<int>& min,
      const std::vector<int>& max,
      size_t N);
};

template<class T>
std::vector<F2CPoint> Visualizer::data2vector(const T& t) {
  if constexpr (std::is_same<T, F2CPoint>::value) {
    return std::vector<F2CPoint>{t};
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
void Visualizer::plot(
    const std::vector<T>& v_t,
    const std::vector<double>& color) {
  if constexpr (std::is_same<T, F2CPoint>::value) {
    auto comp = getComponents(v_t);
    plot(comp[0], comp[1], color);
  } else if constexpr (std::is_same<T, double>::value) {
    std::vector<double> t(v_t.size());
    std::iota(std::begin(t), std::end(t), 0.0);
    plot(t, v_t, color);
  } else {
    for (auto&& t : v_t) {
      plot(t, color);
    }
  }
}

}  // namespace f2c


#endif  // FIELDS2COVER_UTILS_VISUALIZER_H_
