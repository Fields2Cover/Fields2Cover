//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_ROBOT_H_
#define FIELDS2COVER_TYPES_ROBOT_H_

#include <gdal/ogr_geometry.h>
#include <utility>
#include <string>
#include <vector>
#include <optional>
#include "fields2cover/types/Point.h"
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/MultiLineString.h"
#include "fields2cover/types/Cell.h"
#include "fields2cover/types/Cells.h"
#include "fields2cover/types/Field.h"
#include "fields2cover/types/Swath.h"
#include "fields2cover/types/Path.h"
#include "fields2cover/types/PathSimp.h"
#include "fields2cover/types/RobotWheel.h"
#include "fields2cover/types/GenericImplement.h"

namespace f2c::types {

enum class RobotCapacityType {
  BY_LENGTH = 0,
  BY_SWATH_LENGTH = 1,
  // CUSTOM = 2,
};

struct RobotCapacity {
  std::string name;
  int64_t max_capacity {static_cast<int64_t>(1e6)};
  int64_t rate {1};
  int64_t reload_time {0};
  RobotCapacityType type {RobotCapacityType::BY_LENGTH};

  std::vector<Point> reload_points;
};

struct Robot {
 public:
  explicit Robot(double width, double cov_width = 0.0,
      double max_curv = 1.0, double max_diff_curv = 0.3);
  Robot();
  ~Robot();
  Robot(const Robot&);
  Robot(Robot &&);
  Robot &operator=(const Robot&);
  Robot &operator=(Robot&&);

  std::string getName() const;
  void setName(const std::string& str);

  double getWidth() const;
  void setWidth(double);

  double getCovWidth() const;
  void setCovWidth(double);

  void setImpl(const Point& pos_impl, const GenericImplement& impl);
  std::optional<std::pair<Point, GenericImplement>> getImpl() const;
  std::optional<std::pair<Point, GenericImplement>>& getImpl();

  double getLength() const;
  double getRobotWidth() const;
  double getRobotLength() const;
  double getImplWidth() const;
  double getImplLength() const;
  double getImplCovWidth() const;
  double getImplCovLength() const;
  Point getCenterImpl() const;
  Point getCenterImplFromCRot() const;
  Point getCenterImplCov() const;
  Point getCenterImplCovFromCRot() const;
  double getCruiseVel(bool using_impl = true) const;
  double getTurnVel(bool using_impl = false) const;
  double getMaxCurv(bool using_impl = false) const;
  double getMaxDiffCurv(bool using_impl = false) const;

  bool isRobotSymmetric() const;

  void setCruiseVel(double);

  void setTurnVel(double);

  double getMinTurningRadius() const;
  void setMinTurningRadius(double rad);

  void setMaxCurv(double);

  void setMaxDiffCurv(double);

  Robot& addCapacity(const RobotCapacity&);
  Robot& setCapacities(const std::vector<RobotCapacity>& c);
  RobotCapacity getCapacity(size_t i);
  const RobotCapacity& getCapacity(size_t i) const;
  std::vector<RobotCapacity> getCapacities() const;
  void setReloadPoints(const Field& field);
  std::vector<Point> getReloadPoints() const;
  size_t getNumReloadPoints() const;
  size_t getNumCapacities() const;
  bool isReloadPoint(const Point& p) const;

Point getPointOnRobot(const Point& pos_robot, double ang_robot,
      const Point& point_on_robot) const;
  std::vector<Point> getPointsOnRobot(const Point& pos_robot, double ang_robot,
      const std::vector<Point>& point_on_robot) const;

  std::vector<Point> getPointOnRobot(const Path& path,
      const Point& point_on_robot) const;
  std::vector<std::vector<Point>> getPointsOnRobot(const Path& path,
      const std::vector<Point>& points_on_robot) const;

  Point getBackRobot() const;
  Point getFrontRobot() const;
  Point getBackRobot(const Point& pos_robot, double ang_robot) const;
  Point getFrontRobot(const Point& pos_robot, double ang_robot) const;

  Point getBackImpl() const;
  Point getFrontImpl() const;
  Point getBackImpl(const Point& pos_robot, double ang_robot) const;
  Point getFrontImpl(const Point& pos_robot, double ang_robot) const;

  Point getBackImplCov() const;
  Point getFrontImplCov() const;
  Point getCenterImplCov(const Point& pos_robot, double ang_robot) const;
  Point getBackImplCov(const Point& pos_robot, double ang_robot) const;
  Point getFrontImplCov(const Point& pos_robot, double ang_robot) const;

  Cells computeAreaCovered(const Path& path) const;
  Cells computeAreaTravelled(const Path& path) const;


  /////////////////////////////
  double getMinY(const PathSimp& path) const;
  double getMinYWithRobotBackwards(const PathSimp& path) const;
  double getMinY(const PathSimp& path, const std::vector<Point>& corner_robot) const;
  double getMinSignedDistToSegment(
      const PathSimp& path, const Point& seg_s, const Point& seg_e) const;
  std::vector<Point> getCornersRIC() const;
  ////////////////////////////


  size_t getNumWheels() const;
  size_t getNumRobotWheels() const;
  size_t getNumImplWheels() const;

  const RobotWheel& getWheel(size_t i) const;
  const RobotWheel& getRobotWheel(size_t i) const;
  const RobotWheel& getImplWheel(size_t i) const;
  Point getWheelPos(size_t i) const;
  Point getRobotWheelPos(size_t i) const;
  Point getImplWheelPos(size_t i) const;

  Point getWheelPos(size_t i,
      const Point& robot_pos, double robot_angle = 0) const;

  Cell getWheelShape(size_t i,
      double robot_angle = 0.0,
      const Point& robot_pos = Point(0, 0)) const;

  LineString getCenterTrackWheelOnPressurePoint(
      size_t i_wheel, size_t i_press_p, const Path& path) const;
  Cell getTrackWheelOnPressurePoint(
      size_t i_wheel, size_t i_press_p, const Path& path) const;
  Cells getTrackWheelOnPressurePoints(size_t i_wheel, const Path& path) const;

  Cells getTrackWheel(size_t i_wheel, const Path& path) const;

  Cells getTracks(const Path& path) const;
  Cells getDetailedTrackWheelOnPressurePoint(
      size_t i_wheel, const Path& path) const;
  Cells getDetailedTrackWheelOnPressurePoint(
      size_t i_wheel, size_t i_press_p, const Path& path) const;



 private:
  std::string name_;

  /// Width of the robot
  double width_ {0.0};
  double length_ {0.0};
  /// Width of the coverage area of the robot
  double cov_width_ {0.0};

  /// Velocity of the robot when not doing turns.
  double cruise_speed_ {1.0};

  /// Maximum instantaneous curvature change
  double max_icc_ {1.0};  // [1/m]

  /// Maximum linear curvature change
  double linear_curv_change_ {0.3};  // [1/m^2]

  /// Velocity of the robot when doing turns. If not set, cruise_speed_ is used
  std::optional<double> turn_vel_;

  /// Center of rotation from geometric center of robot
  Point center_rot;

  /// Vector of the position of the center of the wheels, and the wheel data.
  std::vector<std::pair<Point, RobotWheel>> wheels;
  /// The position of the attachment of the implement, and the implement data.
  std::optional<std::pair<Point, GenericImplement>> implement;

  std::vector<RobotCapacity> capacities_;
};

}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_ROBOT_H_
