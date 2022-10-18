#=============================================================================
#    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                     Author: Gonzalo Mier
#                        BSD-3 License
#=============================================================================

import math
import fields2cover as f2c

rand = f2c.Random(42)
robot = f2c.Robot(2.0, 6.0)
const_hl = f2c.HG_Const_gen()
field = rand.generateRandField(5, 1e4)
cells = field.field
no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.robot_width)
bf = f2c.SG_BruteForce()

swaths = bf.generateSwaths(math.pi, robot.op_width, no_hl.getGeometry(0))

snake_sorter = f2c.RP_Snake()
swaths = snake_sorter.genSortedSwaths(swaths)
swaths.at(0).getPath().exportToWkt()


robot.setMinRadius(2)  # m
robot.linear_curv_change = 0.1  # 1/m^2
path_planner = f2c.PP_PathPlanning()

print("####### Tutorial 6.1 Dubins curves ######")
dubins = f2c.PP_DubinsCurves()
path_dubins = path_planner.searchBestPath(robot, swaths, dubins);


f2c.Visualizer.figure(61);
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(path_dubins);
f2c.Visualizer.plot(swaths);
f2c.Visualizer.save("Tutorial_6_1_Dubins");

print("\n\n####### Tutorial 6.2 Dubins curves with Continuous curvature ######");
dubins_cc = f2c.PP_DubinsCurvesCC();
path_dubins_cc = path_planner.searchBestPath(robot, swaths, dubins_cc);

f2c.Visualizer.figure(62);
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(path_dubins_cc);
f2c.Visualizer.plot(swaths);
f2c.Visualizer.save("Tutorial_6_2_Dubins_CC");

print("\n\n####### Tutorial 6.3 Reeds-Shepp curves ######")
reeds_shepp = f2c.PP_ReedsSheppCurves();
path_reeds_shepp = path_planner.searchBestPath(robot, swaths, reeds_shepp);

f2c.Visualizer.figure(63);
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(path_reeds_shepp);
f2c.Visualizer.plot(swaths);
f2c.Visualizer.save("Tutorial_6_3_Reeds_Shepp");

print("\n\n####### Tutorial 6.4 Reeds-Shepp curves with Continuous curvature ######")
reeds_shepp_hc = f2c.PP_ReedsSheppCurvesHC();
path_reeds_shepp_hc = path_planner.searchBestPath(robot, swaths, reeds_shepp_hc);

f2c.Visualizer.figure(64);
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(path_reeds_shepp_hc);
f2c.Visualizer.plot(swaths);
f2c.Visualizer.save("Tutorial_6_4_Reeds_Shepp_HC");

