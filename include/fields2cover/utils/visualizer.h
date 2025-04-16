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
#include <utility>
#include <type_traits>
#include "fields2cover/types.h"

namespace f2c {

/// Class to plot Fields2Cover data structures
class Visualizer {
 public:
  static double getLineWidth() {return 1.0;};

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
  static void plot(const std::vector<F2CMultiLineString>& multilines,
      const std::vector<double>& color = {});
  static void plot(const F2CCell& cell,
      const std::vector<double>& color = {});
  static void plot(const F2CCells& cells,
      const std::vector<double>& color = {});
  static void plot(const F2CRoute& route,
      const std::vector<std::vector<double>>& color = {});
  static void plot(const F2CRoute& route, const F2CRobot& robot,
      const std::vector<std::vector<double>>& color = {});
  static void plot(const std::vector<F2CRoute>& route,
      const std::vector<std::vector<double>>& color = {});
  static void plot(const F2CPathStateSimp& path_state,
      const std::vector<std::vector<double>>& color = {});
  static void plot(const F2CPathSimp& path,
      const std::vector<std::vector<double>>& color = {});
  static void plot(const F2CPath& path,
      const std::vector<std::vector<double>>& color = {});

  static void plot(const F2CPath& path, const F2CRoute& route, const F2CRobot& robot);

  static void plot(const F2CField& field,
      const std::vector<double>& color = {});


  static void plot(const F2CRobot& robot,
      const std::vector<double>& color_robot = {0.85, 0.39, 0.35},
      const std::vector<double>& color_impl = {0.4, 0.4, 0.8},
      const std::vector<double>& color_cov = {0.95, 0.68, 0.45},
      const std::vector<double>& color_wheel = {0.95, 0.89, 0.58});

  static void plot(const F2CRobot& robot, const F2CPathState& state,
      const std::vector<double>& color_robot = {0.85, 0.39, 0.35},
      const std::vector<double>& color_impl_on = {0.4, 0.4, 0.8},
      const std::vector<double>& color_impl_off = {0.3, 0.3, 0.6},
      const std::vector<double>& color_cov = {0.95, 0.68, 0.45},
      const std::vector<double>& color_wheel = {0.95, 0.89, 0.58});



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
  //template<class T>
  //static void plot(
  //    const std::vector<T>& v_t,
  //    const std::vector<std::vector<double>>& color = {});


  static void plotReloadPoints(
      const F2CRoute& route, const F2CRobot& robot);

  /// Create figure to plot on
  static void figure();

  /// Change size of current figure
  static void figure_size(unsigned int width, unsigned int height);

  static void axis_equal();

  /// Show the figure on screen
  /// @param block if true the program will wait until the screen is closed
  /// to continue
  static void show();

  static void clear();

  /// Save figure to a file
  /// @param file Name of the file
  static void save(const std::string& file);

  static void animateRobotPath(const std::string& file_name,
      const F2CRobot& robot, const F2CPath& path,
      std::function<void(const F2CRobot&, const F2CPath&, double)> pre_plot,
      std::function<void(const F2CRobot&, const F2CPath&, double)> post_plot,
      double vel_animation = 1);


  template<typename... Args>
  static void show_img(Args&&... args) {
    (plot(std::forward<Args>(args)), ...);
    axis_equal();
    show();
    clear();
  }

  template<typename... Args>
  static void save_img(std::string name, Args&&... args) {
    (plot(std::forward<Args>(args)), ...);
    axis_equal();
    save(name);
    clear();
  }

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
  static std::string color2hex(uint32_t r, uint32_t g, uint32_t b);
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
