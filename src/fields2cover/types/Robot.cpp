//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include "fields2cover/utils/solver.h"
#include "fields2cover/types/Robot.h"

namespace f2c::types {

Robot::Robot(double width, double cov_width,
    double max_curv, double max_diff_curv) :
      width_(width), cov_width_(cov_width),
      max_icc_(max_curv), linear_curv_change_(max_diff_curv) {
  if (width <= 0.0 || cov_width < 0.0) {
    throw std::out_of_range("Robot widths have to be greater than 0.");
  }
  if (cov_width == 0.0) {
    this->cov_width_ = this->width_;
  }
}

Robot::Robot() = default;
Robot::~Robot() = default;
Robot::Robot(const Robot&) = default;
Robot::Robot(Robot &&) = default;
Robot& Robot::operator=(const Robot&) = default;
Robot& Robot::operator=(Robot&&) = default;

void Robot::setImpl(const Point& pos_impl, const GenericImplement& impl) {
  this->implement = std::make_pair(pos_impl, impl);
}

std::optional<std::pair<Point, GenericImplement>> Robot::getImpl() const {
  return this->implement;
}
std::optional<std::pair<Point, GenericImplement>>& Robot::getImpl() {
  return this->implement;
}


double Robot::getWidth() const {
  double rw = 0.5 * getRobotWidth();
  double iw = 0.5 * getImplWidth();
  double ci = getCenterImpl().getY();
  return std::max(rw, ci + iw) -
         std::min(-rw, ci - iw);
}

double Robot::getLength() const {
  return std::max(getFrontRobot().getX(), getFrontImpl().getX()) -
    std::min(getBackRobot().getX(), getBackImpl().getX());
}

std::string Robot::getName() const {
  return this->name_;
}

void Robot::setName(const std::string& str) {
  this->name_ = str;
}

void Robot::setWidth(double w) {
  this->width_ = w;
}

double Robot::getCovWidth() const {
  return this->cov_width_;
}

void Robot::setCovWidth(double w) {
  this->cov_width_ = w;
}

void Robot::setCruiseVel(double v) {
  this->cruise_speed_ = v;
}

void Robot::setTurnVel(double v) {
  this->turn_vel_ = v;
}

double Robot::getMinTurningRadius() const {
  return 1. / (this->max_icc_ + 1e-7);
}

void Robot::setMinTurningRadius(double rad) {
  this->max_icc_ = 1.0 / (fabs(rad) + 1e-7);
}

void Robot::setMaxCurv(double c) {
  this->max_icc_ = fabs(c);
}

void Robot::setMaxDiffCurv(double dc) {
  this->linear_curv_change_ = fabs(dc);
}

Robot& Robot::addCapacity(const RobotCapacity& capacity) {
  this->capacities_.emplace_back(capacity);
  return *this;
}

Robot& Robot::setCapacities(const std::vector<RobotCapacity>& c) {
  this->capacities_ = c;
  return *this;
}


void Robot::setReloadPoints(const Field& field) {
  auto field_reload_points = field.getReloadPoints();
  auto robot_capacities = this->getCapacities();
  for (auto&& c : robot_capacities) {
    c.reload_points.clear();
    for (auto&& fr : field_reload_points) {
      if (c.name == fr.first) {
        c.reload_points = fr.second;
      }
    }
  }
  this->setCapacities(robot_capacities);
}


std::vector<Point> Robot::getReloadPoints() const {
  std::vector<Point> vp;
  for (const auto& c : this->getCapacities()) {
    for (const auto& p : c.reload_points) {
      vp.emplace_back(p);
    }
  }
  return vp;
}

size_t Robot::getNumReloadPoints() const {
  return this->getReloadPoints().size();
}

std::vector<RobotCapacity> Robot::getCapacities() const {
  return this->capacities_;
}

size_t Robot::getNumCapacities() const {
  return this->capacities_.size();
}

bool Robot::isReloadPoint(const Point& p) const {
  for (auto&& c : this->capacities_) {
    for (auto&& reload_point : c.reload_points) {
      if (reload_point == p) {return true;}
    }
  }
  return false;
}

const RobotCapacity& Robot::getCapacity(size_t i) const {
  return this->capacities_[i];
}

RobotCapacity Robot::getCapacity(size_t i) {
  return this->capacities_[i];
}

double Robot::getRobotWidth() const {
  return width_;
}
void Robot::setRobotLength(double length) {
  this->length_ = length;
}
double Robot::getRobotLength() const {
  return length_;
}
double Robot::getImplWidth() const {
  return implement ? implement->second.width : 0.0;
}
double Robot::getImplLength() const {
  return implement ? implement->second.length : 0.0;
}
double Robot::getImplCovWidth() const {
  if (implement) {
    return implement->second.cov_width ?
      *implement->second.cov_width : implement->second.width;
  }
  return this->width_;
}
double Robot::getImplCovLength() const {
  if (implement) {
    return implement->second.cov_length ?
      *implement->second.cov_length : implement->second.length;
  }
  return 0.0;
}

Point Robot::getCenterImpl() const {
  return implement ?
    (implement->first + implement->second.c_impl) : Point(0.0, 0.0);
}

Point Robot::getCenterImplFromCRot() const {
  return getCenterImpl() + center_rot;
}

Point Robot::getCenterImplCov() const {
  return implement ?
    (implement->first + (implement->second.c_impl + implement->second.c_cov)) :
    Point(-length_ * .5, 0.0);
}

Point Robot::getCenterImplCovFromCRot() const {
  return getCenterImplCov() + center_rot;
}

Point Robot::getPointOnRobot(const Point& pos_robot, double ang_robot,
    const Point& point_on_robot) const {
  return pos_robot + Point(0, 0).rotateFromPoint(
      ang_robot, point_on_robot + center_rot);
}

std::vector<Point> Robot::getPointsOnRobot(const Point& pos_robot,
    double ang_robot, const std::vector<Point>& points_on_robot) const {
  std::vector<Point> ps;
  for (auto&& p : points_on_robot) {
    ps.emplace_back(getPointOnRobot(pos_robot, ang_robot, p));
  }
  return ps;
}

std::vector<Point> Robot::getPointOnRobot(const Path& path,
    const Point& point_on_robot) const {
  std::vector<Point> ps;
  for (auto&& s : path) {
    ps.emplace_back(getPointOnRobot(
        s.point, s.angle, point_on_robot));
  }
  ps.emplace_back(getPointOnRobot(
        path.atEnd(), path.back().angle, point_on_robot));
  return ps;
}

std::vector<std::vector<Point>> Robot::getPointsOnRobot(const Path& path,
    const std::vector<Point>& points_on_robot) const {
  std::vector<std::vector<Point>> ps(points_on_robot.size());
  for (size_t i = 0; i < points_on_robot.size(); ++i) {
    ps[i] = getPointOnRobot(path, points_on_robot[i]);
  }
  return ps;
}

Point Robot::getBackRobot() const {
  return {-0.5 * getRobotLength(), 0};
}

Point Robot::getFrontRobot() const {
  return {0.5 * getRobotLength(), 0};
}

Point Robot::getBackRobot(const Point& pos_robot, double ang_robot) const {
  return getPointOnRobot(pos_robot, ang_robot, getBackRobot());
}

Point Robot::getFrontRobot(const Point& pos_robot, double ang_robot) const {
  return getPointOnRobot(pos_robot, ang_robot, getFrontRobot());
}


Point Robot::getBackImpl() const {
  Point c_impl = getCenterImpl();
  return {c_impl.getX() - 0.5 * getImplLength(), c_impl.getY()};
}

Point Robot::getFrontImpl() const {
  Point c_impl = getCenterImpl();
  return {c_impl.getX() + 0.5 * getImplLength(), c_impl.getY()};
}

Point Robot::getBackImpl(const Point& pos_robot, double ang_robot) const {
  return getPointOnRobot(pos_robot, ang_robot, getBackImpl());
}

Point Robot::getFrontImpl(const Point& pos_robot, double ang_robot) const {
  return getPointOnRobot(pos_robot, ang_robot, getFrontImpl());
}

Point Robot::getCenterImplCov(const Point& pos_robot, double ang_robot) const {
  return getPointOnRobot(pos_robot, ang_robot, this->getCenterImplCov());
}

Point Robot::getBackImplCov() const {
  Point c_impl = getCenterImplCov();
  return {c_impl.getX() - 0.5 * getImplCovLength(), c_impl.getY()};
}

Point Robot::getFrontImplCov() const {
  Point c_impl = getCenterImplCov();
  return {c_impl.getX() + 0.5 * getImplCovLength(), c_impl.getY()};
}

Point Robot::getBackImplCov(const Point& pos_robot, double ang_robot) const {
  return getPointOnRobot(pos_robot, ang_robot, getBackImplCov());
}

Point Robot::getFrontImplCov(const Point& pos_robot, double ang_robot) const {
  return getPointOnRobot(pos_robot, ang_robot, getFrontImplCov());
}



double Robot::getCruiseVel(bool using_impl) const {
  if (implement) {
    if (using_impl && implement->second.vel_impl_on) {
      return *implement->second.vel_impl_on;
    } else if (!using_impl && implement->second.vel_impl_off) {
      return *implement->second.vel_impl_off;
    }
  }
  return cruise_speed_;
}

double Robot::getTurnVel(bool using_impl) const {
  if (implement) {
    if (using_impl && implement->second.vel_impl_on) {
      return *implement->second.vel_impl_on;
    } else if (!using_impl && implement->second.vel_impl_off) {
      return *implement->second.vel_impl_off;
    }
  }
  return turn_vel_ ? *turn_vel_ : cruise_speed_;
}

double Robot::getMaxCurv(bool using_impl) const {
  if (implement) {
    if (using_impl && implement->second.max_icc_impl_on) {
      return *implement->second.max_icc_impl_on;
    } else if (!using_impl && implement->second.max_icc_impl_off) {
      return *implement->second.max_icc_impl_off;
    }
  }
  return max_icc_;
}

double Robot::getMaxDiffCurv(bool using_impl) const {
  if (implement) {
    if (using_impl && implement->second.linear_curv_change_impl_on) {
      return *implement->second.linear_curv_change_impl_on;
    } else if (!using_impl && implement->second.linear_curv_change_impl_off) {
      return *implement->second.linear_curv_change_impl_off;
    }
  }
  return linear_curv_change_;
}

bool Robot::isRobotSymmetric() const {
  return (getCenterImpl().getY() == 0.0);
}

double Robot::getMinY(const PathSimp& path) const {
  return getMinY(path, getCornersRIC());
}
double Robot::getMinYWithRobotBackwards(const PathSimp& path) const {
  std::vector<Point> cr = getCornersRIC();
  for (int i = 0; i < cr.size(); ++i) {
    cr[i].setX(-cr[i].getX());
  }
  return getMinY(path, cr);
}

double Robot::getMinY(const PathSimp& path, const std::vector<Point>& corners_robot) const {
  double min_y = 1e7;
  for (auto&& r : corners_robot) {
    auto f = [this, &r, &path] (double t) {
      auto s_t = path.at(t);
      auto p = getPointOnRobot(s_t.p, s_t.theta, r);
      return p.getY();
    };
    double t = f2c::Solver::findMinGoldenSection(f, 0, path.length(), 1e-5);
    double dist = f(t);
    if (min_y > dist) {
      min_y = dist;
    }
  }
  return min_y;
}

double Robot::getMinSignedDistToSegment(
    const PathSimp& path, const Point& seg_s, const Point& seg_e) const {
  auto points_robot = getPointsOnRobot(path.toPath(), getCornersRIC());
  auto min_y = 1e7;
  for (auto&& v_points : points_robot) {
    for (auto&& point : v_points) {
      double dist = point.signedDistance2Segment(seg_s, seg_e);
      if (min_y > dist) {
        min_y = dist;
      }
    }
  }
  return min_y;
}


std::vector<Point> Robot::getCornersRIC() const {
  std::vector<Point> ps;
  ps.emplace_back(0.5  * this->getRobotLength(),  0.5 * this->getRobotWidth());
  ps.emplace_back(-0.5 * this->getRobotLength(),  0.5 * this->getRobotWidth());
  ps.emplace_back(0.5  * this->getRobotLength(), -0.5 * this->getRobotWidth());
  ps.emplace_back(-0.5 * this->getRobotLength(), -0.5 * this->getRobotWidth());
  ps.emplace_back(this->getCenterImpl() +                                 \
      Point(0.5  * this->getImplLength(),  0.5 * this->getImplWidth()));
  ps.emplace_back(this->getCenterImpl() +                                 \
      Point(-0.5 * this->getImplLength(),  0.5 * this->getImplWidth()));
  ps.emplace_back(this->getCenterImpl() +                                 \
      Point(0.5  * this->getImplLength(), -0.5 * this->getImplWidth()));
  ps.emplace_back(this->getCenterImpl() +                                 \
      Point(-0.5 * this->getImplLength(), -0.5 * this->getImplWidth()));
  return ps;
}


Cells Robot::computeAreaCovered(const Path& path) const {
  if (path.size() == 0) {
    return {};
  }
  Point p_bi = getBackImplCov();
  Point p_fi = getFrontImplCov();
  Point p_ubi = p_bi + Point(0, 0.5 * getImplCovWidth());
  Point p_dbi = p_bi + Point(0, -0.5 * getImplCovWidth());
  Point p_ufi = p_fi + Point(0, 0.5 * getImplCovWidth());
  Point p_dfi = p_fi + Point(0, -0.5 * getImplCovWidth());

  auto create_cell_step = [] (const std::vector<std::vector<Point>>& ps,
      size_t i1, size_t i2, size_t i3, size_t i4, size_t j) {
    return Cell::buffer(static_cast<Cell>(LinearRing(
          {ps[i1][j], ps[i2][j], ps[i4][j+1], ps[i3][j+1], ps[i1][j]})), 0);
  };

  auto cov_paths = path.getCovPaths();
  Cells area_bi;
  Cells area_fi;
  for (auto&& cov_p : cov_paths) {
    auto ps = getPointsOnRobot(cov_p, {p_ubi, p_dbi, p_ufi, p_dfi});
    for (size_t i = 0; i < ps[0].size()-1; ++i) {
      area_bi.addGeometry(create_cell_step(ps, 0, 1, 0, 1, i));
      area_fi.addGeometry(create_cell_step(ps, 2, 3, 2, 3, i));
    }
  }
  return area_bi.unionCascaded().intersection(area_fi.unionCascaded());
}


Cells Robot::computeAreaTravelled(const Path& path) const {
  if (path.size() == 0) {
    return {};
  }
  auto p_rob = getCornersRIC();

  auto v_path = path.splitPathByEqualTypeStates();

  auto create_cell_step = [] (const std::vector<std::vector<Point>>& ps,
      size_t i1, size_t i2, size_t i3, size_t i4, size_t j) {
    LinearRing ring {ps[i1][j], ps[i2][j], ps[i4][j+1], ps[i3][j+1], ps[i1][j]};
    return Cell::buffer(static_cast<Cell>(ring), 1e-5);
  };

  Cells area_t;
  for (auto&& path_i : v_path) {
    auto ps = getPointsOnRobot(path_i, p_rob);
    for (size_t i = 0; i < ps[0].size()-1; ++i) {
      area_t.addGeometry(create_cell_step(ps, 0, 2, 1, 3, i));  // Robot
      area_t.addGeometry(create_cell_step(ps, 4, 6, 5, 7, i));  // Impl
      area_t.addGeometry(create_cell_step(ps, 0, 2, 0, 2, i));  // Front Robot
      area_t.addGeometry(create_cell_step(ps, 1, 3, 1, 3, i));  // Back Robot
      area_t.addGeometry(create_cell_step(ps, 4, 6, 4, 6, i));  // Front Impl
      area_t.addGeometry(create_cell_step(ps, 5, 7, 5, 7, i));  // Back Impl
    }
  }
  return area_t.unionCascaded();
}

size_t Robot::getNumWheels() const {
  return this->getNumRobotWheels() + this->getNumImplWheels();
}

size_t Robot::getNumRobotWheels() const {
  return this->wheels.size();
}

size_t Robot::getNumImplWheels() const {
  return (this->implement ? this->implement->second.wheels.size() : 0);
}

const RobotWheel& Robot::getWheel(size_t i) const {
  size_t n_robot_wheels = this->getNumRobotWheels();
  if (i < n_robot_wheels) {
    return getRobotWheel(i);
  }
  return getImplWheel(i - n_robot_wheels);
}

const RobotWheel& Robot::getRobotWheel(size_t i) const {
  if (i >= this->getNumRobotWheels()) {
    throw std::out_of_range(std::string("On getRobotWheel(") + \
        std::to_string(i) + "), " + \
        "i has to be smaller than the number of wheels in the robot (" + \
        std::to_string(this->getNumRobotWheels()) + ")");
  }
  return wheels[i].second;
}

const RobotWheel& Robot::getImplWheel(size_t i) const {
  if (!this->implement) {
    throw std::out_of_range("No implement attached");
  }
  if (i >= this->getNumImplWheels()) {
    throw std::out_of_range(std::string("On getImplWheel(") + \
        std::to_string(i) + "), " + \
        "i has to be smaller than the number of wheels in the implement (" +
        std::to_string(this->getNumImplWheels()) + ")");
  }
  return this->implement->second.wheels[i].second;
}

Point Robot::getWheelPos(size_t i) const {
  size_t n_robot_wheels = this->getNumRobotWheels();
  if (i < n_robot_wheels) {
    return getRobotWheelPos(i);
  }
  return getImplWheelPos(i - n_robot_wheels);
}

Point Robot::getRobotWheelPos(size_t i) const {
  if (i >= this->getNumRobotWheels()) {
    throw std::out_of_range(std::string("On getRobotWheelPos(") + \
        std::to_string(i) + "), " + \
        "i has to be smaller than the number of wheels in the robot (" + \
        std::to_string(this->getNumRobotWheels()) + ")");
  }
  return wheels[i].first;
}

Point Robot::getImplWheelPos(size_t i) const {
  if (!this->implement) {
    throw std::out_of_range("No implement attached");
  }
  if (i >= this->getNumImplWheels()) {
    throw std::out_of_range(std::string("On getImplWheelPos(") + \
        std::to_string(i) + "), " + \
        "i has to be smaller than the number of wheels in the implement (" +
        std::to_string(this->getNumImplWheels()) + ")");
  }
  return  this->implement->first + this->implement->second.wheels[i].first;
}

Point Robot::getWheelPos(size_t i,
    const Point& robot_pos, double robot_angle) const {
  return robot_pos + Point(0, 0).rotateFromPoint(robot_angle, getWheelPos(i));
}

Cell Robot::getWheelShape(size_t i, double robot_angle,
    const Point& robot_pos) const {
  return Cell::buffer(
      Point(0, 0).rotateFromPoint(
        robot_angle,
        getWheel(i).getPressurePoints() + getWheelPos(i)),
      getWheel(i).getWidth());
}

LineString Robot::getCenterTrackWheelOnPressurePoint(
    size_t i_wheel, size_t i_press_p, const Path& path) const {
  if (i_wheel > getNumWheels() ||
      i_press_p > getWheel(i_wheel).size() ||
      path.size() ==0 ) {
    return LineString();
  }
  LineString track;
  Point press_point {getWheelPos(i_wheel) +
    getWheel(i_wheel).getPressurePoint(i_press_p)};
  for (size_t n = 0; n < path.size(); ++n) {
    track.addPoint(
      Point(0, 0).rotateFromPoint(
        path[n].angle,
        getWheelPos(i_wheel) + getWheel(i_wheel).getPressurePoint(i_press_p))
        + path[n].point);
  }
  return track;
}

Cell Robot::getTrackWheelOnPressurePoint(
    size_t i_wheel, size_t i_press_p, const Path& path) const {
  return Cell::buffer(
      getCenterTrackWheelOnPressurePoint(i_wheel, i_press_p, path),
      getWheel(i_wheel).getWidth());
}

Cells Robot::getTrackWheelOnPressurePoints(
    size_t i_wheel, const Path& path) const {
  Cells tracks;
  for (size_t i = 0; i < getWheel(i_wheel).size(); ++i) {
    tracks.addGeometry(getTrackWheelOnPressurePoint(i_wheel, i, path));
  }
  return tracks;
}

Cells Robot::getTrackWheel(size_t i_wheel, const Path& path) const {
  if (getWheel(i_wheel).size() < 2) {
    return getTrackWheelOnPressurePoints(i_wheel, path);
  }
  std::vector<LineString> track_lines;
  for (size_t i = 0; i < getWheel(i_wheel).size(); ++i) {
    LineString track {getCenterTrackWheelOnPressurePoint(i_wheel, i, path)};
    if (!track.isEmpty()) {
      track_lines.emplace_back(track);
    }
  }
  if (track_lines.size() == 0 || track_lines[0].size() == 0) {
    return Cells();
  } else if (track_lines[0].size() <= 1) {
    return Cells(Cell::buffer(track_lines[0], getWheel(i_wheel).getWidth()));
  }
  Cells track_area;
  for (size_t i = 1; i < track_lines[0].size(); ++i) {
    LineString wheel_ring;
    for (size_t j = 0; j < track_lines.size(); ++j) {
      wheel_ring.addPoint(track_lines[j].getGeometry(i));
    }
    for (int j = track_lines.size() - 1; j >= 0; --j) {
      wheel_ring.addPoint(track_lines[j].getGeometry(i - 1));
    }
    wheel_ring.addPoint(track_lines[0].getGeometry(i));
    track_area.addGeometry(Cell(Cell::buffer(
        wheel_ring, getWheel(i_wheel).getWidth())));
  }
  return track_area.unionCascaded();
}

Cells Robot::getTracks(const Path& path) const {
  Cells tracks;
  for (size_t i = 0; i < getNumWheels(); ++i) {
    Cells track_wheel = getTrackWheel(i, path);
    for (size_t j = 0; j < track_wheel.size(); ++j) {
      tracks.addGeometry(track_wheel.getGeometry(j));
    }
  }
  return tracks.unionCascaded();
}

Cells Robot::getDetailedTrackWheelOnPressurePoint(
    size_t i_wheel, const Path& path) const {
  const size_t N = wheels[i_wheel].second.size();
  if (N < 1) {
    return Cells();
  } else if (N == 1) {
    return getDetailedTrackWheelOnPressurePoint(i_wheel, 0, path);
  }

  std::vector<LineString> guide_lines;
  for (int i = 0; i < N; ++i) {
    guide_lines.emplace_back(
        getCenterTrackWheelOnPressurePoint(i_wheel, i, path));
  }
  const double r = getWheel(i_wheel).getWidth();
  constexpr double pi_2 = boost::math::constants::half_pi<double>();
  Cells tracks;
  std::vector<LineString> prev_line;
  for (int j = 0; j < N; ++j) {
    prev_line.emplace_back(LineString({
      guide_lines[j].getGeometry(0).getPointFromAngle(
          path[0].angle + pi_2, r),
      guide_lines[j].getGeometry(0).getPointFromAngle(
          path[0].angle - pi_2, r)}));
  }
  for (size_t i = 1; i < guide_lines[0].size(); ++i) {
    std::vector<LineString> act_line;
    for (int j = 0; j < guide_lines.size(); ++j) {
      act_line.emplace_back(LineString({
        guide_lines[j].getGeometry(i).getPointFromAngle(
            path[i].angle + pi_2, r),
        guide_lines[j].getGeometry(i).getPointFromAngle(
            path[i].angle - pi_2, r)}));
    }
    Cells step_tracks;
    step_tracks.addGeometry(Cell(LinearRing({
        prev_line[0].getGeometry(0), prev_line.back().getGeometry(0),
        prev_line.back().getGeometry(1), prev_line[0].getGeometry(1),
        prev_line[0].getGeometry(0)})));
    step_tracks.addGeometry(Cell(LinearRing({
        act_line[0].getGeometry(0), act_line.back().getGeometry(0),
        act_line.back().getGeometry(1), act_line[0].getGeometry(1),
        act_line[0].getGeometry(0)})));
    step_tracks.addGeometry(Cell(LinearRing({
        prev_line[0].getGeometry(0), prev_line[0].getGeometry(1),
        act_line[0].getGeometry(1), prev_line[0].getGeometry(0)})));
    step_tracks.addGeometry(Cell(LinearRing({
        prev_line[0].getGeometry(0), prev_line[0].getGeometry(1),
        act_line[0].getGeometry(0), prev_line[0].getGeometry(0)})));
    step_tracks.addGeometry(Cell(LinearRing({
        prev_line.back().getGeometry(0), prev_line.back().getGeometry(1),
        act_line.back().getGeometry(1), prev_line.back().getGeometry(0)})));
    step_tracks.addGeometry(Cell(LinearRing({
        prev_line.back().getGeometry(0), prev_line.back().getGeometry(1),
        act_line.back().getGeometry(0), prev_line.back().getGeometry(0)})));

    auto union_tracks = step_tracks.unionCascaded();
    if (union_tracks.size() > 0 && union_tracks.getGeometry(0).size() > 0) {
      tracks.addGeometry(Cell(union_tracks.getGeometry(0).getGeometry(0)));
    }

    prev_line = act_line;
  }
  return tracks.unionCascaded();
}

Cells Robot::getDetailedTrackWheelOnPressurePoint(
    size_t i_wheel, size_t i_press_p, const Path& path) const {
  LineString guide_line =
    getCenterTrackWheelOnPressurePoint(i_wheel, i_press_p, path);
  if (guide_line.size() <= 1) {
    return Cells();
  }
  const double r = getWheel(i_wheel).getWidth();
  constexpr double pi_2 = boost::math::constants::half_pi<double>();
  Cells tracks;
  LineString prev_line({
      guide_line.getGeometry(0).getPointFromAngle(
          path[0].angle + pi_2, r),
      guide_line.getGeometry(0).getPointFromAngle(
          path[0].angle - pi_2, r)});
  for (size_t i = 1; i < guide_line.size(); ++i) {
    LineString act_line({
        guide_line.getGeometry(i).getPointFromAngle(
            path[i].angle + pi_2, r),
        guide_line.getGeometry(i).getPointFromAngle(
            path[i].angle - pi_2, r)});
    tracks.addGeometry(Cell(LinearRing({
        prev_line.getGeometry(0), prev_line.getGeometry(1),
        act_line.getGeometry(1), act_line.getGeometry(0),
        prev_line.getGeometry(0)})));
    prev_line = act_line;
  }
  return tracks;
}




}  // namespace f2c::types

