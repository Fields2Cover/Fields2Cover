//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/utils/transformation.h"

namespace f2c {

std::unique_ptr<OGRCoordinateTransformation,
  void(*)(OGRCoordinateTransformation*)>
    Transform::generateCoordTransf(
        const std::string& _coord_sys_from, const std::string& _coord_sys_to) {
  return createCoordTransf(
      createSptRef(_coord_sys_from), createSptRef(_coord_sys_to));
}

void Transform::transform(F2CField& _field, const std::string& _coord_sys_to) {
  _field.field = _field.field + _field.ref_point;
  _field.field->transform(
      generateCoordTransf(_field.coord_sys, _coord_sys_to).get());
  _field.coord_sys = _coord_sys_to;
  _field.ref_point = _field.field.getCellBorder(0).StartPoint().clone();
  _field.field = _field.field - _field.ref_point;
}

F2CPath Transform::transform(const F2CPath& _path, const F2CField& _field,
    const std::string& _coord_sys_to) {
  auto new_path = _path.clone();
  auto coords = generateCoordTransf(_field.coord_sys, _coord_sys_to);
  for (auto&& p : new_path.points) {
    p = p + _field.ref_point;
    p->transform(coords.get());
  }
  return new_path;
}

F2CPoint Transform::getRefPointInGPS(const F2CField& _field) {
  auto point = _field.ref_point.clone();
  point->transform(generateCoordTransf(_field.coord_sys, "EPSG:4326").get());
  return point;
}

std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)>
      Transform::createSptRef(const std::string& _coord_sys) {
  auto spt_ref =
    std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)>(
      new OGRSpatialReference(), [](OGRSpatialReference* ref) {
      OGRSpatialReference::DestroySpatialReference(ref);});
  if (_coord_sys.empty()) {
    throw std::invalid_argument("Coordinate system empty");
  } else if (F2CField::isCoordSystemEPSG(_coord_sys)) {
    spt_ref->importFromEPSG(F2CField::getEPSGCoordSystem(_coord_sys));
  } else if (F2CField::isCoordSystemUTM(_coord_sys)) {
    spt_ref->importFromProj4(("+proj=utm +zone=" +
      F2CField::getUTMCoordSystem(_coord_sys) +
      "+datum=etrs89 +units=m").c_str());
  } else  {
    throw std::invalid_argument("Coordinate system not recognized");
  }
  if (GDALVersionInfo("VERSION_NUM")[0] == '3') {
    // Next line solve issue in https://github.com/OSGeo/gdal/issues/1546
    spt_ref->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
  }
  return spt_ref;
}


std::unique_ptr<OGRCoordinateTransformation,
      void(*)(OGRCoordinateTransformation*)> Transform::createCoordTransf(
  std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)> _in,
  std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)> _out) {
    return std::unique_ptr<OGRCoordinateTransformation,
      void(*)(OGRCoordinateTransformation*)>(
        OGRCreateCoordinateTransformation(_in.get(), _out.get()),
          [](OGRCoordinateTransformation* coord) {
            OGRCoordinateTransformation::DestroyCT(coord);});
}


}  // namespace f2c
