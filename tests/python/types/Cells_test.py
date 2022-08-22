#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import math
import fields2cover as f2c

def near(a, b, error = 1e-7):
  assert abs(a - b) < error

def test_fields2cover_types_cells_constructor():
  ring1 = f2c.LinearRing(f2c.VectorPoint(
      [f2c.Point(0,0), f2c.Point(1,0), f2c.Point(1,1), f2c.Point(0,1), f2c.Point(0,0)]));
  ring2 = f2c.LinearRing(f2c.VectorPoint(
      [f2c.Point(.1,.1), f2c.Point(.9,.1), f2c.Point(.9,.9),
          f2c.Point(.1,.9), f2c.Point(.1,.1)]));
  cell = f2c.Cell(ring1);
  cell.addRing(ring2);
  cells = f2c.Cells();
  near(cells.size(), 0);
  cells.addGeometry(cell);

  cells_clone = cells.clone();

  near(cells.getArea(), 0.36);
  near(cells_clone.getArea(), 0.36);
  near(cells_clone.getGeometry(0).getArea(), 0.36);


  cell2 = f2c.Cell(ring2);
  near(cell2.getArea(), 0.64);
  cells.addGeometry(cell2);

  cells_ring = cells.getInteriorRing(0, 0)
  near(cells_ring.getLength(), 3.2);


def test_fields2cover_types_cells_Difference():
  ring1 = f2c.LinearRing(f2c.VectorPoint(
      [f2c.Point(0,0), f2c.Point(2,0), f2c.Point(2,2), f2c.Point(0,2), f2c.Point(0,0)]));
  ring2 = f2c.LinearRing(f2c.VectorPoint(
      [f2c.Point(1,0), f2c.Point(3,0), f2c.Point(3,2), f2c.Point(1,2), f2c.Point(1,0)]));
  cell1 = f2c.Cell();
  cell2 = f2c.Cell();
  cell1.addRing(ring1);
  cell2.addRing(ring2);
  near(ring1.size(), 5);
  near(ring2.size(), 5);
  near(cell1.getArea(), 4);
  near(cell2.getArea(), 4);
  near(f2c.Cells(cell1).getArea(), 4);
  near(f2c.Cells(cell2).getArea(), 4);
  near(f2c.Cells(cell1).Difference(f2c.Cells(cell2)).getArea(), 2);

"""
def test_fields2cover_types_cells_Buffer():
  line = f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0,0), f2c.Point(2,0), f2c.Point(2,2), f2c.Point(0,2), f2c.Point(0,0)]));
  cells = f2c.Cells.Buffer(line, 2);
  assert cells.getArea() > 4
"""  

def test_fields2cover_types_cells_Intersection():
  cell1 = f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
      [f2c.Point(0,0), f2c.Point(2,0), f2c.Point(2,2), f2c.Point(0,2), f2c.Point(0,0)])));
  cell2 = f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
      [f2c.Point(1,0), f2c.Point(3,0), f2c.Point(3,2), f2c.Point(1,2), f2c.Point(1,0)])));
  cells2 = f2c.Cells(cell2);
  near(f2c.Cells.Intersection(cell1, cell2).getArea(), 2);
  # near(cells2.Intersection(cell1).getArea(), 2);

def test_fields2cover_types_cells_longLines():
  cells = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
      [f2c.Point(0,0), f2c.Point(2,0), f2c.Point(2,2), f2c.Point(0,2), f2c.Point(0,0)]))));
  line1 = cells.getSemiLongCurve(f2c.Point(), math.pi * 3.0/4.0)
  line2 = cells.getStraightLongCurve(f2c.Point(), math.pi * 3.0/4.0)

  near(line1.getX(0), 0, 1e-3);
  near(line1.getY(0), 0, 1e-3);
  near(line1.getX(1), -2*math.sqrt(2), 1e-3);
  near(line1.getY(1), 2*math.sqrt(2), 1e-3);
  near(line2.getX(0), -2*math.sqrt(2), 1e-3);
  near(line2.getY(0), 2*math.sqrt(2), 1e-3);
  near(line2.getX(1), 2*math.sqrt(2), 1e-3);
  near(line2.getY(1), -2*math.sqrt(2), 1e-3);

def test_fields2cover_types_cells_wherePoint():
  cells = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(0,0), f2c.Point(2,0), f2c.Point(2,2), f2c.Point(0,2), f2c.Point(0,0)]))));
  cells.addGeometry(f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(10,9), f2c.Point(12,9),f2c.Point(12,12),f2c.Point(10,12), f2c.Point(10,9)]))));
  assert (cells.isPointInBorder(f2c.Point(0, 0)));
  assert (cells.isPointInBorder(f2c.Point(1, 0)));
  assert (cells.isPointInBorder(f2c.Point(11, 12)));
  assert not (cells.isPointInBorder(f2c.Point(1, 1)));
  assert not (cells.isPointInBorder(f2c.Point(5, 5)));
  near(cells.getCellWherePoint(f2c.Point(5, 5)).getArea(), 0);
  near(cells.getCellWherePoint(f2c.Point(1, 1)).getArea(), 4);
  near(cells.getCellWherePoint(f2c.Point(0, 0)).getArea(), 4);
  near(cells.getCellWherePoint(f2c.Point(11, 11)).getArea(), 6);

