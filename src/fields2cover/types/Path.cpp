//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <numeric>
#include <steering_functions/utilities/utilities.hpp>
#include "fields2cover/utils/spline.h"
#include "fields2cover/types/Path.h"

namespace f2c::types {

PathState& Path::getState(size_t i) {
  return this->states_[i];
}

const PathState& Path::getState(size_t i) const {
  return this->states_[i];
}

void Path::setState(size_t i, const PathState& ps) {
  this->states_[i] = ps;
}

void Path::addState(const PathState& ps) {
  this->states_.emplace_back(ps);
}

void Path::addState(const Point& p, double ang, double len,
    PathDirection dir, PathSectionType type, double vel) {
  PathState ps;
  ps.point = p;
  ps.angle = ang;
  ps.len = len;
  ps.dir = dir;
  ps.type = type;
  ps.velocity = vel;
  this->states_.emplace_back(ps);
}

std::vector<PathState>& Path::getStates() {
  return this->states_;
}

const std::vector<PathState>& Path::getStates() const {
  return this->states_;
}

void Path::setStates(const std::vector<PathState>& v_ps) {
  this->states_ = v_ps;
}

double Path::getTaskTime() const {
  return std::accumulate(this->begin(), this->end(), 0.0,
      [] (double d, const PathState& s) {return s.len / s.velocity + d;});
}

std::vector<PathState>::const_iterator Path::cbegin() const {
  return this->states_.begin();
}

std::vector<PathState>::const_iterator Path::cend() const {
  return this->states_.end();
}

std::vector<PathState>::const_iterator Path::begin() const {
  return this->cbegin();
}

std::vector<PathState>::const_iterator Path::end() const {
  return this->cend();
}

std::vector<PathState>::iterator Path::begin() {
  return this->states_.begin();
}

std::vector<PathState>::iterator Path::end() {
  return this->states_.end();
}

const PathState& Path::operator[](size_t idx) const {
  return this->states_[idx];
}

PathState& Path::operator[](size_t idx) {
  return this->states_[idx];
}

const PathState& Path::back() const {
  return this->states_.back();
}

PathState& Path::back() {
  return this->states_.back();
}

Path& Path::operator+=(const Path& path) {
  for (auto&& new_state : path) {
    this->states_.emplace_back(new_state);
  }
  return *this;
}

size_t Path::size() const {
  return states_.size();
}

double Path::getDimMinX() const {
  return std::min_element(this->begin(), this->end(),
      [](const PathState& s1, const PathState& s2) {
        return s1.point.getX() < s2.point.getX();
      })->point.getX();
}

double Path::getDimMinY() const {
  return std::min_element(this->begin(), this->end(),
      [](const PathState& s1, const PathState& s2) {
        return s1.point.getY() < s2.point.getY();
      })->point.getY();
}

double Path::getDimMaxX() const {
  return std::max_element(this->begin(), this->end(),
      [](const PathState& s1, const PathState& s2) {
        return s1.point.getX() < s2.point.getX();
      })->point.getX();
}

double Path::getDimMaxY() const {
  return std::max_element(this->begin(), this->end(),
      [](const PathState& s1, const PathState& s2) {
        return s1.point.getY() < s2.point.getY();
      })->point.getY();
}

void Path::moveTo(const Point& p) {
  for (auto&& s : this->states_) {
    s.point = s.point + p;
  }
}

void Path::rotateFromPoint(const Point& p, double ang) {
  for (auto&& s : this->states_) {
    s.point = p.rotateFromPoint(ang, s.point);
    s.angle = s.point.mod_2pi(s.angle + ang);
  }
}

void Path::mirrorX() {
  for (auto&& s : this->states_) {
    s.point.setX(-s.point.getX());
    s.angle = s.point.mod_2pi(M_PI - s.angle);
  }
}
void Path::mirrorY() {
  for (auto&& s : this->states_) {
    s.point.setY(-s.point.getY());
    s.angle = s.point.mod_2pi(-s.angle);
  }
}

void Path::setBackwardDir() {
  for (size_t i = 0; i < this->size(); ++i) {
    this->setBackwardDir(i);
  }
}

void Path::setBackwardDir(int i) {
  this->states_[i].dir = PathDirection::BACKWARD;
}

void Path::setForwardDir() {
  for (size_t i = 0; i < this->size(); ++i) {
    this->setForwardDir(i);
  }
}

void Path::setForwardDir(int i) {
  this->states_[i].dir = PathDirection::FORWARD;
}

void Path::setSwathType() {
  for (size_t i = 0; i < this->size(); ++i) {
    this->setSwathType(i);
  }
}

void Path::setSwathType(int i) {
  this->states_[i].type = PathSectionType::SWATH;
}

void Path::setTurnType() {
  for (size_t i = 0; i < this->size(); ++i) {
    this->setTurnType(i);
  }
}

void Path::setTurnType(int i) {
  this->states_[i].type = PathSectionType::TURN;
}


double Path::length() const {
  return std::accumulate(states_.begin(), states_.end(), 0.0,
      [](double d, const PathState& s) {return d + fabs(s.len);});
}

void Path::appendSwath(const Swath& swath, double cruise_speed) {
  for (size_t i = 0; i < swath.numPoints() - 1; ++i) {
    PathState s;
    s.point = swath.getPoint(i);
    auto p_ang = Point(swath.getPoint(i + 1).getX() - swath.getPoint(i).getX(),
                       swath.getPoint(i + 1).getY() - swath.getPoint(i).getY());
    s.angle = p_ang.getAngleFromPoint();
    s.len = swath.getPoint(i + 1).distance(swath.getPoint(i));
    s.dir = PathDirection::FORWARD;
    s.type = PathSectionType::SWATH;
    s.velocity = cruise_speed;
    this->addState(s);
  }
}

PathState Path::at(double len) const {
  double total_length = this->length();
  if (len > total_length) {
    return this->at(total_length);
  } else if (len < 0.0) {
    return this->at(0.0);
  }

  PathState state_at;
  for (size_t i = 0; i < this->size(); ++i) {
    double len_state = fabs(states_[i].len);
    if (len_state < len) {
      len -= len_state;
    } else {
      state_at = states_[i];
      Point p = states_[i].point;
      double ang = states_[i].angle;

      len *= static_cast<double>(states_[i].dir);
      state_at.point = Point(p.getX() + len * cos(ang),
                             p.getY() + len * sin(ang));
      break;
    }
  }
  return state_at;
}

Point Path::atStart() const {
  return states_[0].point;
}

Point Path::atEnd() const {
  return states_.back().atEnd();
}

template<typename CharT>
class DecimalSeparator : public std::numpunct<CharT>{
 public:
  explicit DecimalSeparator(CharT Separator) : m_Separator(Separator) {}

 protected:
  CharT do_decimal_point() const {
    return m_Separator;
  }

 private:
  CharT m_Separator;
};

std::string to_string(double d, const int precision = 6) {
  std::stringstream ss;
  ss.precision(precision);
  ss.imbue(std::locale(std::locale(), new DecimalSeparator<char>('.')));
  ss << d;
  return ss.str();
}


std::string Path::serializePath(size_t digit_precision) const {
  std::locale::global(std::locale::classic());
  std::string res = "";

  for (size_t i = 0; i < size(); ++i) {
    res += to_string(states_[i].point.getX(), digit_precision) + " ";
    res += to_string(states_[i].point.getY(), digit_precision) + " ";
    res += to_string(states_[i].point.getZ(), digit_precision) + " ";
    res += to_string(states_[i].angle, digit_precision) + " ";
    res += to_string(states_[i].velocity, digit_precision) + " ";
    res += to_string(states_[i].len, digit_precision) + " ";
    res += to_string(static_cast<int>(states_[i].dir)) + " ";
    res += to_string(static_cast<int>(states_[i].type)) + "\n";
  }
  return res;
}

/**
 * @brief Discretize the swath sections of the path and return a new path
 * @param step_size Discretization step in [m]
 * @return New path with swath now discretized
*/
Path Path::discretizeSwath(double step_size) const {
  Path new_path;
  for (auto&& s : this->states_) {
    if (s.type == PathSectionType::SWATH) {
      double n_steps = max(1.0, std::round(fabs(s.len / step_size)));
      Point start2end = s.atEnd() - s.point;
      for (double j = 0.0; j < n_steps; j += 1.0) {
        PathState state = s;
        state.point = s.point + start2end * (j / n_steps);
        state.len /= n_steps;
        new_path.addState(state);
      }
    } else {
      new_path.addState(s);
    }
  }
  return new_path;
}

void Path::saveToFile(const std::string& file, size_t precision) const {
  std::ofstream out(file);
  out << serializePath(precision);
  out.close();
}

void Path::loadFile(const std::string& file) {
  std::ifstream in(file);
  double x, y, z, ang, vel, len, dir, d_type;
  while (in >> x >> y >> z >> ang >> vel >> len >> dir >> d_type) {
    PathState s;
    s.point = Point(x, y, z);
    s.angle = ang;
    s.velocity = vel;
    s.len = len;
    s.dir = static_cast<f2c::types::PathDirection>(dir);
    s.type = static_cast<f2c::types::PathSectionType>(d_type);
    this->states_.emplace_back(s);
  }
  in.close();
}

Path& Path::populate(int number_points) {
  if (this->size() < 3) { return *this;}

  std::vector<double> x, y, ang_prov, old_vel, len;
  std::vector<PathDirection> old_dir;
  std::vector<PathSectionType> old_type;
  for (int i = 0; i < this->size(); ++i) {
    if (this->states_[i].len != 0.0) {
      x.emplace_back(this->states_[i].point.getX());
      y.emplace_back(this->states_[i].point.getY());
      ang_prov.emplace_back(this->states_[i].angle);
      len.emplace_back(this->states_[i].len);
      old_vel.emplace_back(this->states_[i].velocity);
      old_dir.emplace_back(this->states_[i].dir);
      old_type.emplace_back(this->states_[i].type);
    }
  }
  x.emplace_back(this->states_.back().atEnd().getX());
  y.emplace_back(this->states_.back().atEnd().getY());
  ang_prov.emplace_back(this->states_.back().angle);
  len.emplace_back(0.0);
  old_vel.emplace_back(this->states_.back().velocity);
  old_dir.emplace_back(this->states_.back().dir);
  old_type.emplace_back(this->states_.back().type);

  auto ang = f2c::types::Point::getAngContinuity(ang_prov);
  std::vector<double> t(len.size());
  std::partial_sum(len.begin(), len.end() - 1, t.begin() + 1);

  CubicSpline s_x(t, x);
  CubicSpline s_y(t, y);
  CubicSpline s_ang(t, ang);

  this->states_.clear();

  double step = t.back() / static_cast<double>(number_points);
  PathState state;
  for (size_t i = 0; i < number_points; ++i) {
    double d = i * step;
    state.point = Point(s_x(d), s_y(d));
    state.angle = Point::mod_2pi(s_ang(d));
    state.len = step;
    int it = std::lower_bound(t.begin(), t.end(), d + 1e-10) - t.begin() - 1;
    state.velocity = old_vel[it];
    state.dir = old_dir[it];
    state.type = old_type[it];
    this->addState(state);
  }
  return *this;
}

Path& Path::reduce(double min_dist_equal) {
  size_t N = this->size();
  Path new_path;
  for (size_t i = 0; i < N;) {
    double new_len {this->states_[i].len};
    int j = i + 1;
    while (j < N &&
        this->states_[i].type == this->states_[j].type &&
        this->states_[i].dir == this->states_[j].dir &&
        this->states_[i].point.distance(
          this->states_[j].point) < min_dist_equal) {
      new_len += this->states_[j].len;
      ++j;
    }
    PathState state;
    state.point = this->states_[i].point;
    state.angle = this->states_[i].angle;
    state.velocity = this->states_[i].velocity;
    state.len = new_len;
    state.dir = this->states_[i].dir;
    state.type = this->states_[i].type;
    new_path.states_.emplace_back(state);
    i = j;
  }
  *this = new_path;
  return *this;
}

LineString Path::toLineString() const {
  LineString line;
  for (auto&& s : this->states_) {
    line.addPoint(s.point);
  }
  line.addPoint(this->atEnd());
  return line;
}

Path& Path::discretize(double step_size) {
  if (this->size() > 1) {
    int n = static_cast<int>((10.0 / step_size) * this->length());
    this->populate(n);
    this->reduce(step_size);
  }
  return *this;
}

}  // namespace f2c::types

