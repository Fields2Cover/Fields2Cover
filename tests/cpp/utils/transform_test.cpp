//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
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
  EXPECT_EQ(field.getField().getCellBorder(0).startPoint(), F2CPoint(0, 0, 0));
  EXPECT_EQ(field.getCRS(), "EPSG:28992");

  auto ref_gps_point = f2c::Transform::getRefPointInGPS(field);
  EXPECT_NEAR(ref_gps_point.getX(), 6.062131843297665, 1e-3);
  EXPECT_NEAR(ref_gps_point.getY(), 51.51238564279176, 1e-3);
  EXPECT_NEAR(ref_gps_point.getZ(), 0, 1e-3);

  f2c::Transform::transform(field, "EPSG:4326");
  auto final_field = (field.getField() + field.getRefPoint()).getCellBorder(0);
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
  field.setCRS("");
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
  EXPECT_EQ(field.getField().getCellBorder(0).startPoint(), F2CPoint(0, 0, 0));
  EXPECT_EQ(field.getCRS(), "UTM:32N datum:WGS84");
  EXPECT_EQ(field.getPrevCRS(), "EPSG:4326");

  f2c::Transform::transformToPrevCRS(field);
  auto final_field = (field.getField() + field.getRefPoint()).getCellBorder(0);
  for (int i = 0; i < final_field.size(); ++i) {
    EXPECT_NEAR(final_field.getGeometry(i).getX(),
        poly.getGeometry(0).getGeometry(i).getX(), 1e-3);
    EXPECT_NEAR(final_field.getGeometry(i).getY(),
        poly.getGeometry(0).getGeometry(i).getY(), 1e-3);
    EXPECT_NEAR(final_field.getGeometry(i).getZ(),
        poly.getGeometry(0).getGeometry(i).getZ(), 1e-3);
  }

  field.setCRS("");
  EXPECT_LT(field.area(), 1e-5);
  EXPECT_GT(field.area(), 0);
  f2c::Transform::transformToUTM(field, false);
  EXPECT_EQ(field.getCRS(), "UTM:32N datum:WGS84");
  EXPECT_NEAR(field.area(), 5.9, 0.005);
  f2c::Transform::transformToPrevCRS(field);
  EXPECT_LT(field.area(), 1e-5);
  EXPECT_GT(field.area(), 0);

  field.setCRS("");
  f2c::Transform::transformToUTM(field, true);
  EXPECT_EQ(field.getCRS(), "UTM:32N datum:ETRS89");
  EXPECT_NEAR(field.area(), 5.9, 0.005);
  f2c::Transform::transformToPrevCRS(field);
  EXPECT_LT(field.area(), 1e-5);
  EXPECT_GT(field.area(), 0);


  field.setCRS("EPSG:4258");
  f2c::Transform::transformToUTM(field);
  EXPECT_EQ(field.getCRS(), "UTM:32N datum:ETRS89");
  EXPECT_NEAR(field.area(), 5.9, 0.005);
  f2c::Transform::transformToPrevCRS(field);
  EXPECT_LT(field.area(), 1e-5);
  EXPECT_GT(field.area(), 0);

  field.setCRS("ERROR");
  EXPECT_THROW( f2c::Transform::transformToUTM(field), std::invalid_argument);
  field.setCRS("");
  EXPECT_THROW( f2c::Transform::transformToPrevCRS(field), std::invalid_argument);
}

TEST(fields2cover_utils_transformer, convert_to_single_digit_UTM_zones) {
  const std::vector<F2CPoint> gps_points {
    F2CPoint(-147.0, 64.0),
    F2CPoint(-147.0, -45.0)
  };

  for (const auto& gps_point : gps_points) {
    F2CField field;
    field.setRefPoint(gps_point);
    field.setEPSGCoordSystem(4326);

    f2c::Transform::transformToUTM(field, false);

    const std::string hemisphere {gps_point.getY() > 0 ? "N" : "S"};
    EXPECT_EQ(field.getCRS(), "UTM:6" + hemisphere + " datum:WGS84");
    EXPECT_EQ(field.getUTMZone(), "6");
    EXPECT_EQ(field.getUTMHemisphere(),
      gps_point.getY() > 0 ? "+north" : "+south");
    EXPECT_EQ(field.getPrevCRS(), "EPSG:4326");

    f2c::Transform::transformToPrevCRS(field);
    EXPECT_NEAR(field.getRefPoint().getX(), gps_point.getX(), 1e-6);
    EXPECT_NEAR(field.getRefPoint().getY(), gps_point.getY(), 1e-6);
  }
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
  strip.setCell(F2CCell::buffer(moved_swaths[0].getPath(), 15));
  F2CStrips strips;
  strips.emplace_back(strip);


  auto cells = moved_swaths.at(0).areaCovered();
  F2CField field(cells, "field");
  field.setEPSGCoordSystem(28992);
  field.setPrevCRS("EPSG:4326");
  EXPECT_NE(field.getRefPoint().getX(), 6.062131843297665);
  EXPECT_NE(field.getRefPoint().getY(), 51.51238564279176);
  EXPECT_GT(field.getRefPoint().getX(), 1e4);
  EXPECT_GT(field.getRefPoint().getY(), 1e4);
  EXPECT_NEAR(field.area(), 1150, 10);
  EXPECT_NEAR(strip.getCell().area(), 1150, 10);

  auto moved_strips = f2c::Transform::transformStrips(strips, "EPSG:28992", "EPSG:4326");
  auto moved_cells = f2c::Transform::transform(cells, "EPSG:28992", "EPSG:4326");
  EXPECT_NEAR(moved_strips[0].getCell().area(), moved_cells.area(), 1e-7);

  F2CPath path;
  path.appendSwath(moved_swaths.at(0), 10);
  F2CPath path2;
  F2CSwath swath2{F2CLineString({F2CPoint(0,0), F2CPoint(1,1)}), 40, 2};
  path2.appendSwath(swath2, 10);


  auto returned_path = f2c::Transform::transformPath(path, "EPSG:28992", "EPSG:4326");
  EXPECT_NEAR(returned_path[0].point.getX(), 6.062131843297665, 1e-3);
  EXPECT_NEAR(returned_path[0].point.getY(), 51.51238564279176, 1e-3);
  auto returned_path2 = f2c::Transform::transformPathWithFieldRef(path2, field, "EPSG:4326");
  EXPECT_NEAR(returned_path2[0].point.getX(), 6.062131843297665, 1e-3);
  EXPECT_NEAR(returned_path2[0].point.getY(), 51.51238564279176, 1e-3);
  auto returned_path3 = f2c::Transform::transformToPrevCRS(path2, field);
  EXPECT_NEAR(returned_path3[0].point.getX(), 6.062131843297665, 1e-3);
  EXPECT_NEAR(returned_path3[0].point.getY(), 51.51238564279176, 1e-3);

  auto returned_swaths =
    f2c::Transform::transformSwathsWithFieldRef(moved_swaths, field, "EPSG:4326");
  EXPECT_NEAR(returned_path2[0].point.getX(), 6.062131843297665, 1e-3);
  EXPECT_NEAR(returned_path2[0].point.getY(), 51.51238564279176, 1e-3);
}

TEST(fields2cover_utils_transformer, convert_to_field_crs_without_prev_crs) {
  F2CField field {F2CCells(F2CCell(F2CLinearRing({
    F2CPoint(5, 5), F2CPoint(15, 5), F2CPoint(15, 15), F2CPoint(5, 5)})))};
  ASSERT_EQ(field.getRefPoint(), F2CPoint(5, 5));
  ASSERT_TRUE(field.getPrevCRS().empty());

  EXPECT_EQ(f2c::Transform::transformToFieldCRS(F2CPoint(5, 5), field),
      F2CPoint(0, 0));
  EXPECT_EQ(f2c::Transform::transformToFieldCRS(F2CPoint(10, 8), field),
      F2CPoint(5, 3));
}

TEST(fields2cover_utils_transformer, convert_to_field_crs) {
  const F2CLineString gps_border({
    F2CPoint(6.062131843297665, 51.51238564279176, 0),
    F2CPoint(6.062215149507296, 51.51204470468504, 0),
    F2CPoint(6.062232423018175, 51.51197104647852, 0),
    F2CPoint(6.062259746209906, 51.51187606814959, 0)});
  F2CField field {F2CCells(F2CCell(F2CLinearRing({
    gps_border.getGeometry(0), gps_border.getGeometry(1),
    gps_border.getGeometry(2), gps_border.getGeometry(3),
    gps_border.getGeometry(0)})))};
  field.setEPSGCoordSystem(4326);
  f2c::Transform::transformToUTM(field);
  const auto local_border = field.getField().getCellBorder(0);

  // The field went through the same transformation, so its border is the
  // answer. Not bit for bit: the field added its GPS reference back before
  // projecting, which rounds in the last digit (measured 4e-14 m).
  const F2CLineString gps_copy = gps_border.clone();
  auto line = f2c::Transform::transformToFieldCRS(gps_border, field);
  ASSERT_EQ(line.size(), gps_border.size());
  for (size_t i = 0; i < line.size(); ++i) {
    EXPECT_NEAR(line.getGeometry(i).getX(), local_border.getGeometry(i).getX(), 1e-12);
    EXPECT_NEAR(line.getGeometry(i).getY(), local_border.getGeometry(i).getY(), 1e-12);
    EXPECT_EQ(gps_border.getGeometry(i), gps_copy.getGeometry(i));
  }

  // Two UTM coordinates of ~5.7e6 m differ by one ulp of ~1e-9 m.
  auto corner = f2c::Transform::transformToFieldCRS(gps_border.getGeometry(0), field);
  EXPECT_NEAR(corner.getX(), 0, 1e-8);
  EXPECT_NEAR(corner.getY(), 0, 1e-8);

  const F2CPoint p = gps_border.getGeometry(2);
  auto back = f2c::Transform::transform(
      f2c::Transform::transformToFieldCRS(p, field), field.getRefPoint(),
      field.getCRS(), field.getPrevCRS());
  EXPECT_NEAR(back.getX(), p.getX(), 1e-12);
  EXPECT_NEAR(back.getY(), p.getY(), 1e-12);
}
