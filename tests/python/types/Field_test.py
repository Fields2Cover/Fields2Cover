#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)

def test_fields2cover_types_field_area():
  field = f2c.Field();
  near(field.getArea(), 0);

  cells = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(0,0), f2c.Point(2,0),f2c.Point(2,2),f2c.Point(0,2), f2c.Point(0,0)]))));
  field.field = cells;
  near(field.getArea(), 4);
  field2 = field.clone();
  near(field2.getArea(), 4);


