#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import math
import fields2cover as f2c

def near(a, b, error = 1e-7):
  assert abs(a - b) < error

def test_fields2cover_types_cell_constructor():
  line = f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(), f2c.Point(2,0), f2c.Point(2,2), f2c.Point(0,2), f2c.Point()]));
  cell = f2c.Cell(line);
  cell_clone = cell.clone();
  near(cell.area(), 4)
  near(cell_clone.area(), 4)

def test_fields2cover_types_cell_addRing():
  ring1 = f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(), f2c.Point(2,0), f2c.Point(2,2), f2c.Point(0,2), f2c.Point()]));
  ring2 = f2c.LinearRing(f2c.VectorPoint(
      [f2c.Point(0.5,0.5), f2c.Point(1.5,0.5), f2c.Point(1.5,1.5),
        f2c.Point(0.5,1.5), f2c.Point(0.5,0.5)]));
  near(ring1.size(), 5);
  near(ring2.size(), 5);

  cell1 = f2c.Cell();
  cell1.addRing(ring1)
  near(cell1.area(), 4)
  cell1.addRing(ring2)
  near(cell1.area(), 3)
  near(cell1.size(), 2);

  cell1_ring1 = cell1.getGeometry(0);
  near(cell1_ring1.size(), 5);
  near(cell1_ring1.length(), 8);
  cell1_ring2 = cell1.getGeometry(1);
  near(cell1_ring2.size(), 5);
  near(cell1_ring2.length(), 4);

  cell_ring3 = f2c.LinearRing();
  cell1.getGeometry(0, cell_ring3);
  near(cell_ring3.size(), 5);
  near(cell_ring3.length(), 8);
  cell_ring4 = f2c.LinearRing();
  cell1.getGeometry(1, cell_ring4);
  near(cell_ring4.size(), 5);
  near(cell_ring4.length(), 4);


def test_fields2cover_types_cell_createLineUntilBorder():
  cell = f2c.Cell();
  ring1 = f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(), f2c.Point(2,0), f2c.Point(2,2), f2c.Point(0,2), f2c.Point()]));
  cell.addRing(ring1);
  line = cell.createLineUntilBorder(f2c.Point(1, 1), 0);
  near(line.size(), 2);
  if line.size() == 2:
    near(line.getX(0), 1)
    near(line.getY(0), 1)
    near(line.getX(1), 2)
    near(line.getY(1), 1)

def test_fields2cover_types_cell_isPointInBorder():
  cell = f2c.Cell();
  ring1 = f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(), f2c.Point(2,0), f2c.Point(2,2), f2c.Point(0,2), f2c.Point()]));
  cell.addRing(ring1);

  assert cell.isPointInBorder(f2c.Point(2,2))
  assert cell.isPointInBorder(f2c.Point(1,2))
  assert not cell.isPointInBorder(f2c.Point(1,1))
  assert not cell.isPointInBorder(f2c.Point(10,10))

def test_fields2cover_types_cell_buffer():
  cell = f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(), f2c.Point(1,0), f2c.Point(1,1), f2c.Point(0,1), f2c.Point()])))
  near(cell.area(), 1)

  cell_buffer = f2c.Cell.buffer(cell, 2.0);
  near(cell.area(), 1)
  near(cell_buffer.area(), 25, 0.1)

  p = f2c.Point(1, 2);
  point_buffer = f2c.Cell.buffer(p, 3.0)
  near(point_buffer.area(), 3 * 3 * math.pi, 0.2)
  near(p.getX(), 1)
  near(p.getY(), 2)

  line = f2c.LineString(f2c.VectorPoint([f2c.Point(3,4), f2c.Point(3,6)]))
  line_buffer = f2c.Cell.buffer(line, 4.0)
  near(line_buffer.area(), 2 * (4 * 2), 1e-5);


