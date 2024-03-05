#=============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#=============================================================================


import math
import fields2cover as f2c

rand = f2c.Random(42);
robot = f2c.Robot(2.0, 26.0);
field = rand.generateRandField(1e4, 5);
cells = field.getField();

print("####### Tutorial 3.1 Constant width headland generator ######");
const_hl = f2c.HG_Const_gen();
no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.getWidth());
print("The complete area is ", cells.area(),
    ", and the area without headlands is ", no_hl.area());

f2c.Visualizer.figure();
f2c.Visualizer.plot(field);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.show();


f2c.Visualizer.figure();
f2c.Visualizer.plot(field);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.save("Tutorial_3_1_Const_width.png");
