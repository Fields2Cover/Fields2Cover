//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include <gtest/gtest.h>
#include "fields2cover/utils/parser.h"
#include "fields2cover/utils/transformation.h"

#ifdef F2C_DEF_MATPLOTLIB
#include "fields2cover/utils/visualizer.h"
#endif  // F2C_DEF_MATPLOTLIB

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

  #ifdef F2C_DEF_MATPLOTLIB
    f2c::Visualizer::figure(200);
    f2c::Visualizer::plot(field);
    f2c::Visualizer::show(false);
    //f2c::Visualizer::show(true);
  #endif  // F2C_DEF_MATPLOTLIB
}

