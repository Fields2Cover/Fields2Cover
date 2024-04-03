#=============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#=============================================================================

import math
import fields2cover as f2c
import os
DATA_PATH = os.path.dirname(os.path.realpath(__file__))+"/" + 2 * "../"+"data/";

# Import field
field = f2c.Parser().importFieldGml(DATA_PATH + "test1.xml");
orig_field = field.clone();
# Transform into UTM to work in meters
f2c.Transform.transformToUTM(field);

robot = f2c.Robot(2.0, 6.0);
const_hl = f2c.HG_Const_gen()
no_hl = const_hl.generateHeadlands(field.getField(), 3.0 * robot.getWidth())
bf = f2c.SG_BruteForce()
swaths = bf.generateSwaths(math.pi, robot.getCovWidth(), no_hl.getGeometry(0))
snake_sorter = f2c.RP_Snake()
swaths = snake_sorter.genSortedSwaths(swaths)

robot.setMinTurningRadius(2)
path_planner = f2c.PP_PathPlanning()
dubins = f2c.PP_DubinsCurves()
path = path_planner.planPath(robot, swaths, dubins);


f2c.Visualizer.figure();
f2c.Visualizer.plot(field);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(path);
f2c.Visualizer.save("Tutorial_8_1_UTM.png");


# Transform the generated path back to the previousa CRS.
path_gps = f2c.Transform.transformToPrevCRS(path, field);
f2c.Transform.transformToPrevCRS(field);

ps = [path_gps.getState(i).point for i in range(path_gps.size())]
L = f2c.LineString(f2c.VectorPoint(ps))

f2c.Visualizer.figure();
f2c.Visualizer.plot(orig_field.getCellsAbsPosition());
f2c.Visualizer.plot(L);
#f2c.Visualizer.plot(path_gps);
#f2c.Visualizer.axis_equal();
f2c.Visualizer.save("Tutorial_8_1_GPS.png");



