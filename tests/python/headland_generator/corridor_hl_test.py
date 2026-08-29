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

def test_fields2cover_hl_corridor_gen_corridorShares():
  cells = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(0,0), f2c.Point(20,0), f2c.Point(20,10), f2c.Point(0,10), f2c.Point(0,0)]))));
  cells.addGeometry(f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
    [f2c.Point(10,10), f2c.Point(20,10), f2c.Point(20,20), f2c.Point(10,20), f2c.Point(10,10)]))));
  shares = f2c.HG_Corridor_gen().corridorShares(cells);
  assert (len(shares) == 2);
  for share in shares:
    assert (not share.same_size);
    near(share.shared_length, 10, 1e-2);
  # The smaller cell gives the whole corridor, the larger one gives nothing.
  by_cell = {share.cell_i: share for share in shares};
  assert (by_cell[0].cell_k == 1 and by_cell[1].cell_k == 0);
  near(by_cell[0].share, 0.0, 1e-9);
  near(by_cell[1].share, 1.0, 1e-9);
