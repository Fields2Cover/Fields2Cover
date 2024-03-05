//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Field.h"
#include <regex>

namespace f2c::types {

Field::Field() = default;
Field::Field(const Cells& cells, const std::string& id_p) : id_(id_p) {
  if (!cells.isEmpty()) {
    this->ref_point_ = cells.getCellBorder(0).startPoint();
  }
  this->field_ = cells - this->ref_point_;
}

Field::~Field() = default;
Field::Field(const Field&) = default;
Field::Field(Field&&) = default;
Field& Field::operator=(Field&&) = default;
Field& Field::operator=(const Field&) = default;



std::string Field::getId() const {
  return this->id_;
}

void Field::setId(const std::string& _id) {
  this->id_ = _id;
}

std::string Field::getCRS() const {
  return this->coord_sys_;
}

void Field::setCRS(const std::string& crs) {
  this->coord_sys_ = crs;
}

std::string Field::getPrevCRS() const {
  return this->prev_coord_sys_;
}

void Field::setPrevCRS(const std::string& prev_crs) {
  this->prev_coord_sys_ = prev_crs;
}

Point& Field::getRefPoint() {
  return this->ref_point_;
}

const Point& Field::getRefPoint() const {
  return this->ref_point_;
}

void Field::setRefPoint(const Point& _ref_point) {
  this->ref_point_ = _ref_point;
}

Cells& Field::getField() {
  return this->field_;
}

const Cells& Field::getField() const {
  return this->field_;
}

void Field::setField(const Cells& _field) {
  this->field_ = _field;
}

Field Field::clone() const {
  Field new_field {this->field_ + this->ref_point_, this->id_};
  new_field.coord_sys_ = this->coord_sys_;
  return new_field;
}

double Field::area(void) const {
  return this->field_.area();
}

bool Field::isEmpty() const {
  return this->field_.isEmpty();
}

bool Field::isCoordSystemUTM(const std::string& coord_sys) {
  return coord_sys.find("UTM:") != std::string::npos;
}

bool Field::isCoordSystemUTM() const {
  return isCoordSystemUTM(this->coord_sys_);
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
  return getUTMCoordSystem(this->coord_sys_);
}

std::string Field::getUTMDatum() const {
  return getUTMDatum(this->coord_sys_);
}

bool Field::isCoordSystemEPSG(const std::string& coord_sys) {
  return coord_sys.find("EPSG:") != std::string::npos;
}

bool Field::isCoordSystemEPSG() const {
  return isCoordSystemEPSG(this->coord_sys_);
}

int Field::getEPSGCoordSystem(const std::string& coord_sys) {
  return isCoordSystemEPSG(coord_sys) ?
    std::stoi(coord_sys.substr(5)) : -1;
}

int Field::getEPSGCoordSystem() const {
  return getEPSGCoordSystem(this->coord_sys_);
}

void Field::setEPSGCoordSystem(int epsg) {
  this->coord_sys_ = "EPSG:" + std::to_string(epsg);
}

void Field::setUTMCoordSystem(const std::string& utm) {
  this->coord_sys_ =
    "UTM:" + getUTMCoordSystem(utm, utm) +
    " datum:" + getUTMDatum(utm);
}

void Field::setUTMCoordSystem(
    const std::string& utm, const std::string& datum) {
  this->coord_sys_ =
    "UTM:" + getUTMCoordSystem(utm, utm) +
    " datum:" + getUTMDatum(datum, datum);
}

Cells Field::getCellsAbsPosition() const {
  return this->field_ + this->ref_point_;
}

std::string Field::getUTMZone(const std::string& coord_sys) {
  return getUTMCoordSystem(coord_sys).substr(0, 2);
}

std::string Field::getUTMZone() const {
  return getUTMZone(this->coord_sys_);
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
  return getUTMHemisphere(this->coord_sys_);
}

}  // namespace f2c::types

