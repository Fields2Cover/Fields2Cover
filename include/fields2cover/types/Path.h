//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_PATH_H_
#define FIELDS2COVER_TYPES_PATH_H_

#include <ogr_geometry.h>
#include <fstream>
#include <vector>
#include <string>
#include "fields2cover/types/Swath.h"
#include "fields2cover/types/Point.h"
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/MultiLineString.h"
#include "fields2cover/types/PathState.h"

namespace f2c::types {

struct Path {
 public:
  PathState& getState(size_t i);
  const PathState& getState(size_t i) const;
  void setState(size_t i, const PathState& ps);

  void addState(const PathState& ps);
  void addState(const Point& p, double ang, double len,
      PathDirection dir = PathDirection::FORWARD,
      PathSectionType type = PathSectionType::SWATH, double vel = 1.0);

  std::vector<PathState>& getStates();
  const std::vector<PathState>& getStates() const;
  void setStates(const std::vector<PathState>& v_ps);

  double getTaskTime() const;

  std::vector<PathState>::const_iterator cbegin() const;
  std::vector<PathState>::const_iterator cend() const;
  std::vector<PathState>::const_iterator begin() const;
  std::vector<PathState>::const_iterator end() const;
  std::vector<PathState>::iterator begin();
  std::vector<PathState>::iterator end();

  const PathState& operator[](size_t idx) const;
  PathState& operator[](size_t idx);
  const PathState& back() const;
  PathState& back();


  Path& operator+=(const Path& path);
  size_t size() const;

  double getDimMinX() const;
  double getDimMinY() const;
  double getDimMaxX() const;
  double getDimMaxY() const;

  void moveTo(const Point&);
  void rotateFromPoint(const Point&, double ang);

  void mirrorX();
  void mirrorY();
  void setBackwardDir();
  void setBackwardDir(int i);
  void setForwardDir();
  void setForwardDir(int i);
  void setSwathType();
  void setSwathType(int i);
  void setTurnType();
  void setTurnType(int i);

  double length(void) const;

  /// Corners the path turns through faster than the vehicle can steer.
  ///
  /// Each state carries the heading it is driven at, so a jump in heading
  /// between one state and the next is a corner turned on the spot. A vehicle
  /// with a turning radius cannot drive one. PathPlanning leaves a corner
  /// square when no turn fits the room it has, and nothing reports it: the
  /// path comes back looking complete and is not drivable.
  /// @param max_jump Heading change, in radians, still counted as steering.
  /// @return Number of corners that turn faster than \a max_jump
  size_t countSharpTurns(double max_jump = 0.35) const;

  /// Widest heading jump between two consecutive states, in radians.
  /// Zero on a path of fewer than two states.
  double maxHeadingJump() const;

  void appendSwath(const Swath& swath, double cruise_speed);
  void appendSwath(const Swath& swath, double cruise_speed, PathSectionType type);

  /// Append a straight leg from one point to another as a single state.
  /// Legs shorter than 1e-6 are skipped, as they carry no direction.
  void appendStraight(const Point& start, const Point& end,
      double cruise_speed, PathSectionType type);

  /// Furthest any state of this path lies from the given line.
  double maxDistanceTo(const LineString& line) const;

  PathState at(double t) const;
  Point atStart() const;
  Point atEnd() const;

  LineString toLineString() const;
  std::string serializePath(size_t digit_precision = 6) const;
  void saveToFile(const std::string& file, size_t precision = 6) const;
  void loadFile(const std::string& file);

  Path discretizeSwath(double step_size) const;
  Path& discretize(double step_size);
  Path& populate(int number_points = 100);
  Path& reduce(double min_dist_equal = 0.1);

 private:
  std::vector<PathState> states_;
};

}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_PATH_H_
