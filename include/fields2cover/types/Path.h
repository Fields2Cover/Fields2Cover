//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_PATH_H_
#define FIELDS2COVER_TYPES_PATH_H_

#include <gdal/ogr_geometry.h>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include "fields2cover/types/Swath.h"
#include "fields2cover/types/Point.h"

namespace f2c::types {

enum class PathSectionType {
  SWATH = 1,
  TURN = 2,
};

enum class PathDirection {
  FORWARD = 1,
  BACKWARD = -1,
  // STOP = 0,
};

struct PathState {
  Point point;
  double angle;
  double velocity;
  double duration;
  PathDirection dir;
  PathSectionType type;

 public:
  PathState clone() const;
};

struct Path {
 public:
  std::vector<PathState> states;
  double task_time {0.0};
  double measure_error {0.1};

 public:
  Path& operator+=(const Path& path);
  Path clone() const;
  size_t size() const;

  void moveTo(const Point&);

  double length(void) const;

  void appendSwath(const Swath& swath, double cruise_speed);

  std::string serializePath(size_t digit_precision = 6) const;
  Path discretize_swath(double step_size) const;
  void saveToFile(const std::string& file, size_t precision = 6) const;
  void loadFile(const std::string& file);

  Path& populate(int number_points = 100);
  Path& reduce(double min_dist_equal = 0.1);
};

}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_PATH_H_
