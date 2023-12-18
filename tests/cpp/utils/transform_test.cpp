//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <iostream>
#include <gtest/gtest.h>
#include "fields2cover/utils/transformation.h"
#include "fields2cover.h"

TEST(fields2cover_utils_transformer, convert_to_F2C_and_return) {
  F2CCell poly(F2CLinearRing({
    F2CPoint(6.062131843297665, 51.51238564279176, 0),
    F2CPoint(6.062215149507296, 51.51204470468504, 0),
    F2CPoint(6.062232423018175, 51.51197104647852, 0),
    F2CPoint(6.062259746209906, 51.51187606814959, 0),
    F2CPoint(6.062131843297665, 51.51238564279176, 0),
  }));
  auto poly_to_transform = poly;
  EXPECT_TRUE(poly == poly_to_transform);
  F2CField field {F2CCells(poly_to_transform)};
  field.setEPSGCoordSystem(4326);
  f2c::Transform::transform(field, "EPSG:28992");
  EXPECT_EQ(field.field.getCellBorder(0).StartPoint(), F2CPoint(0, 0, 0));
  EXPECT_EQ(field.coord_sys, "EPSG:28992");

  auto ref_gps_point = f2c::Transform::getRefPointInGPS(field);
  EXPECT_NEAR(ref_gps_point.getX(), 6.062131843297665, 1e-3);
  EXPECT_NEAR(ref_gps_point.getY(), 51.51238564279176, 1e-3);
  EXPECT_NEAR(ref_gps_point.getZ(), 0, 1e-3);

  f2c::Transform::transform(field, "EPSG:4326");
  auto final_field = (field.field + field.ref_point).getCellBorder(0);
  for (int i = 0; i < final_field.size(); ++i) {
    EXPECT_NEAR(final_field.getGeometry(i).getX(),
        poly.getGeometry(0).getGeometry(i).getX(), 1e-3);
    EXPECT_NEAR(final_field.getGeometry(i).getY(),
        poly.getGeometry(0).getGeometry(i).getY(), 1e-3);
    EXPECT_NEAR(final_field.getGeometry(i).getZ(),
        poly.getGeometry(0).getGeometry(i).getZ(), 1e-3);
  }

  EXPECT_NO_THROW(f2c::Transform::transform(field, "UTM:32N datum:WGS84"));
  EXPECT_THROW( f2c::Transform::transform(field, "NONE"), std::invalid_argument);
  field.coord_sys = "";
  EXPECT_THROW( f2c::Transform::transform(field, "EPSG:28992"), std::invalid_argument);
}

TEST(fields2cover_utils_transformer, convert_to_UTM_and_return) {
  F2CCell poly(F2CLinearRing({
    F2CPoint(6.062131843297665, 51.51238564279176, 0),
    F2CPoint(6.062215149507296, 51.51204470468504, 0),
    F2CPoint(6.062232423018175, 51.51197104647852, 0),
    F2CPoint(6.062259746209906, 51.51187606814959, 0),
    F2CPoint(6.062131843297665, 51.51238564279176, 0),
  }));
  auto poly_to_transform = poly;
  EXPECT_TRUE(poly == poly_to_transform);
  F2CField field {F2CCells(poly_to_transform)};
  field.setEPSGCoordSystem(4326);
  f2c::Transform::transformToUTM(field);
  EXPECT_EQ(field.getField().getCellBorder(0).StartPoint(), F2CPoint(0, 0, 0));
  EXPECT_EQ(field.getCRS(), "UTM:32N datum:WGS84");
  EXPECT_EQ(field.getPrevCRS(), "EPSG:4326");

  f2c::Transform::transformToPrevCRS(field);
  auto final_field = (field.field + field.ref_point).getCellBorder(0);
  for (int i = 0; i < final_field.size(); ++i) {
    EXPECT_NEAR(final_field.getGeometry(i).getX(),
        poly.getGeometry(0).getGeometry(i).getX(), 1e-3);
    EXPECT_NEAR(final_field.getGeometry(i).getY(),
        poly.getGeometry(0).getGeometry(i).getY(), 1e-3);
    EXPECT_NEAR(final_field.getGeometry(i).getZ(),
        poly.getGeometry(0).getGeometry(i).getZ(), 1e-3);
  }

  field.setCRS("");
  EXPECT_LT(field.getArea(), 1e-5);
  EXPECT_GT(field.getArea(), 0);
  f2c::Transform::transformToUTM(field, false);
  EXPECT_EQ(field.getCRS(), "UTM:32N datum:WGS84");
  EXPECT_NEAR(field.getArea(), 5.9, 0.005);
  f2c::Transform::transformToPrevCRS(field);
  EXPECT_LT(field.getArea(), 1e-5);
  EXPECT_GT(field.getArea(), 0);

  field.setCRS("");
  f2c::Transform::transformToUTM(field, true);
  EXPECT_EQ(field.getCRS(), "UTM:32N datum:ETRS89");
  EXPECT_NEAR(field.getArea(), 5.9, 0.005);
  f2c::Transform::transformToPrevCRS(field);
  EXPECT_LT(field.getArea(), 1e-5);
  EXPECT_GT(field.getArea(), 0);


  field.setCRS("EPSG:4258");
  f2c::Transform::transformToUTM(field);
  EXPECT_EQ(field.getCRS(), "UTM:32N datum:ETRS89");
  EXPECT_NEAR(field.getArea(), 5.9, 0.005);
  f2c::Transform::transformToPrevCRS(field);
  EXPECT_LT(field.getArea(), 1e-5);
  EXPECT_GT(field.getArea(), 0);

  field.setCRS("ERROR");
  EXPECT_THROW( f2c::Transform::transformToUTM(field), std::invalid_argument);
  field.setCRS("");
  EXPECT_THROW( f2c::Transform::transformToPrevCRS(field), std::invalid_argument);
}

TEST(fields2cover_utils_transformer, convert_types) {
  F2CLineString line({
    F2CPoint(6.062131843297665, 51.51238564279176, 0),
    F2CPoint(6.062215149507296, 51.51204470468504, 0)});
  F2CSwath swath{line, 30, 1};
  F2CSwaths swaths;
  swaths.push_back(swath);

  auto moved_swaths = f2c::Transform::transformSwaths(swaths, "EPSG:4326", "EPSG:28992");

  F2CStrip strip;
  strip.cell = F2CCell::Buffer(moved_swaths[0].getPath(), 15);
  F2CStrips strips;
  strips.emplace_back(strip);


  auto cells = moved_swaths.at(0).computeAreaCovered();
  F2CField field(cells, "field");
  field.setEPSGCoordSystem(28992);
  field.setPrevCRS("EPSG:4326");
  EXPECT_NE(field.ref_point.getX(), 6.062131843297665);
  EXPECT_NE(field.ref_point.getY(), 51.51238564279176);
  EXPECT_GT(field.ref_point.getX(), 1e4);
  EXPECT_GT(field.ref_point.getY(), 1e4);
  EXPECT_NEAR(field.getArea(), 1150, 10);
  EXPECT_NEAR(strip.cell.getArea(), 1150, 10);

  auto moved_strips = f2c::Transform::transformStrips(strips, "EPSG:28992", "EPSG:4326");
  auto moved_cells = f2c::Transform::transform(cells, "EPSG:28992", "EPSG:4326");
  EXPECT_NEAR(moved_strips[0].cell.getArea(), moved_cells.getArea(), 1e-7);

  F2CPath path;
  path.appendSwath(moved_swaths.at(0), 10);
  F2CPath path2;
  F2CSwath swath2{F2CLineString({F2CPoint(0,0), F2CPoint(1,1)}), 40, 2};
  path2.appendSwath(swath2, 10);


  auto returned_path = f2c::Transform::transformPath(path, "EPSG:28992", "EPSG:4326");
  EXPECT_NEAR(returned_path.states[0].point.getX(), 6.062131843297665, 1e-3);
  EXPECT_NEAR(returned_path.states[0].point.getY(), 51.51238564279176, 1e-3);
  auto returned_path2 = f2c::Transform::transformPathWithFieldRef(path2, field, "EPSG:4326");
  EXPECT_NEAR(returned_path2.states[0].point.getX(), 6.062131843297665, 1e-3);
  EXPECT_NEAR(returned_path2.states[0].point.getY(), 51.51238564279176, 1e-3);
  auto returned_path3 = f2c::Transform::transformToPrevCRS(path2, field);
  EXPECT_NEAR(returned_path3.states[0].point.getX(), 6.062131843297665, 1e-3);
  EXPECT_NEAR(returned_path3.states[0].point.getY(), 51.51238564279176, 1e-3);

  auto returned_swaths =
    f2c::Transform::transformSwathsWithFieldRef(moved_swaths, field, "EPSG:4326");
  EXPECT_NEAR(returned_path2.states[0].point.getX(), 6.062131843297665, 1e-3);
  EXPECT_NEAR(returned_path2.states[0].point.getY(), 51.51238564279176, 1e-3);
}
