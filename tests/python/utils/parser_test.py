#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c
import os
DATA_PATH = os.path.dirname(os.path.realpath(__file__))+"/" + 3*"../"+"data/";


def near(a, b, error = 1e-7):
  assert abs(a - b) < error


def test_fields2cover_utils_parser_importGml():
  fields = f2c.Fields();
  f2c.Parser.importGml(DATA_PATH + "test1.xml", fields);

  assert (fields[0].getId() == "b913fe9d-ed9f-4cc4-aa77-a10652165455");
  assert (fields[0].getCRS() == "EPSG:4258");

def test_fields2cover_utils_parser_getAreaImported():
  fields = f2c.Fields();
  f2c.Parser.importGml(DATA_PATH + "test1.xml", fields);
  # f2c.Transform.transform(fields[0], "EPSG:28992");
  near(fields[0].getField().area(), 2.24e-05, 1e-7);


def test_fields2cover_utils_parser_importJson():
  # Need more complex tests
  fields = f2c.Fields();
  f2c.Parser.importJson(DATA_PATH + "test.json", fields);
  field = fields[0];
  field.setEPSGCoordSystem(4326);

  assert (field.getId() == "test parcel");
  assert (field.getCRS() == "EPSG:4326");

  f2c.Visualizer.figure();
  f2c.Visualizer.plot(field);
  f2c.Visualizer.show();

def test_fields2cover_utils_parser_importJson_2_cell():
  fields = f2c.Fields();
  f2c.Parser.importJson(DATA_PATH + "test_2_cell.json", fields);
  assert (fields.size() == 2);
  assert (fields[0].getId() == "field1");
  assert (fields[1].getId() == "field2");

