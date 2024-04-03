//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_field, area) {
  F2CField field;
  EXPECT_EQ(field.area(), 0);

  F2CCells cells {
    F2CCell(F2CLinearRing({
          F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)
    }))
  };
  field.setField(cells);
  EXPECT_EQ(field.area(), 4);
  const auto field2 = field;
  EXPECT_EQ(field2.area(), 4);
}

TEST(fields2cover_types_field, clone) {
  F2CCells cells {F2CCell(F2CLinearRing({
      F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)
  }))};
  F2CField field{F2CCells()};
  field.setField(cells);
  field.setId("Nice field");
  field.setUTMCoordSystem("42N");
  EXPECT_EQ(field.area(), 4);
  EXPECT_EQ(field.getId(), "Nice field");
  EXPECT_TRUE(field.isCoordSystemUTM());
  EXPECT_EQ(field.getUTMCoordSystem(), "42N");

  auto field2 = field.clone();
  EXPECT_EQ(field.area(), 4);
  EXPECT_EQ(field.getId(), "Nice field");
  EXPECT_TRUE(field.isCoordSystemUTM());
  EXPECT_FALSE(field.isCoordSystemEPSG());
  EXPECT_EQ(field.getUTMCoordSystem(), "42N");
  EXPECT_EQ(field.getEPSGCoordSystem(), -1);
  EXPECT_EQ(field2.area(), 4);
  EXPECT_EQ(field2.getId(), "Nice field");
  EXPECT_TRUE(field2.isCoordSystemUTM());
  EXPECT_EQ(field2.getUTMCoordSystem(), "42N");
  EXPECT_EQ(field2.getUTMZone(), "42");
  EXPECT_EQ(field2.getUTMHemisphere(), "+north");

  field2.setId("Change");
  field2.setUTMCoordSystem("50S");
  EXPECT_EQ(field.getId(), "Nice field");
  EXPECT_TRUE(field.isCoordSystemUTM());
  EXPECT_EQ(field.getUTMCoordSystem(), "42N");
  EXPECT_EQ(field.getUTMZone(), "42");
  EXPECT_EQ(field.getUTMHemisphere(), "+north");

  EXPECT_EQ(field2.getId(), "Change");
  EXPECT_TRUE(field2.isCoordSystemUTM());
  EXPECT_EQ(field2.getUTMCoordSystem(), "50S");
  EXPECT_EQ(field2.getUTMZone(), "50");
  EXPECT_EQ(field2.getUTMHemisphere(), "+south");


  F2CField field3;
  field3 = field;
  EXPECT_EQ(field.getId(), "Nice field");

  field3.setId("Change 3");
  field3.setUTMCoordSystem("31Z", " etrs89");
  EXPECT_EQ(field3.getId(), "Change 3");
  EXPECT_TRUE(field3.isCoordSystemUTM());
  EXPECT_EQ(field3.getUTMCoordSystem(), "31Z");
  EXPECT_EQ(field3.getUTMZone(), "31");
  EXPECT_EQ(field3.getUTMHemisphere(), "");

  EXPECT_EQ(field.getId(), "Nice field");
  field2.setEPSGCoordSystem(-5);
  EXPECT_EQ(field.getEPSGCoordSystem(), -1);
}

TEST(fields2cover_types_field, getCellsAbsPosition) {
  F2CCells cells {F2CCell(F2CLinearRing({
      F2CPoint(0,0), F2CPoint(2,0),F2CPoint(2,2),F2CPoint(0,2), F2CPoint(0,0)
  }))};
  cells = cells + F2CPoint(5, 6);
  EXPECT_EQ(cells.getGeometry(0).getGeometry(0).getX(0), 5);
  EXPECT_EQ(cells.getGeometry(0).getGeometry(0).getY(0), 6);
  EXPECT_EQ(cells.getGeometry(0).getGeometry(0).getX(1), 7);
  EXPECT_EQ(cells.getGeometry(0).getGeometry(0).getY(1), 6);

  F2CField field(cells);
  field.setEPSGCoordSystem(12345);
  EXPECT_FALSE(field.isCoordSystemUTM());
  EXPECT_TRUE(field.isCoordSystemEPSG());
  EXPECT_EQ(field.getEPSGCoordSystem(), 12345);
  EXPECT_EQ(field.getUTMCoordSystem(), "");
  EXPECT_EQ(field.getRefPoint().getX(), 5);
  EXPECT_EQ(field.getRefPoint().getY(), 6);
  EXPECT_EQ(field.getField().getGeometry(0).getGeometry(0).getX(0), 0);
  EXPECT_EQ(field.getField().getGeometry(0).getGeometry(0).getY(0), 0);
  EXPECT_NE(field.getField().getGeometry(0).getGeometry(0).getX(1), 0);
  EXPECT_EQ(field.getField().getGeometry(0).getGeometry(0).getY(1), 0);

  F2CCells o_cells = field.getCellsAbsPosition();
  EXPECT_EQ(o_cells.getGeometry(0).getGeometry(0).getX(0), 5);
  EXPECT_EQ(o_cells.getGeometry(0).getGeometry(0).getY(0), 6);
  EXPECT_EQ(o_cells.getGeometry(0).getGeometry(0).getX(1), 7);
  EXPECT_EQ(o_cells.getGeometry(0).getGeometry(0).getY(1), 6);
}

TEST(fields2cover_types_field, getUTMData) {
  EXPECT_EQ(F2CField::getUTMCoordSystem("32S"), "");
  EXPECT_EQ(F2CField::getUTMCoordSystem("not found", "def_if_not_found"), "def_if_not_found");
  EXPECT_EQ(F2CField::getUTMCoordSystem("UTM:10N"), "10N");
  EXPECT_EQ(F2CField::getUTMCoordSystem("UTM: 10N"), "10N");
  EXPECT_EQ(F2CField::getUTMCoordSystem("UTM 10N datum etrs89"), "10N");
  EXPECT_EQ(F2CField::getUTMDatum("UTM 10N datum etrs89"), "etrs89");
  EXPECT_EQ(F2CField::getUTMDatum("UTM 10N datum:etrs89"), "etrs89");
  EXPECT_EQ(F2CField::getUTMDatum("84WS"), "etrs89");
  F2CField f;
  f.setUTMCoordSystem("32N");
  EXPECT_EQ(f.getUTMCoordSystem(), "32N");
  f.setUTMCoordSystem("UTM33N");
  EXPECT_EQ(f.getUTMDatum(), "etrs89");
  EXPECT_EQ(f.getUTMCoordSystem(), "33N");
  EXPECT_EQ(f.getUTMDatum(), "etrs89");
  f.setUTMCoordSystem("UTM:34N datum:84WG");
  EXPECT_EQ(f.getUTMCoordSystem(), "34N");
  EXPECT_EQ(f.getUTMDatum(), "84WG");
  f.setCRS("UTM:35N +datum:85WG");
  EXPECT_EQ(f.getUTMCoordSystem(), "35N");
  EXPECT_EQ(f.getUTMDatum(), "85WG");
}
