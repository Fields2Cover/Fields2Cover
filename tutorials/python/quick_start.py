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
robot = f2c.Robot(2.0, 6.0, 0.5, 0.2);

cov_route = f2c.planCovRoute(robot, field, False);
cov_path = f2c.planCovPath(robot, field, False);


f2c.Visualizer.figure();
f2c.Visualizer.plot(field.getCellsAbsPosition());
f2c.Visualizer.plot(cov_route);
f2c.Visualizer.save("cov_route_quick_start.png");

f2c.Visualizer.figure();
f2c.Visualizer.plot(field.getCellsAbsPosition());
f2c.Visualizer.plot(cov_path);
f2c.Visualizer.save("cov_path_quick_start.png");


