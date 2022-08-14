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
}

