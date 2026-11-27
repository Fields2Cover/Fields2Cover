#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def square(x0, y0, x1, y1):
  ring = f2c.LinearRing()
  ring.addPoint(x0, y0); ring.addPoint(x1, y0); ring.addPoint(x1, y1)
  ring.addPoint(x0, y1); ring.addPoint(x0, y0)
  return ring

def test_fields2cover_rp_free_space_settings():
  planner = f2c.RP_FreeSpaceRoutePlanner()
  assert isinstance(planner, f2c.RP_RoutePlannerBase)
  planner.setClearance(3.0)
  planner.setClearanceCost(5.0)
  planner.setSampleStep(0.25)
  assert planner.getClearance() == 3.0
  assert planner.getClearanceCost() == 5.0
  assert planner.getSampleStep() == 0.25

def test_fields2cover_rp_free_space_counts_pieces():
  planner = f2c.RP_FreeSpaceRoutePlanner()

  # A headland ring around the crop is one piece of ground.
  cells = f2c.Cells(f2c.Cell(square(0, 0, 100, 100)))
  cells.addRing(0, square(20, 20, 80, 80))
  planner.createShortestGraph(cells, f2c.SwathsByCells(), 1e-4)
  assert planner.getComponentCount() == 1

  # Two blocks with no ground between them are two.
  apart = f2c.Cells(f2c.Cell(square(0, 0, 10, 10)))
  apart.addGeometry(f2c.Cell(square(20, 0, 30, 10)))
  planner.createShortestGraph(apart, f2c.SwathsByCells(), 1e-4)
  assert planner.getComponentCount() == 2
