//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_SWATH_H_
#define FIELDS2COVER_TYPES_SWATH_H_

#include <gdal/ogr_geometry.h>

#include <algorithm>
#include <memory>
#include <utility>
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/Cells.h"

namespace f2c::types {

enum class SwathType {MAINLAND = 0, HEADLAND = 1};

struct Swath {
 public:
  Swath();
  explicit Swath(const LineString& path);
  explicit Swath(double width);
  Swath(const LineString& path, double width,
      int id = 0, SwathType type = SwathType::MAINLAND);
  Swath(const Swath&);
  virtual ~Swath();
  virtual Swath& operator=(Swath&&);
  virtual Swath& operator=(const Swath&);
  bool operator!=(const Swath& s) const;
  bool operator==(const Swath& s) const;
  bool operator>=(const Swath& s) const;
  bool operator<=(const Swath& s) const;
  bool operator>(const Swath& s) const;
  bool operator<(const Swath& s) const;

 public:
  void setId(int id);
  int getId() const;

  void setPath(const LineString& path);
  LineString getPath() const;

  void setWidth(double width);
  double getWidth() const;

  bool getCreationDir() const;
  void setCreationDir(bool creation_dir);

  SwathType getType() const;
  void setType(SwathType type);

 public:
  double length() const;

  double area() const;
  double area(const Cells& polys) const;

  void reverse();

  double getInAngle() const;

  double getOutAngle() const;

  Point startPoint() const;

  Point endPoint() const;

  Cells areaCovered() const;
  Cells areaCovered(const Cells& polys) const;
  Swath clone() const;

  size_t numPoints() const;

  Point getPoint(int i);
  const Point getPoint(int i) const;


  /// Check if the difference between swaths angles is less than pi.
  bool hasSameDir(const Swath& s) const;

  /// Reverse the direction of the swath if the difference of the angles is
  /// greater than pi
  void targetSameDirAs(const Swath& s);

  /// Reverse the direction of the swath if the difference of the angles is
  /// less than pi
  void targetOppositeDirAs(const Swath& s);

  /// Moves swath data by a reference point
  void moveTo(const Point& ref_pt);

 private:
  int id_ {0};  // Id of the swath
  LineString path_;
  double width_ {0.0};
  bool creation_dir_ {true};
  SwathType type_ {SwathType::MAINLAND};
};


}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_SWATH_H_
