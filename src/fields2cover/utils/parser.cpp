//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/utils/parser.h"
#include "fields2cover/utils/transformation.h"
#include <boost/optional.hpp>
#include <nlohmann/json.hpp>
#include <tinyxml2.h>

using json = nlohmann::json;

namespace f2c
{

    void Parser::importGml(const std::string &file, F2CFields &fields) { fields.emplace_back(importFieldGml(file)); }

    F2CField Parser::importFieldGml(const std::string &file, bool coord_frame_fail_silently)
    {
        // Tinyxml2 depends on locale when parsing files. It may expect float numbers
        // as "1,5" instead of "1.5", which is parsed as "1". The next line solves
        // the issue.
        std::locale::global(std::locale::classic());

        tinyxml2::XMLDocument doc;
        doc.LoadFile(file.c_str());
        auto *p_parcel = doc.RootElement();

        if (p_parcel == nullptr)
        {
            throw std::invalid_argument("File not found");
        }

        std::string id{""};

        auto e_result = p_parcel->Attribute("id");
        if (e_result != nullptr)
        {
            id = e_result;
        }

        auto *p_field = p_parcel->FirstChildElement("Field");

        auto *p_polygon = p_field->FirstChildElement("geometry")->FirstChildElement("gml:Polygon");

        std::string coord_sys = p_polygon->Attribute("srsName");

        std::string p_coords = std::string(
            p_polygon->FirstChildElement("gml:outerBoundaryIs")->FirstChildElement("gml:LinearRing")->FirstChildElement("gml:coordinates")->GetText());
        auto findAndReplaceAll = [](std::string &data, std::string toSearch, std::string replaceStr)
        {
            size_t pos = data.find(toSearch);
            while (pos != std::string::npos)
            {
                data.replace(pos, toSearch.size(), replaceStr);
                pos = data.find(toSearch, pos + replaceStr.size());
            }
            return;
        };
        findAndReplaceAll(p_coords, ",", ";");
        findAndReplaceAll(p_coords, " ", ", ");
        findAndReplaceAll(p_coords, ";", " ");
        p_coords = "POLYGON ((" + p_coords + "))";
        OGRGeometry *new_geom{};
        auto spt_ref = Transform::createSptRef(coord_sys, coord_frame_fail_silently);
        OGRGeometryFactory::createFromWkt(p_coords.c_str(), spt_ref.get(), &new_geom);

        F2CField field(F2CCells(new_geom), id);
        field.setCRS(coord_sys);
        OGRGeometryFactory::destroyGeometry(new_geom);

        return field;
    }

    F2CPoint getPointFromJson(const json &ps)
    {
        if (ps.size() == 3)
        {
            return F2CPoint(ps[0], ps[1], ps[2]);
        }
        else if (ps.size() == 2)
        {
            return F2CPoint(ps[0], ps[1]);
        }
        return F2CPoint();
    }

    F2CCell getCellFromJson(const json &imported_cell)
    {
        F2CLinearRing outer_ring;
        for (auto &&ps : imported_cell["geometry"]["coordinates"][0])
        {
            outer_ring.addPoint(getPointFromJson(ps));
        }
        F2CCell cell(outer_ring);
        for (int i = 1; i < imported_cell["geometry"]["coordinates"].size(); ++i)
        {
            F2CLinearRing inner_ring;
            for (auto &&ps : imported_cell["geometry"]["coordinates"][i])
            {
                inner_ring.addPoint(getPointFromJson(ps));
            }
            cell.addRing(inner_ring);
        }
        return cell;
    }

    std::string gen_random(const int len)
    {
        static const char alphanum[] = "0123456789"
                                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                       "abcdefghijklmnopqrstuvwxyz";
        std::string tmp_s;
        tmp_s.reserve(len);

        for (int i = 0; i < len; ++i)
        {
            tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
        }

        return tmp_s;
    }

    int importJsonData(json imported_field, F2CFields &fields)
    {
        auto name = imported_field["properties"].contains("Name") ? imported_field["properties"]["Name"].get<std::string>() : "cell " + gen_random(3);
        for (auto &&imported_cell : imported_field["features"])
        {
            fields.emplace_back(F2CField(F2CCells(getCellFromJson(imported_cell)), name));
        }
        return 0;
    }

    int Parser::importJson(const std::string &file, F2CFields &fields)
    {
        std::ifstream f(file);
        json imported_field = json::parse(f);

        return importJsonData(imported_field, fields);
    }

    int Parser::importJsonFromString(const std::string &data, F2CFields &fields)
    {
        json imported_field = json::parse(data);

        return importJsonData(imported_field, fields);
    }

    F2CCell Parser::importCellJson(const std::string &file)
    {
        std::ifstream f(file);
        json imported_field = json::parse(f);
        return getCellFromJson(imported_field["features"][0]);
    }

    F2CCell Parser::importCellJsonFromString(const std::string &jsonString)
    {
        json imported_field = json::parse(jsonString);
        return getCellFromJson(imported_field["features"][0]);
    }

    F2CSwaths importSwathsJsonData(const json &data)
    {
        F2CSwaths swaths;
        for (auto &&imported_swath : data["features"])
        {
            F2CLineString line;
            for (auto &&ps : imported_swath["geometry"]["coordinates"])
            {
                line.addPoint(getPointFromJson(ps));
            }
            swaths.emplace_back(line, imported_swath["properties"]["width"], imported_swath["properties"]["path_id"]);
        }
        return swaths;
    }

    F2CSwaths Parser::importSwathsJson(const std::string &file)
    {
        std::ifstream f(file);
        json imported_swaths = json::parse(f);
        return importSwathsJsonData(imported_swaths);
    }

    F2CSwaths Parser::importSwathsJsonFromString(const std::string &jsonString)
    {
        json imported_swaths = json::parse(jsonString);
        F2CSwaths swaths;
        return importSwathsJsonData(imported_swaths);
    }

    F2CStrips importStripsJsonData(const json &data)
    {
        F2CStrips strips;
        for (auto &&imported_strip : data["features"])
        {
            F2CStrip strip;
            strip.setName(imported_strip["properties"]["crop_id"]);
            strip.setCell(getCellFromJson(imported_strip));
            strips.emplace_back(strip);
        }
        return strips;
    }

    F2CStrips Parser::importStripsJson(const std::string &file)
    {
        std::ifstream f(file);
        json imported_strips = json::parse(f);
        return importStripsJsonData(imported_strips);
    }

    F2CStrips Parser::importStripsJsonFromString(const std::string &jsonString)
    {
        json imported_strips = json::parse(jsonString);
        return importStripsJsonData(imported_strips);
    }

} // namespace f2c
