#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import fields2cover as f2c


def _field():
  return f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint(
      [f2c.Point(0, 0), f2c.Point(100, 0), f2c.Point(100, 50),
       f2c.Point(0, 50), f2c.Point(0, 0)]))))


def _column(xs, first_id):
  swaths = f2c.Swaths()
  i = first_id
  for x in xs:
    swaths.push_back(f2c.Swath(f2c.LineString(f2c.VectorPoint(
        [f2c.Point(x, 5), f2c.Point(x, 45)])), 4.0, i))
    i += 1
  return swaths


def _driven(route):
  out = []
  for i in range(route.sizeVectorSwaths()):
    for s in route.getSwaths(i):
      out.append(s)
  return out


def test_fields2cover_route_multicell_start_point_reaches_python():
  cells = _field()
  sbc = f2c.SwathsByCells()
  sbc.push_back(_column([10, 15], 0))
  sbc.push_back(_column([90, 95], 10))

  order = f2c.RP_Boustrophedon()
  order.setStartAndEndPoint(f2c.Point(98, 25))
  driven = _driven(order.genRoute(cells, sbc))

  assert len(driven) == sbc.sizeTotal()
  assert driven[0].startPoint().getX() > 85.0
