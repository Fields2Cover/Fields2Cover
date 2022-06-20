//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <spline.h>
#include <numeric>
#include "fields2cover/types/Path.h"

namespace f2c::types {

Path& Path::operator+=(const Path& path) {
  for (auto&& p : path.points) {
    points.emplace_back(p.clone());
  }
  angles.insert(std::end(angles),
      std::begin(path.angles), std::end(path.angles));
  velocities.insert(std::end(velocities),
      std::begin(path.velocities), std::end(path.velocities));
  directions.insert(std::end(directions),
      std::begin(path.directions), std::end(path.directions));
  durations.insert(std::end(durations),
      std::begin(path.durations), std::end(path.durations));
  type.insert(std::end(type),
      std::begin(path.type), std::end(path.type));
  task_time += path.task_time;
  return *this;
}

Path Path::clone() const {
  Path new_path;
  for (const auto& p : points) {
    new_path.points.emplace_back(p.getX(), p.getY());
  }

  new_path.angles = angles;
  new_path.velocities = velocities;
  new_path.durations = durations;
  new_path.directions = directions;
  new_path.type = type;
  new_path.task_time = task_time;
  new_path.measure_error = measure_error;
  return new_path;
}

size_t Path::size() const {
  return points.size();
}

void Path::moveTo(const Point& p) {
  for (auto&& point : this->points) {
    point = point + p;
  }
}

double Path::length() const {
  if (points.size() < 2) { return 0.0;}
  double total_length {0.0};
  for (size_t i = 0; i < points.size() - 1; ++i) {
    total_length += points[i].Distance(points[i+1]);
  }
  return total_length;
}

void Path::appendSwath(const Swath& swath, double cruise_speed) {
  for (size_t i = 0; i < swath.getNumPoints(); ++i) {
    points.emplace_back(swath.getPoint(i).clone());
    type.emplace_back(PathSectionType::SWATH);
    directions.emplace_back(PathDirection::FORWARD);
    double ang {0.0};
    velocities.emplace_back(cruise_speed);
    if (i == swath.getNumPoints() - 1) {
      durations.emplace_back(0.0);
      ang = Point(swath.getPoint(i).getX() - swath.getPoint(i - 1).getX(),
          swath.getPoint(i).getY() - swath.getPoint(i-1).getY())
            .getAngleFromPoint();
    } else {
      double duration = swath.getPoint(i + 1).Distance(
          swath.getPoint(i)) / cruise_speed;
      durations.emplace_back(duration);
      task_time += duration;
      ang = Point(swath.getPoint(i + 1).getX() - swath.getPoint(i).getX(),
          swath.getPoint(i + 1).getY() - swath.getPoint(i).getY())
            .getAngleFromPoint();
    }
    angles.emplace_back(ang);
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

std::string to_string(double d) {
  std::stringstream ss;
  ss.imbue(std::locale(std::locale(), new DecimalSeparator<char>('.')));
  ss << d;
  return ss.str();
}


std::string Path::serializePath() const {
  std::locale::global(std::locale::classic());
  std::string res = "";

  for (size_t i = 0; i < points.size(); ++i) {
    res += to_string(points[i].getX()) + " ";
    res += to_string(points[i].getY()) + " ";
    res += to_string(angles[i]) + " ";
    res += to_string(velocities[i]) + " ";
    res += to_string(durations[i]) + " ";
    res += to_string(directions[i]) + " ";
    res += to_string(type[i]) + "\n";
  }
  return res;
}

void Path::saveToFile(const std::string& file) const {
  std::ofstream out(file);
  out << serializePath();
  out.close();
}

void Path::loadFile(const std::string& file) {
  std::ifstream in(file);
  double x, y, ang, vel, dur, dir, d_type;
  while (in >> x >> y >> ang >> vel >> dur >> dir >> d_type) {
    points.emplace_back(Point(x, y));
    angles.emplace_back(ang);
    velocities.emplace_back(vel);
    durations.emplace_back(dur);
    directions.emplace_back(static_cast<f2c::types::PathDirection>(dir));
    type.emplace_back(static_cast<f2c::types::PathSectionType>(d_type));
  }
  in.close();
}

bool Path::isValid() const {
  return (points.size() == angles.size() &&
    points.size() == velocities.size() &&
    points.size() == durations.size() &&
    points.size() == directions.size() &&
    points.size() == type.size() &&
    fabs(std::accumulate(durations.begin(), durations.end(), -task_time))
      < 1e-3);
}

Path& Path::populate(int number_points) {
  if (points.size() < 3) { return *this;}

  std::vector<double> x, y, ang_prov, vel, dur;
  for (int i = 0; i < this->points.size(); ++i) {
    if (this->durations[i] != 0.0 ||  i == this->points.size() - 1) {
      x.emplace_back(this->points[i].getX());
      y.emplace_back(this->points[i].getY());
      ang_prov.emplace_back(this->angles[i]);
      vel.emplace_back(this->velocities[i]);
      dur.emplace_back(this->durations[i]);
    }
  }

  auto ang = f2c::types::Point::getAngContinuity(ang_prov);

  std::vector<double> t(dur.size());
  std::partial_sum(dur.begin(), dur.end() - 1, t.begin() + 1);


  tk::spline s_x(t, x);
  tk::spline s_y(t, y);
  tk::spline s_ang(t, ang);
  tk::spline s_vel(t, vel);

  points.clear();
  angles.clear();
  velocities.clear();
  durations.clear();
  std::vector<PathDirection> new_dir;
  std::vector<PathSectionType> new_type;

  double step = t.back() / static_cast<double>(number_points - 1);
  for (double d = 0.0; d < t.back(); d += step) {
    this->points.emplace_back(s_x(d), s_y(d));
    this->angles.emplace_back(f2c::types::Point::mod_2pi(s_ang(d)));
    this->velocities.emplace_back(s_vel(d));
    this->durations.emplace_back(step);
    int it = std::lower_bound(t.begin(), t.end(), d) - t.begin();
    new_dir.emplace_back(directions[it]);
    new_type.emplace_back(type[it]);
  }
  this->points.emplace_back(s_x(t.back()), s_y(t.back()));
  this->angles.emplace_back(f2c::types::Point::mod_2pi(s_ang(t.back())));
  this->velocities.emplace_back(abs(s_vel(t.back())));
  this->durations.emplace_back(0.0);
  new_dir.emplace_back((s_vel(t.back()) > 0) ?
    f2c::types::PathDirection::FORWARD :
    f2c::types::PathDirection::BACKWARD);
  new_type.emplace_back(type.back());

  this->directions = new_dir;
  this->type = new_type;

  return *this;
}

Path& Path::reduce(double min_dist_equal) {
  size_t N = points.size();
  Path new_path;
  for (size_t i = 0; i < N;) {
    double new_dur {this->durations[i]};
    int j = i + 1;
    while (j < N &&
        this->type[i] == this->type[j] &&
        this->directions[i] == this->directions[j] &&
        this->points[i].Distance(this->points[j]) < min_dist_equal) {
      new_dur += this->durations[j];
      ++j;
    }
    new_path.points.emplace_back(this->points[i]);
    new_path.angles.emplace_back(this->angles[i]);
    new_path.velocities.emplace_back(this->velocities[i]);
    new_path.durations.emplace_back(new_dur);
    new_path.directions.emplace_back(this->directions[i]);
    new_path.type.emplace_back(this->type[i]);
    i = j;
  }
  // Reducing number of points may change the task_time of the path.
  // Issue to be aware
  new_path.task_time = std::accumulate(new_path.durations.begin(),
      new_path.durations.end(), 0.0);
  new_path.measure_error = this->measure_error;
  *this = new_path;
  return *this;
}



}  // namespace f2c::types

