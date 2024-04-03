//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/utils/transformation.h"
#include <boost/algorithm/string/predicate.hpp>

namespace f2c {

std::unique_ptr<OGRCoordinateTransformation,
  void(*)(OGRCoordinateTransformation*)>
    Transform::generateCoordTransf(
        const std::string& coord_sys_from, const std::string& coord_sys_to) {
  return createCoordTransf(
      createSptRef(coord_sys_from), createSptRef(coord_sys_to));
}

void Transform::transform(F2CField& field, const std::string& coord_sys_to) {
  if (!field.isEmpty()) {
    field.setField(field.getField() + field.getRefPoint());
    field.getField()->transform(
        generateCoordTransf(field.getCRS(), coord_sys_to).get());
    field.setRefPoint(field.getField().getCellBorder(0).startPoint());
    field.setField(field.getField() - field.getRefPoint());
  } else {
    field.setRefPoint( \
        transform(field.getRefPoint(), field.getCRS(), coord_sys_to));
  }
  field.setPrevCRS(field.getCRS());
  field.setCRS(coord_sys_to);
}

F2CRoute Transform::transformRouteWithFieldRef(const F2CRoute& route,
      const F2CField& field, const std::string& coord_sys_to) {
  auto new_route = route.clone();
  auto coords = generateCoordTransf(field.getCRS(), coord_sys_to);
  for (size_t i = 0; i < new_route.sizeVectorSwaths(); ++i) {
    new_route.setSwaths(i, transformSwathsWithFieldRef(
          new_route.getSwaths(i), field, coord_sys_to));
  }
  for (size_t i = 0; i < new_route.sizeConnections(); ++i) {
    new_route.setConnection(i, transformMultiPointWithFieldRef(
          new_route.getConnection(i), field, coord_sys_to));
  }
  return new_route;
}

F2CPath Transform::transformPathWithFieldRef(const F2CPath& path,
      const F2CField& field, const std::string& coord_sys_to) {
  if (path.size() < 1) {return path;}

  auto new_path = path;
  F2CPathState s_end = path.back();
  if (s_end.len > 0.0) {
    s_end.point = path.atEnd();
    s_end.len = 0.0;
    new_path.addState(s_end);
  }

  auto coords = generateCoordTransf(field.getCRS(), coord_sys_to);
  for (auto&& s : new_path) {
    s.point = s.point + field.getRefPoint();
    s.point->transform(coords.get());
  }
  for (size_t i = 0; i < new_path.size() - 1; ++i) {
    new_path[i].len = new_path[i].point.distance(new_path[i+1].point);
    new_path[i].angle = (new_path[i+1].point - new_path[i].point).getAngleFromPoint();
  }


  return new_path;
}

F2CMultiPoint Transform::transformMultiPointWithFieldRef(const F2CMultiPoint& mp,
      const F2CField& field, const std::string& coord_sys_to) {
  F2CMultiPoint new_mp;
  auto coords = generateCoordTransf(field.getCRS(), coord_sys_to);
  for (auto&& p : mp) {
    F2CPoint abs_p = p + field.getRefPoint();
    abs_p->transform(coords.get());
    new_mp.addGeometry(abs_p);
  }
  return new_mp;
}

F2CSwath Transform::transformSwathWithFieldRef(const F2CSwath& swath,
      const F2CField& field, const std::string& coord_sys_to) {
  return F2CSwath(transform(
        swath.getPath(), field.getRefPoint(), field.getCRS(), coord_sys_to),
      swath.getWidth(), swath.getId());
}

F2CSwaths Transform::transformSwathsWithFieldRef(const F2CSwaths& swaths,
      const F2CField& field, const std::string& coord_sys_to) {
  F2CSwaths new_swaths;
  for (auto&& swath : swaths) {
    new_swaths.emplace_back(
        transformSwathWithFieldRef(swath, field, coord_sys_to));
  }
  return new_swaths;
}

F2CStrip Transform::transformStrip(const F2CStrip& strip,
      const std::string& coord_sys_from, const std::string& coord_sys_to) {
  F2CStrip new_strip;
  new_strip.setName(strip.getName());
  new_strip.setCell(transform(strip.getCell(), coord_sys_from, coord_sys_to));
  return new_strip;
}

F2CStrips Transform::transformStrips(const F2CStrips& strips,
      const std::string& coord_sys_from, const std::string& coord_sys_to) {
  F2CStrips new_strips;
  for (auto&& strip : strips) {
    new_strips.emplace_back(
        transformStrip(strip, coord_sys_from, coord_sys_to));
  }
  return new_strips;
}

F2CSwaths Transform::transformSwaths(const F2CSwaths& swaths,
      const std::string& coord_sys_from, const std::string& coord_sys_to) {
  F2CSwaths new_swaths;
  for (auto&& swath : swaths) {
    new_swaths.emplace_back(
        transformSwath(swath, coord_sys_from, coord_sys_to));
  }
  return new_swaths;
}

F2CSwath Transform::transformSwath(const F2CSwath& swath,
      const std::string& coord_sys_from, const std::string& coord_sys_to) {
  return F2CSwath(transform(swath.getPath(), coord_sys_from, coord_sys_to),
                   swath.getWidth(), swath.getId());
}

F2CPath Transform::transformPath(const F2CPath& path,
      const std::string& coord_sys_from, const std::string& coord_sys_to) {
  auto new_path = path;
  auto coords = generateCoordTransf(coord_sys_from, coord_sys_to);
  for (auto&& s : new_path) {
    s.point->transform(coords.get());
  }
  return new_path;
}

void Transform::transformToUTM(F2CField& field, bool is_etrs89_opt) {
  std::string field_crs = field.getCRS();
  if (!field_crs.empty() && \
      field_crs != "EPSG:4258" && field_crs != "EPSG:4326") {
    transform(field, "EPSG:4326");
    if (is_etrs89_opt && \
        (field.getRefPoint().getX() <  32.88 && \
         field.getRefPoint().getX() > -16.1 && \
         field.getRefPoint().getY() <  84.73 && \
         field.getRefPoint().getY() >  40.18) ) {
      transform(field, "EPSG:4258");
    }
  } else if (field_crs.empty()) {
    if (field.getRefPoint() == F2CPoint(0, 0)) {
      field = field.clone();  // Move field first point to (0, 0)
    }
    if (field.getRefPoint() == F2CPoint(0, 0)) {
      throw std::out_of_range(
        "Error on transformToUTM: Coordinates are in a local system");
    }
    if (is_etrs89_opt && \
        (field.getRefPoint().getX() <  32.88 && \
         field.getRefPoint().getX() > -16.1 && \
         field.getRefPoint().getY() <  84.73 && \
         field.getRefPoint().getY() >  40.18) ) {
      field.setCRS("EPSG:4258");
    } else if (field.getRefPoint().getX() <  180. && \
               field.getRefPoint().getX() > -180. && \
               field.getRefPoint().getY() <   90. && \
               field.getRefPoint().getY() >  -90.) {
      field.setCRS("EPSG:4326");
    } else {
      throw std::invalid_argument(
        "Error on transformToUTM: Coordinates local system was not recognized");
    }
    field_crs = field.getCRS();
  }

  std::string utm_zone = std::string("UTM:") + \
      std::to_string(static_cast<size_t>(
            floor((field.getRefPoint().getX() + 180) / 6) + 1)) + \
      (field.getRefPoint().getY() > 0 ? "N" : "S") + \
      " datum:" + (field.getCRS() == "EPSG:4258" ? "ETRS89" : "WGS84");
  transform(field, utm_zone);
  field.setPrevCRS(field_crs);
}


void Transform::transformToPrevCRS(F2CField& field) {
  if (field.getPrevCRS().empty()) {
    throw std::invalid_argument(
        "Error in Transform::transformToUTM. No previous CRS recorded.");
  }
  transform(field, field.getPrevCRS());
}

F2CRoute Transform::transformToPrevCRS(
    const F2CRoute& route, const F2CField& field) {
  return transformRouteWithFieldRef(route, field, field.getPrevCRS());
}

F2CPath Transform::transformToPrevCRS(
    const F2CPath& path, const F2CField& field) {
  return transformPathWithFieldRef(path, field, field.getPrevCRS());
}

F2CStrip Transform::transformToPrevCRS(
    const F2CStrip& s, const F2CField& field) {
  return transformStrip(s, field.getCRS(), field.getPrevCRS());
}

F2CStrips Transform::transformToPrevCRS(
    const F2CStrips& s, const F2CField& field) {
  return transformStrips(s, field.getCRS(), field.getPrevCRS());
}

F2CSwath Transform::transformToPrevCRS(
    const F2CSwath& s, const F2CField& field) {
  return transformSwathWithFieldRef(s, field, field.getPrevCRS());
}

F2CSwaths Transform::transformToPrevCRS(
    const F2CSwaths& s, const F2CField& field) {
  return transformSwathsWithFieldRef(s, field, field.getPrevCRS());
}

F2CPoint Transform::getRefPointInGPS(const F2CField& field) {
  auto point = field.getRefPoint();
  point->transform(generateCoordTransf(field.getCRS(), "EPSG:4326").get());
  return point;
}

std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)>
      Transform::createSptRef(
          const std::string& coord_sys, bool fail_silently) {
  auto spt_ref =
    std::unique_ptr<OGRSpatialReference, void(*)(OGRSpatialReference*)>(
      new OGRSpatialReference(), [](OGRSpatialReference* ref) {
      OGRSpatialReference::DestroySpatialReference(ref);});
  if (coord_sys.empty() && !fail_silently) {
    throw std::invalid_argument("Coordinate system empty");
  } else if (F2CField::isCoordSystemEPSG(coord_sys)) {
    spt_ref->importFromEPSG(F2CField::getEPSGCoordSystem(coord_sys));
  } else if (F2CField::isCoordSystemUTM(coord_sys)) {
    std::string proj = (
      std::string("+proj=utm +zone=") + F2CField::getUTMZone(coord_sys) + " " +
      F2CField::getUTMHemisphere(coord_sys) +
      (boost::iequals(F2CField::getUTMDatum(coord_sys), "ETRS89") ?
        " +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +type=crs" :
        " +datum=" + F2CField::getUTMDatum(coord_sys))
      + " +units=m +no_defs ");
    spt_ref->importFromProj4(proj.c_str());
  } else if (!fail_silently) {
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
