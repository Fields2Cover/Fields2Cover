#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b, error = 1e-7):
  assert a == pytest.approx(b, error)

def test_fields2cover_decomp_simplifyForDecomposition():
  # A 100x100 field with a 1 x 0.5 m notch cut out of its top edge: smaller
  # than the 1.5 m the robot's coverage width asks for, so it goes.
  cells = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(0,0), f2c.Point(100,0), f2c.Point(100,100),
     f2c.Point(50,100), f2c.Point(50,99.5), f2c.Point(49,99.5),
     f2c.Point(49,100), f2c.Point(0,100), f2c.Point(0,0)]))));
  robot = f2c.Robot(2.5, 2.5);
  near(cells.area(), 100 * 100 - 0.5, 1e-6);

  simple = f2c.simplifyForDecomposition(cells, robot);
  near(simple.area(), 100 * 100, 1e-6);

def test_fields2cover_decomp_simplifyForDecomposition_share():
  # The same notch, asked for a tenth of the coverage width -- 0.25 m: the
  # 0.5 m deep notch is detail to keep.
  cells = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(0,0), f2c.Point(100,0), f2c.Point(100,100),
     f2c.Point(50,100), f2c.Point(50,99.5), f2c.Point(49,99.5),
     f2c.Point(49,100), f2c.Point(0,100), f2c.Point(0,0)]))));
  robot = f2c.Robot(2.5, 2.5);

  simple = f2c.simplifyForDecomposition(cells, robot, 0.1);
  near(simple.area(), 100 * 100 - 0.5, 1e-6);
