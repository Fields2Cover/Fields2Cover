#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b, error = 1e-7):
  assert abs(a - b) < error

# Transform class won't be in the interface
def test_fields2cover_utils_transformer_convertToF2CAndReturn():
  poly = f2c.Cell(f2c.LinearRing(f2c.VectorPoint([
    f2c.Point(6.062131843297665, 51.51238564279176, 0),
    f2c.Point(6.062215149507296, 51.51204470468504, 0),
    f2c.Point(6.062232423018175, 51.51197104647852, 0),
    f2c.Point(6.062259746209906, 51.51187606814959, 0),
    f2c.Point(6.062131843297665, 51.51238564279176, 0),
    ])));
  poly_to_transform = poly;
  assert (poly == poly_to_transform);
  field = f2c.Field(f2c.Cells(poly_to_transform));
  field.setEPSGCoordSystem(4326);
  f2c.Transform.transform(field, "EPSG:28992");
  near(field.field.getCellBorder(0).StartPoint().getX(), 0);
  near(field.field.getCellBorder(0).StartPoint().getY(), 0);
  near(field.field.getCellBorder(0).StartPoint().getZ(), 0);
  assert (field.coord_sys == "EPSG:28992");

  ref_gps_point = f2c.Transform.getRefPointInGPS(field);
  near(ref_gps_point.getX(), 6.062131843297665, 1e-3);
  near(ref_gps_point.getY(), 51.51238564279176, 1e-3);
  near(ref_gps_point.getZ(), 0, 1e-3);

  f2c.Transform.transform(field, "EPSG:4326");
  final_field = field.getCellsAbsPosition().getCellBorder(0);
  for i in range(final_field.size()):
    near(final_field.getGeometry(i).getX(),
        poly.getGeometry(0).getGeometry(i).getX(), 1e-3);
    near(final_field.getGeometry(i).getY(),
        poly.getGeometry(0).getGeometry(i).getY(), 1e-3);
    near(final_field.getGeometry(i).getZ(),
        poly.getGeometry(0).getGeometry(i).getZ(), 1e-3);

  path = f2c.Path()
  f2c.Transform.transformPathWithFieldRef(path, field, "EPSG:4326");
