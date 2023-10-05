//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_FIELD_H_
#define FIELDS2COVER_TYPES_FIELD_H_

#include <string>
#include <memory>
#include "fields2cover/types/Point.h"
#include "fields2cover/types/Cells.h"

namespace f2c::types {

struct Field {
 public:
  std::string id {""};
  std::string coord_sys {""};
  std::string prev_coord_sys {""};
  Point ref_point;
  Cells field;

 public:
  Field();
  explicit Field(const Cells& cells, const std::string& id = "");
  ~Field();
  Field(const Field&);
  Field(Field&&);
  Field& operator=(Field&&);
  Field& operator=(const Field&);

 public:
  void setId(const std::string& _id) {id = _id;}
  std::string getId() const {return id;}
  void setCRS(const std::string& crs) {coord_sys = crs;}
  std::string getCRS() const {return coord_sys;}
  void setPrevCRS(const std::string& prev_crs) {prev_coord_sys = prev_crs;}
  std::string getPrevCRS() const {return prev_coord_sys;}
  void setRefPoint(const Point& _ref_point) {ref_point = _ref_point;}
  Point getRefPoint() const {return ref_point;}
  void setField(const Cells& _field) {field = _field;}
  Cells getField() const {return field;}

 public:
  Field clone() const;
  double getArea() const;

  static bool isCoordSystemUTM(const std::string& coord_sys);
  bool isCoordSystemUTM() const;
  static std::string getUTMCoordSystem(const std::string& coord_sys,
    const std::string& if_not_found = "");
  static std::string getUTMDatum(const std::string& coord_sys,
    const std::string& if_not_found = "etrs89");
  static std::string getUTMZone(const std::string& coord_sys);
  static std::string getUTMHemisphere(const std::string& coord_sys);

  std::string getUTMCoordSystem() const;
  std::string getUTMDatum() const;
  std::string getUTMZone() const;
  std::string getUTMHemisphere() const;
  static bool isCoordSystemEPSG(const std::string& coord_sys);
  bool isCoordSystemEPSG() const;
  static int getEPSGCoordSystem(const std::string& coord_sys);
  int getEPSGCoordSystem() const;
  void setEPSGCoordSystem(int epsg);
  void setUTMCoordSystem(const std::string& utm);
  void setUTMCoordSystem(const std::string& utm, const std::string& datum);
  Cells getCellsAbsPosition() const;
};

}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_FIELD_H_
