//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Field.h"
#include <regex>

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

std::string Field::getUTMCoordSystem(
    const std::string& coord_sys, const std::string& if_not_found) {
  std::smatch match;
  if (std::regex_search(coord_sys, match,
        std::regex("UTM[^0-9A-Za-z]*(\\d++\\w)[^\\w]*"))) {
    return match.str(1);
  }
  return if_not_found;
}

std::string Field::getUTMDatum(
    const std::string& coord_sys, const std::string& if_not_found) {
  std::smatch match;
  if (std::regex_search(coord_sys, match,
        std::regex("datum[^0-9A-Za-z]*([0-9A-Za-z]+)"))) {
    return match.str(1);
  }
  return if_not_found;
}

std::string Field::getUTMCoordSystem() const {
  return getUTMCoordSystem(coord_sys);
}

std::string Field::getUTMDatum() const {
  return getUTMDatum(this->coord_sys);
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
  this->coord_sys = "EPSG:" + std::to_string(epsg);
}

void Field::setUTMCoordSystem(const std::string& utm) {
  this->coord_sys =
    "UTM:" + getUTMCoordSystem(utm, utm) +
    " datum:" + getUTMDatum(utm);
}

void Field::setUTMCoordSystem(
    const std::string& utm, const std::string& datum) {
  this->coord_sys =
    "UTM:" + getUTMCoordSystem(utm, utm) +
    " datum:" + getUTMDatum(datum, datum);
}

Cells Field::getCellsAbsPosition() const {
  return this->field + this->ref_point;
}

std::string Field::getUTMZone(const std::string& coord_sys) {
  return getUTMCoordSystem(coord_sys).substr(0, 2);
}

std::string Field::getUTMZone() const {
  return getUTMZone(coord_sys);
}

std::string Field::getUTMHemisphere(const std::string& coord_sys) {
  std::string hemisphere {getUTMCoordSystem(coord_sys).substr(2, 1)};
  if (hemisphere == "n" || hemisphere == "N") {
    return "+north";
  } else if (hemisphere == "s" || hemisphere == "S") {
    return "+south";
  } else {
    return "";
  }
}

std::string Field::getUTMHemisphere() const {
  return getUTMHemisphere(coord_sys);
}

}  // namespace f2c::types

