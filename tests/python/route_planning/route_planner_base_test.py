#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_route_planner_base_simple_example():
  rand = f2c.Random(4);
  robot = f2c.Robot(2.0, 2.0);
  robot.setMinTurningRadius(2);
  robot.setMaxDiffCurv(0.1);

  const_hl = f2c.HG_Const_gen();
  ring1 = f2c.LinearRing()
  ring2 = f2c.LinearRing()
  ring3 = f2c.LinearRing()
  ring1.addPoint(0,0); ring1.addPoint(100,0); ring1.addPoint(100,100); ring1.addPoint(0,100); ring1.addPoint(0,0)
  ring2.addPoint(20,20); ring2.addPoint(20,30); ring2.addPoint(30,30); ring2.addPoint(30,20); ring2.addPoint(20,20)
  ring3.addPoint(60,60); ring3.addPoint(80,60); ring3.addPoint(80,80); ring3.addPoint(60,80); ring3.addPoint(60,60)

  cells = f2c.Cells(f2c.Cell(ring1))
  cells.addRing(0, ring2)
  cells.addRing(0, ring3)


  no_hl = const_hl.generateHeadlandArea(cells, robot.getCovWidth(), 3);
  hl_swaths = const_hl.generateHeadlandSwaths(cells, robot.getCovWidth(), 3, False);

  decomp = f2c.DECOMP_Boustrophedon();
  decomp.setSplitAngle(0.0);
  #no_hl = decomp.decompose(no_hl);

  bf = f2c.SG_BruteForce();
  swaths = bf.generateSwaths(3.1416, robot.getCovWidth(), no_hl);

  route_planner = f2c.RP_RoutePlannerBase()
  route = route_planner.genRoute(hl_swaths[1], swaths)

  path_planner = f2c.PP_PathPlanning();
  dubins = f2c.PP_DubinsCurves();

  path_dubins = path_planner.planPath(robot, route, dubins)

  f2c.Visualizer.figure();
  f2c.Visualizer.plot(cells);
  f2c.Visualizer.plot(no_hl);
  f2c.Visualizer.plot(hl_swaths[1]);
  f2c.Visualizer.plot(path_dubins);
  f2c.Visualizer.show();





