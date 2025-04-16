//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_FIELD_H_
#define FIELDS2COVER_TYPES_FIELD_H_

#include <string>
#include <memory>
#include <utility>
#include <vector>
#include "fields2cover/types/Point.h"
#include "fields2cover/types/Cells.h"

namespace f2c::types {

struct Field {
 public:
  Field();
  explicit Field(const Cells& cells, const std::string& id = "");
  explicit Field(const Cells& cells, const std::string& id,
      const std::vector<std::pair<std::string,
                                  std::vector<Point>>>& reload_points);
  ~Field();
  Field(const Field&);
  Field(Field&&);
  Field& operator=(Field&&);
  Field& operator=(const Field&);

  std::string getId() const;
  void setId(const std::string& _id);
  std::string getCRS() const;
  void setCRS(const std::string& crs);
  std::string getPrevCRS() const;
  void setPrevCRS(const std::string& prev_crs);
  Point& getRefPoint();
  const Point& getRefPoint() const;
  void setRefPoint(const Point& _ref_point);
  Cells& getField();
  const Cells& getField() const;
  void setField(const Cells& _field);
  std::vector<std::pair<std::string, std::vector<Point>>>
      getReloadPoints() const;
  void setReloadPoints(const std::vector<std::pair<
      std::string, std::vector<Point>>>& _reload_points);

  Field clone() const;
  double area() const;
  bool isEmpty() const;

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
  std::vector<std::pair<std::string, std::vector<Point>>>
      getReloadPointsAbsPosition() const;

 private:
  std::string id_ {""};
  std::string coord_sys_ {""};
  std::string prev_coord_sys_ {""};
  Point ref_point_;
  Cells field_;

  // First parameter is the name of the capacity
  std::vector<std::pair<std::string, std::vector<Point>>> reload_points_;
};

}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_FIELD_H_
