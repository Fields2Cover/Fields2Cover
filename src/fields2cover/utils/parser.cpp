//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <tinyxml2.h>
#include <filesystem>
#include <boost/optional.hpp>
#include <nlohmann/json.hpp>
#include "fields2cover/utils/parser.h"
#include "fields2cover/utils/transformation.h"

using json = nlohmann::json;

namespace f2c {

void Parser::importGml(const std::string& file, F2CFields& fields) {
  fields.emplace_back(importFieldGml(file));
}

F2CField Parser::importFieldGml(
    const std::string& file, bool coord_frame_fail_silently) {
  // Tinyxml2 depends on locale when parsing files. It may expect float numbers
  // as "1,5" instead of "1.5", which is parsed as "1". The next line solves
  // the issue.
  std::locale::global(std::locale::classic());

  std::string file_type = "GAOS";
  std::vector<std::string> tags {
    "id",
    "Field",
    "",
    "geometry",
    "gml:Polygon",
    "srsName",
    "gml:outerBoundaryIs",
    "gml:LinearRing",
    "gml:coordinates",
  };

  tinyxml2::XMLDocument doc;
  doc.LoadFile(file.c_str());
  auto* p_parcel = doc.RootElement();

  if (p_parcel == nullptr) {
    throw std::invalid_argument("File not found");
  }

  if (std::string(p_parcel->Name()).compare("wfs:FeatureCollection") == 0) {
    file_type = "wfs";
    tags[0] = "gml:id";
    tags[1] = "gml:featureMember";
    tags[2] = "referentiepercelen:referentiepercelen";
    tags[3] = "referentiepercelen:geom";
    tags[6] = "gml:exterior";
    tags[7] = "gml:LinearRing";
    tags[8] = "gml:posList";
  }

  std::string id {""};

  auto e_result = p_parcel->Attribute(tags[0].c_str());
  if (e_result != nullptr) {
    id = e_result;
  }

  F2CCells cells;

  std::string coord_sys;
  for (auto* p_field = p_parcel->FirstChildElement(tags[1].c_str());
      p_field != nullptr;
      p_field = p_field->NextSiblingElement(tags[1].c_str())) {
    auto* p_field2 = p_field;
    if (!tags[2].empty()) {
      p_field2 = p_field->FirstChildElement(tags[2].c_str());
    }
    auto* p_polygon = p_field2->FirstChildElement(tags[3].c_str())
                             ->FirstChildElement(tags[4].c_str());
    coord_sys = p_polygon->Attribute(tags[5].c_str());

    auto* p_coords = p_polygon->FirstChildElement(tags[6].c_str())
                              ->FirstChildElement(tags[7].c_str())
                              ->FirstChildElement(tags[8].c_str());
    std::string s_coords = std::string(p_coords->GetText());
    auto findAndReplaceAll = [](
        std::string& data, std::string toSearch, std::string replaceStr) {
      size_t pos = data.find(toSearch);
      while (pos != std::string::npos) {
        data.replace(pos, toSearch.size(), replaceStr);
        pos = data.find(toSearch, pos + replaceStr.size());
      }
      return;
    };
    auto replaceNthSpace = [] (std::string& data, int n) {
      int s = 0;
      for (size_t i = 0; i < data.length()-1; ++i) {
        if (data[i] == ' ') {
          ++s;
          if (s % n == 0) {
            data[i] = ',';
          }
        }
      }
      return;
    };
    if (file_type.compare("GAOS") == 0) {
      findAndReplaceAll(s_coords, ",", ";");
      findAndReplaceAll(s_coords, " ", ", ");
      findAndReplaceAll(s_coords, ";", " ");
    } else if (file_type.compare("wfs") == 0) {
      replaceNthSpace(s_coords, 2);
    }
    s_coords = "POLYGON ((" + s_coords + "))";

    OGRGeometry* new_geom{};
    auto spt_ref = Transform::createSptRef(
        coord_sys, coord_frame_fail_silently);
    OGRGeometryFactory::createFromWkt(
        s_coords.c_str(), spt_ref.get(), &new_geom);
    F2CCell cell(new_geom);
    cells.addGeometry(cell.clone());
    OGRGeometryFactory::destroyGeometry(new_geom);
  }

  F2CField field(cells, id);
  field.setCRS(coord_sys);

  return field;
}


F2CPoint getPointFromJson(const json& ps) {
  if (ps.size() == 3) {
    return F2CPoint(ps[0], ps[1], ps[2]);
  } else if (ps.size() == 2) {
    return F2CPoint(ps[0], ps[1]);
  }
  return F2CPoint();
}

F2CCell getCellFromJson(const json& imported_cell) {
  F2CLinearRing outer_ring;
  for (auto&& ps : imported_cell["geometry"]["coordinates"][0]) {
    outer_ring.addPoint(getPointFromJson(ps));
  }
  F2CCell cell(outer_ring);
  for (int i = 1; i < imported_cell["geometry"]["coordinates"].size(); ++i) {
    F2CLinearRing inner_ring;
    for (auto&& ps : imported_cell["geometry"]["coordinates"][i]) {
      inner_ring.addPoint(getPointFromJson(ps));
    }
    cell.addRing(inner_ring);
  }
  return cell;
}


int Parser::importJson(const std::string& file, F2CFields& fields) {
  std::ifstream f(file);
  json imported_field = json::parse(f);

  for (auto&& imported_cell : imported_field["features"]) {
    fields.emplace_back(F2CCells(getCellFromJson(imported_cell)),
        imported_cell["properties"]["Name"]);
  }
  return 0;
}

F2CCell Parser::importCellJson(const std::string& file) {
  std::ifstream f(file);
  json imported_field = json::parse(f);
  return getCellFromJson(imported_field["features"][0]);
}

F2CSwaths Parser::importSwathsJson(const std::string& file) {
  std::ifstream f(file);
  json imported_swaths = json::parse(f);
  F2CSwaths swaths;
  for (auto&& imported_swath : imported_swaths["features"]) {
    F2CLineString line;
    for (auto&& ps : imported_swath["geometry"]["coordinates"]) {
      line.addPoint(getPointFromJson(ps));
    }
    swaths.emplace_back(line,
      imported_swath["properties"]["width"],
      imported_swath["properties"]["path_id"]);
  }
  return swaths;
}

F2CStrips Parser::importStripsJson(const std::string& file) {
  std::ifstream f(file);
  json imported_strips = json::parse(f);
  F2CStrips strips;
  for (auto&& imported_strip : imported_strips["features"]) {
    F2CStrip strip;
    strip.setName(imported_strip["properties"]["crop_id"]);
    strip.setCell(getCellFromJson(imported_strip));
    strips.emplace_back(strip);
  }
  return strips;
}

F2CCell Parser::importCellWkt(const std::string& file) {
  std::ifstream f(file);
  if (!f) {return {};}

  std::ostringstream ss;
  ss << f.rdbuf();
  std::string data = ss.str();

  F2CCell cell;
  cell.importFromWkt(data);
  return cell;
}

F2CFields Parser::importDatasetWkt(const std::string& folder) {
  F2CFields fields;
  for (const auto& file : std::filesystem::directory_iterator(folder)) {
    if (file.is_regular_file() && file.path().extension() == ".wkt") {
      std::string file_name = file.path().filename();
      std::string file_dir = std::filesystem::absolute(file.path()).string();
      fields.emplace_back(F2CCells(importCellWkt(file_dir)), file_name);
      fields.back().setId(file_name.substr(0, file_name.size()-4));
    }
  }
  return fields;
}


}  // namespace f2c
