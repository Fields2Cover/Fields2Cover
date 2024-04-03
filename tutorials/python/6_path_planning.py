#=============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#=============================================================================

import math
import fields2cover as f2c

rand = f2c.Random(42)
robot = f2c.Robot(2.0, 6.0)
field = rand.generateRandField(1e4, 5);
cells = field.getField();
const_hl = f2c.HG_Const_gen()
no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.getWidth())
bf = f2c.SG_BruteForce()

swaths = bf.generateSwaths(math.pi, robot.getCovWidth(), no_hl.getGeometry(0))

snake_sorter = f2c.RP_Snake()
swaths = snake_sorter.genSortedSwaths(swaths)
swaths.at(0).getPath().exportToWkt()


robot.setMinTurningRadius(2)  # m
robot.setMaxDiffCurv(0.1)  # 1/m^2
path_planner = f2c.PP_PathPlanning()

print("####### Tutorial 6.1 Dubins curves ######")
dubins = f2c.PP_DubinsCurves()
path_dubins = path_planner.planPath(robot, swaths, dubins);


f2c.Visualizer.figure();
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(path_dubins);
f2c.Visualizer.plot(swaths);
f2c.Visualizer.save("Tutorial_6_1_Dubins.png");

print("\n\n####### Tutorial 6.2 Dubins curves with Continuous curvature ######");
dubins_cc = f2c.PP_DubinsCurvesCC();
path_dubins_cc = path_planner.planPath(robot, swaths, dubins_cc);

f2c.Visualizer.figure();
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(path_dubins_cc);
f2c.Visualizer.plot(swaths);
f2c.Visualizer.save("Tutorial_6_2_Dubins_CC.png");

print("\n\n####### Tutorial 6.3 Reeds-Shepp curves ######")
reeds_shepp = f2c.PP_ReedsSheppCurves();
path_reeds_shepp = path_planner.planPath(robot, swaths, reeds_shepp);

f2c.Visualizer.figure();
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(path_reeds_shepp);
f2c.Visualizer.plot(swaths);
f2c.Visualizer.save("Tutorial_6_3_Reeds_Shepp.png");

print("\n\n####### Tutorial 6.4 Reeds-Shepp curves with Continuous curvature ######")
reeds_shepp_hc = f2c.PP_ReedsSheppCurvesHC();
path_reeds_shepp_hc = path_planner.planPath(robot, swaths, reeds_shepp_hc);

f2c.Visualizer.figure();
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(path_reeds_shepp_hc);
f2c.Visualizer.plot(swaths);
f2c.Visualizer.save("Tutorial_6_4_Reeds_Shepp_HC.png");

