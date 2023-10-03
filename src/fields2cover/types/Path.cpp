//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <spline.h>
#include <numeric>
#include "fields2cover/types/Path.h"

namespace f2c::types {

PathState PathState::clone() const {
  PathState new_state {*this};
  new_state.point = this->point.clone();
  return new_state;
}

Path& Path::operator+=(const Path& path) {
  for (auto&& state : path.states) {
    states.emplace_back(state.clone());
  }
  task_time += path.task_time;
  return *this;
}

Path Path::clone() const {
  Path new_path;
  new_path += *this;
  return new_path;
}

size_t Path::size() const {
  return states.size();
}

void Path::moveTo(const Point& p) {
  for (auto&& s : this->states) {
    s.point = s.point + p;
  }
}

double Path::length() const {
  if (size() < 2) { return 0.0;}
  double total_length {0.0};
  for (size_t i = 0; i < size() - 1; ++i) {
    total_length += states[i].point.Distance(states[i+1].point);
  }
  return total_length;
}

void Path::appendSwath(const Swath& swath, double cruise_speed) {
  for (size_t i = 0; i < swath.getNumPoints(); ++i) {
    PathState s;
    s.point = swath.getPoint(i).clone();
    if (i == swath.getNumPoints() - 1) {
      s.duration = 0.0;
      s.angle = Point(swath.getPoint(i).getX() - swath.getPoint(i - 1).getX(),
          swath.getPoint(i).getY() - swath.getPoint(i-1).getY())
            .getAngleFromPoint();
    } else {
      s.duration = swath.getPoint(i + 1).Distance(
          swath.getPoint(i)) / cruise_speed;
      task_time += s.duration;
      s.angle = Point(swath.getPoint(i + 1).getX() - swath.getPoint(i).getX(),
          swath.getPoint(i + 1).getY() - swath.getPoint(i).getY())
            .getAngleFromPoint();
    }
    s.velocity = cruise_speed;
    s.dir = PathDirection::FORWARD;
    s.type = PathSectionType::SWATH;
    states.emplace_back(s);
  }
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
    res += to_string(states[i].point.getX(), digit_precision) + " ";
    res += to_string(states[i].point.getY(), digit_precision) + " ";
    res += to_string(states[i].point.getZ(), digit_precision) + " ";
    res += to_string(states[i].angle, digit_precision) + " ";
    res += to_string(states[i].velocity, digit_precision) + " ";
    res += to_string(states[i].duration, digit_precision) + " ";
    res += to_string(static_cast<int>(states[i].dir)) + " ";
    res += to_string(static_cast<int>(states[i].type)) + "\n";
  }
  return res;
}

/**
 * @brief Discretize the swath sections of the path and return a new path
 * @param step_size Discretization step in [m]
 * @return New path with swath now discretized
*/
Path Path::discretize_swath(double step_size) const {
  // Create new path
  Path new_path;

  // Loop through all the points in the path
  for (size_t i = 0; i < this->size()-1; i++) {
    // Check if the current point and next point is a SWATH
    if (this->states.at(i).type == f2c::types::PathSectionType::SWATH
        && this->states.at(i+1).type == f2c::types::PathSectionType::SWATH) {
      // We know that swaths are represented by only two point so the next point will
      // be the end of the swath
      // Save start and end swath points to variables
      Point start_point = this->states.at(i).point;
      Point end_point = this->states.at(i+1).point;

      // Calculate the distance between the start and end point
      double distance = start_point.Distance(end_point);
      // Calculate the number of steps using the input step size
      double number_of_steps = fabs(distance / step_size);
      // Round the number of steps to the nearest integer
      int rounded_number_of_steps = std::round(number_of_steps);
      // If rounded number of steps is equal to zero, then the provided step_size is greater
      // than the distance. In this case, set number of steps to 1, thus do not discretize the path
      if (rounded_number_of_steps == 0) {
        rounded_number_of_steps = 1;
      }

      // Iterate over each pair of coordinates and add them as states into our new Path object
      for (int j = 0; j <= rounded_number_of_steps; j++) {
        // Create a new PathState object
        PathState state;
        // Update point with incremental step values
        state.point = Point(
            start_point.getX() +
              (j * (end_point.getX() - start_point.getX()) / rounded_number_of_steps),
            start_point.getY() +
              (j * (end_point.getY() - start_point.getY()) / rounded_number_of_steps),
            start_point.getZ() +
              (j * (end_point.getZ() - start_point.getZ()) / rounded_number_of_steps));
        // Set angle
        state.angle = this->states.at(i).angle;
        // Set velocity
        state.velocity = this->states.at(i).velocity;
        // Set duration
        state.duration = static_cast<double>((distance/state.velocity)/rounded_number_of_steps);
        state.dir = this->states.at(i).dir;
        state.type = f2c::types::PathSectionType::SWATH;
        new_path.states.push_back(state);
      }
    } else if (this->states.at(i).type == f2c::types::PathSectionType::TURN) {
      // If the current point is not a swath point, we can just add it to the new path
      new_path.states.push_back(this->states.at(i));
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
  double x, y, z, ang, vel, dur, dir, d_type;
  while (in >> x >> y >> z >> ang >> vel >> dur >> dir >> d_type) {
    PathState s;
    s.point = Point(x, y, z);
    s.angle = ang;
    s.velocity = vel;
    s.duration = dur;
    s.dir = static_cast<f2c::types::PathDirection>(dir);
    s.type = static_cast<f2c::types::PathSectionType>(d_type);
    states.emplace_back(s);
  }
  in.close();
  task_time = std::accumulate(states.begin(), states.end(), 0.0,
      [] (double b, const PathState& a) {return a.duration + b;});
}

Path& Path::populate(int number_points) {
  if (this->size() < 3) { return *this;}

  std::vector<double> x, y, ang_prov, vel, dur;
  std::vector<PathDirection> old_dir;
  std::vector<PathSectionType> old_type;
  for (int i = 0; i < this->size(); ++i) {
    if (this->states[i].duration != 0.0 ||  i == this->size() - 1) {
      x.emplace_back(this->states[i].point.getX());
      y.emplace_back(this->states[i].point.getY());
      ang_prov.emplace_back(this->states[i].angle);
      vel.emplace_back(this->states[i].velocity);
      dur.emplace_back(this->states[i].duration);
      old_dir.emplace_back(this->states[i].dir);
      old_type.emplace_back(this->states[i].type);
    }
  }
  auto ang = f2c::types::Point::getAngContinuity(ang_prov);
  std::vector<double> t(dur.size());
  std::partial_sum(dur.begin(), dur.end() - 1, t.begin() + 1);

  tk::spline s_x(t, x);
  tk::spline s_y(t, y);
  tk::spline s_ang(t, ang);
  tk::spline s_vel(t, vel);

  states.clear();

  double step = t.back() / static_cast<double>(number_points - 1);
  PathState state;
  for (double d = 0.0; d < t.back(); d += step) {
    state.point = Point(s_x(d), s_y(d));
    state.angle = Point::mod_2pi(s_ang(d));
    state.velocity = s_vel(d);
    state.duration = step;
    int it = std::lower_bound(t.begin(), t.end(), d) - t.begin();
    state.dir = old_dir[it];
    state.type = old_type[it];
    this->states.emplace_back(state);
  }

  state.point = Point(s_x(t.back()), s_y(t.back()));
  state.angle = Point::mod_2pi(s_ang(t.back()));
  state.velocity = abs(s_vel(t.back()));
  state.duration = 0.0;
  state.dir = old_dir.back();
  state.type = old_type.back();
  this->states.emplace_back(state);

  task_time = std::accumulate(states.begin(), states.end(), 0.0,
      [] (double b, const PathState& a) {return a.duration + b;});
  return *this;
}

Path& Path::reduce(double min_dist_equal) {
  size_t N = this->size();
  Path new_path;
  for (size_t i = 0; i < N;) {
    double new_dur {this->states[i].duration};
    int j = i + 1;
    while (j < N &&
        this->states[i].type == this->states[j].type &&
        this->states[i].dir == this->states[j].dir &&
        this->states[i].point.Distance(
          this->states[j].point) < min_dist_equal) {
      new_dur += this->states[j].duration;
      ++j;
    }
    PathState state;
    state.point = this->states[i].point;
    state.angle = this->states[i].angle;
    state.velocity = this->states[i].velocity;
    state.duration = new_dur;
    state.dir = this->states[i].dir;
    state.type = this->states[i].type;
    new_path.states.emplace_back(state);
    i = j;
  }
  // Reducing number of points may change the task_time of the path.
  // Issue to be aware
  new_path.task_time = std::accumulate(
      new_path.states.begin(), new_path.states.end(), 0.0,
      [] (double b, const PathState& a) {return a.duration + b;});
  new_path.measure_error = this->measure_error;
  *this = new_path;
  return *this;
}



}  // namespace f2c::types

