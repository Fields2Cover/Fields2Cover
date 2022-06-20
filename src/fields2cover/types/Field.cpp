//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Field.h"

namespace f2c::types {

Field::Field() = default;
Field::Field(const Cells& cells, const std::string& id_p) : id(id_p) {
  if (!cells.isEmpty()) {
    ref_point = cells.getCellBorder(0).StartPoint().clone();
  }
  field = cells - ref_point;
}

Field::~Field() = default;
Field::Field(const Field&) = default;
Field::Field(Field&&) = default;
Field& Field::operator=(Field&&) = default;
Field& Field::operator=(const Field&) = default;

Field Field::clone() const {
  Field new_field {this->field + this->ref_point, this->id};
  new_field.coord_sys = this->coord_sys;
  return new_field;
}

double Field::getArea(void) const {
  return field.getArea();
}

bool Field::isCoordSystemUTM(const std::string& coord_sys) {
  return coord_sys.find("UTM:") != std::string::npos;
}

bool Field::isCoordSystemUTM() const {
  return isCoordSystemUTM(coord_sys);
}

std::string Field::getUTMCoordSystem(const std::string& coord_sys) {
  return isCoordSystemUTM(coord_sys) ? coord_sys.substr(4) : "";
}

std::string Field::getUTMCoordSystem() const {
  return getUTMCoordSystem(coord_sys);
}

bool Field::isCoordSystemEPSG(const std::string& coord_sys) {
  return coord_sys.find("EPSG:") != std::string::npos;
}

bool Field::isCoordSystemEPSG() const {
  return isCoordSystemEPSG(coord_sys);
}

int Field::getEPSGCoordSystem(const std::string& coord_sys) {
  return isCoordSystemEPSG(coord_sys) ?
    std::stoi(coord_sys.substr(5)) : -1;
}

int Field::getEPSGCoordSystem() const {
  return getEPSGCoordSystem(coord_sys);
}

void Field::setEPSGCoordSystem(int epsg) {
  coord_sys = "EPSG:" + std::to_string(epsg);
}

void Field::setUTMCoordSystem(const std::string& utm) {
  coord_sys = "UTM:" + utm;
}

}  // namespace f2c::types

