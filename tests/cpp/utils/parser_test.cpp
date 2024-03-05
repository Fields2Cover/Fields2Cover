//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include <gtest/gtest.h>
#include "fields2cover/utils/parser.h"
#include "fields2cover/utils/transformation.h"
#include "fields2cover/utils/visualizer.h"

TEST(fields2cover_utils_parser, import_gml) {
  F2CFields fields;

  f2c::Parser::importGml(std::string(DATA_PATH) + "test1.xml",
      fields);

  EXPECT_EQ(fields[0].getId(), "b913fe9d-ed9f-4cc4-aa77-a10652165455");
  EXPECT_EQ(fields[0].getCRS(), "EPSG:4258");
}

TEST(fields2cover_utils_parser, get_area_imported) {
  F2CFields fields;
  f2c::Parser::importGml(std::string(DATA_PATH) + "test1.xml", fields);
  f2c::Transform::transform(fields[0], "EPSG:28992");
  EXPECT_NEAR(fields[0].area(), 10000.0 * 17.2581, 5.0);
}

TEST(fields2cover_utils_parser, file_not_found) {
  F2CFields fields;
  EXPECT_THROW(f2c::Parser::importGml("file_not_found.xml",
      fields), std::invalid_argument);
}


TEST(fields2cover_utils_parser, import_json) {
  // Need more complex tests
  F2CFields fields;
  f2c::Parser::importJson(std::string(DATA_PATH) + "test.json", fields);
  auto field = fields[0];
  field.setEPSGCoordSystem(4326);

  f2c::Transform::transform(field, "UTM:32N datum:etrs89");

  EXPECT_EQ(field.getId(), "test parcel");
  EXPECT_EQ(field.getCRS(), "UTM:32N datum:etrs89");
}

TEST(fields2cover_utils_parser, import_json_two_cells) {
  F2CFields fields;
  f2c::Parser::importJson(std::string(DATA_PATH) + "test_2_cell.json", fields);
  EXPECT_EQ(fields.size(), 2);
  EXPECT_EQ(fields[0].getId(), "field1");
  EXPECT_EQ(fields[1].getId(), "field2");
  EXPECT_GT(fields[0].area(), 0);
  EXPECT_GT(fields[1].area(), 0);
}

TEST(fields2cover_utils_parser, import_cell_json) {
  F2CCell cell;
  cell = f2c::Parser::importCellJson(std::string(DATA_PATH) + "test.json");
  EXPECT_EQ(cell.size(), 1);
  EXPECT_EQ(cell.getGeometry(0).size(), 20);
  EXPECT_GT(cell.area(), 0);
}

TEST(fields2cover_utils_parser, import_strips_json) {
  F2CStrips strips = f2c::Parser::importStripsJson(std::string(DATA_PATH) + "strips.geojson");
  EXPECT_EQ(strips.size(), 3);
  EXPECT_EQ(strips[0].getName(), "Winter barley");
  EXPECT_EQ(strips[1].getName(), "Potato (late)");
  EXPECT_GT(strips[0].getCell().area(), 0);
  EXPECT_GT(strips[1].getCell().area(), 0);
  EXPECT_NE(strips[0].getCell().area(), strips[1].getCell().area());
}

TEST(fields2cover_utils_parser, import_swaths_json) {
  F2CSwaths swaths = f2c::Parser::importSwathsJson(std::string(DATA_PATH) + "swaths.geojson");
  EXPECT_EQ(swaths.size(), 3);
  EXPECT_EQ(swaths[0].getWidth(), 0.3);
  EXPECT_EQ(swaths[1].getWidth(), 0.3);
  EXPECT_EQ(swaths[0].getId(), 44);
  EXPECT_EQ(swaths[1].getId(), 25);
  EXPECT_GT(swaths[0].length(), 0);
  EXPECT_GT(swaths[1].length(), 0);
  EXPECT_NE(swaths[0].length(), swaths[1].length());
}

TEST(fields2cover_utils_parser, import_json_ring) {
  F2CFields fields;
  f2c::Parser::importJson(std::string(DATA_PATH) + "test_ring.json", fields);
  EXPECT_EQ(fields.size(), 1);
  EXPECT_EQ(fields[0].getId(), "Ring");
  EXPECT_EQ(fields[0].area(), 10*10 - 8*8);
}

