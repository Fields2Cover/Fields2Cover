//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
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

  EXPECT_EQ(fields[0].id, "b913fe9d-ed9f-4cc4-aa77-a10652165455");
  EXPECT_EQ(fields[0].coord_sys, "EPSG:4258");
}

TEST(fields2cover_utils_parser, get_area_imported) {
  F2CFields fields;
  f2c::Parser::importGml(std::string(DATA_PATH) + "test1.xml", fields);
  f2c::Transform::transform(fields[0], "EPSG:28992");
  EXPECT_NEAR(fields[0].field.getArea(), 10000.0 * 17.2581, 5.0);
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

  f2c::Transform::transform(field, "UTM:32U");

  EXPECT_EQ(field.id, "test parcel");
  EXPECT_EQ(field.coord_sys, "UTM:32U");

  f2c::Visualizer::figure(200);
  f2c::Visualizer::plot(field);
  f2c::Visualizer::show(false);
  //f2c::Visualizer::show(true);
}

TEST(fields2cover_utils_parser, import_json_two_cells) {
  F2CFields fields;
  f2c::Parser::importJson(std::string(DATA_PATH) + "test_2_cell.json", fields);
  EXPECT_EQ(fields.size(), 2);
  EXPECT_EQ(fields[0].id, "field1");
  EXPECT_EQ(fields[1].id, "field2");
  EXPECT_GT(fields[0].getArea(), 0);
  EXPECT_GT(fields[1].getArea(), 0);
}

TEST(fields2cover_utils_parser, import_json_ring) {
  F2CFields fields;
  f2c::Parser::importJson(std::string(DATA_PATH) + "test_ring.json", fields);
  EXPECT_EQ(fields.size(), 1);
  EXPECT_EQ(fields[0].id, "Ring");
  EXPECT_EQ(fields[0].getArea(), 10*10 - 8*8);
}

