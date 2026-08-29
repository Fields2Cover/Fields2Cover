#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b, error = 1e-7):
  assert a == pytest.approx(b, error)

def test_fields2cover_hl_corridor_gen_generateHeadlands():
  cells = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(0,0), f2c.Point(50,0), f2c.Point(50,100), f2c.Point(0,100), f2c.Point(0,0)]))));
  cells.addGeometry(f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(50,0), f2c.Point(100,0), f2c.Point(100,100), f2c.Point(50,100), f2c.Point(50,0)]))));
  carved = f2c.HG_Corridor_gen().generateHeadlands(cells, 2.0);
  assert (carved.size() == 2);
  near(cells.area() - carved.area(), 200, 1e-3);
