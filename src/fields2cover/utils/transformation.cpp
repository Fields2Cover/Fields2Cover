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
        const std::string& coord_sys_from, const std::string& coord_sys_to) {
  return createCoordTransf(
      createSptRef(coord_sys_from), createSptRef(coord_sys_to));
}

void Transform::transform(F2CField& field, const std::string& coord_sys_to) {
  field.field = field.field + field.ref_point;
  field.field->transform(
      generateCoordTransf(field.coord_sys, coord_sys_to).get());
  field.coord_sys = coord_sys_to;
  field.ref_point = field.field.getCellBorder(0).StartPoint().clone();
  field.field = field.field - field.ref_point;
}

F2CPath Transform::transform(const F2CPath& path, const F2CField& field,
    const std::string& coord_sys_to) {
  auto new_path = path.clone();
  auto coords = generateCoordTransf(field.coord_sys, coord_sys_to);
  for (auto&& p : new_path.points) {
    p = p + field.ref_point;
    p->transform(coords.get());
  }
  return new_path;
}

F2CPoint Transform::getRefPointInGPS(const F2CField& field) {
  auto point = field.ref_point.clone();
  point->transform(generateCoordTransf(field.coord_sys, "EPSG:4326").get());
  return point;
}

std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)>
      Transform::createSptRef(const std::string& coord_sys) {
  auto spt_ref =
    std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)>(
      new OGRSpatialReference(), [](OGRSpatialReference* ref) {
      OGRSpatialReference::DestroySpatialReference(ref);});
  if (coord_sys.empty()) {
    throw std::invalid_argument("Coordinate system empty");
  } else if (F2CField::isCoordSystemEPSG(coord_sys)) {
    spt_ref->importFromEPSG(F2CField::getEPSGCoordSystem(coord_sys));
  } else if (F2CField::isCoordSystemUTM(coord_sys)) {
    spt_ref->importFromProj4(("+proj=utm +zone=" +
      F2CField::getUTMCoordSystem(coord_sys) +
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
  std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)> in,
  std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)> out) {
    return std::unique_ptr<OGRCoordinateTransformation,
      void(*)(OGRCoordinateTransformation*)>(
        OGRCreateCoordinateTransformation(in.get(), out.get()),
          [](OGRCoordinateTransformation* coord) {
            OGRCoordinateTransformation::DestroyCT(coord);});
}


}  // namespace f2c
