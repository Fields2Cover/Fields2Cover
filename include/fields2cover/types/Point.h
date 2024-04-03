//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_POINT_H_
#define FIELDS2COVER_TYPES_POINT_H_

#include <gdal/ogr_geometry.h>
#include <iostream>
#include <functional>
#include <memory>
#include <utility>
#include <vector>
#include "fields2cover/types/Geometry.h"

namespace f2c::types {

struct Point : public Geometry<OGRPoint, wkbPoint> {
 public:
  using Geometry<OGRPoint, wkbPoint>::Geometry;
  Point();
  Point(double x, double y, double z = 0);
  Point(const Point&);
  Point(Point&&);
  ~Point();
  Point& operator=(const Point&);
  Point& operator=(Point&&);

 public:
  bool operator==(const Point& b) const;

  bool operator!=(const Point& b) const;

  bool operator<(const Point& b) const;

  Point operator+(const Point& b) const;

  Point operator-(const Point& b) const;

  Point& operator*=(double b);

  Point operator*(double b) const;
  double operator*(const Point& b) const;

  Point operator/(double b) const;

  static double det(const Point& u, const Point& v);


 public:
  Point clone() const;
  double X() const;
  double Y() const;
  double Z() const;

  double getX() const;
  double getY() const;
  double getZ() const;
  void setX(double x);
  void setY(double y);
  void setZ(double z);

  void setPoint(double x, double y, double z = 0);
  void setPoint(const OGRPoint& p);
  void setPoint(const Point& p);

  double getAngleFromPoints(const Point& end) const;
  double getAngleFromPoint() const;
  /// Angle between vectors p1->p2 and p2->p3
  static double getAngleFromPoints(
      const Point& p1, const Point& p2, const Point& p3);
  Point getPointFromAngle(double angle, double dist) const;

  Point rotateFromPoint(double angle, const Point& p_r) const;

  double signedDistance2Segment(const Point& start, const Point& end) const;
  static Point intersectionOfLines(
      const Point& l1_s, const Point& l1_e,
      const Point& l2_s, const Point& l2_e);

  Point closestPointInSegment(const Point& seg_s, const Point& seg_e) const;

  template <class T>
  std::vector<T> rotateFromPoint(double angle, const std::vector<T>& t) const;

  template <class T>
  T rotateFromPoint(double angle, const T& t) const;
};

std::ostream& operator<<(std::ostream& os, const Point& p);

template <class T>
std::vector<T> Point::rotateFromPoint(
    double angle, const std::vector<T>& t) const {
  std::vector<T> res;
  for (auto&& p : t) {
    res.emplace_back(this->rotateFromPoint(angle, p));
  }
  return res;
}

template <class T>
T Point::rotateFromPoint(double angle, const T& t) const {
  T res;
  for (auto&& p : t) {
    res.addGeometry(this->rotateFromPoint(angle, p));
  }
  return res;
}


inline OGRPoint operator+(const OGRPoint& a, const f2c::types::Point& b) {
  return {a.getX() + b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ()};
}

inline OGRPoint operator-(const OGRPoint& a, const f2c::types::Point& b) {
  return {a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ()};
}

template <class T>
inline std::vector<T> operator+(
    const std::vector<T>& t, const f2c::types::Point& dir) {
  std::vector<T> res;
  for (auto&& p : t) {
    res.emplace_back(p + dir);
  }
  return res;
}

template <class T>
inline std::vector<T> operator-(
    const f2c::types::Point& dir, const std::vector<T>& t) {
  std::vector<T> res;
  for (auto&& p : t) {
    res.emplace_back(dir - p);
  }
  return res;
}

template <class T>
inline std::vector<T> operator-(
    const std::vector<T>& t, const f2c::types::Point& dir) {
  std::vector<T> res;
  for (auto&& p : t) {
    res.emplace_back(p - dir);
  }
  return res;
}

template <class T>
inline T operator+(const T& t, const f2c::types::Point& dir) {
  T res;
  for (auto&& p : t) {
    res.addGeometry(p + dir);
  }
  return res;
}

template <class T>
inline T operator-(const f2c::types::Point& dir, const T& t) {
  T res;
  for (auto&& p : t) {
    res.addGeometry(dir - p);
  }
  return res;
}

template <class T>
inline T operator-(const T& t, const f2c::types::Point& dir) {
  T res;
  for (auto&& p : t) {
    res.addGeometry(p - dir);
  }
  return res;
}

}  // namespace f2c::types

namespace std {
template<>
struct hash<f2c::types::Point> {
  inline size_t operator()(const f2c::types::Point& p) const {
    return size_t(p.getX() + p.getY() * 1e10 + p.getZ() * 1e20);
  }
};
}

#endif  // FIELDS2COVER_TYPES_POINT_H_
