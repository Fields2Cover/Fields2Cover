//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <tinyxml2.h>
#include <boost/optional.hpp>
#include <nlohmann/json.hpp>
#include "fields2cover/utils/parser.h"
#include "fields2cover/utils/transformation.h"

using json = nlohmann::json;

namespace f2c {

int Parser::importGml(const std::string& _file, F2CFields& _fields) {
  // Tinyxml2 depends on locale when parsing files. It may expect float numbers
  // as "1,5" instead of "1.5", which is parsed as "1". The next line solves
  // the issue.
  std::locale::global(std::locale::classic());

  tinyxml2::XMLDocument doc;
  doc.LoadFile(_file.c_str());
  auto* p_parcel = doc.RootElement();

  if (p_parcel == nullptr) {
    throw std::invalid_argument("File not found");
    return -1;
  }

  std::string id {""};

  auto e_result = p_parcel->Attribute("id");
  if (e_result != nullptr) {
    id = e_result;
  }

  auto* p_field = p_parcel->FirstChildElement("Field");

  auto* p_polygon = p_field\
            ->FirstChildElement("geometry")\
            ->FirstChildElement("gml:Polygon");

  std::string coord_sys = p_polygon->Attribute("srsName");

  std::string p_coords = std::string(
            p_polygon->FirstChildElement("gml:outerBoundaryIs")\
            ->FirstChildElement("gml:LinearRing")\
            ->FirstChildElement("gml:coordinates")\
            ->GetText());
  auto findAndReplaceAll =
      [](std::string& data, std::string toSearch, std::string replaceStr) {
            size_t pos = data.find(toSearch);
            while (pos != std::string::npos) {
                data.replace(pos, toSearch.size(), replaceStr);
                pos = data.find(toSearch, pos + replaceStr.size());
            }
            return;
      };


  findAndReplaceAll(p_coords, ",", ";");
  findAndReplaceAll(p_coords, " ", ", ");
  findAndReplaceAll(p_coords, ";", " ");
  p_coords = "POLYGON ((" + p_coords + "))";
  OGRGeometry* new_geom{};
  auto spt_ref = Transform::createSptRef(coord_sys);
  OGRGeometryFactory::createFromWkt(p_coords.c_str(), spt_ref.get(), &new_geom);

  F2CField field(F2CCells(new_geom), id);
  field.coord_sys = coord_sys;
  OGRGeometryFactory::destroyGeometry(new_geom);

  _fields.emplace_back(field);
  return 0;
}

int Parser::importJson(const std::string& _file, F2CFields& _fields) {
  std::ifstream f(_file);
  json imported_field = json::parse(f);

  F2CLinearRing line;
  F2CCell poly;
  for (auto&& ps :
      imported_field["features"][0]["geometry"]["coordinates"][0]) {
    F2CPoint point(ps[0], ps[1], ps[2]);
    line.addPoint(point);
  }
  poly.addRing(line);
  F2CField field(F2CCells(poly),
      imported_field["features"][0]["properties"]["Name"]);

  _fields.emplace_back(field);
  return 0;
}

}  // namespace f2c
