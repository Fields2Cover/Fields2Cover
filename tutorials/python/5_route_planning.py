#=============================================================================
#    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                     Author: Gonzalo Mier
#                        BSD-3 License
#=============================================================================


import math
import fields2cover as f2c

rand = f2c.Random(42);
robot = f2c.Robot(2.0, 6.0);
const_hl = f2c.HG_Const_gen();
field = rand.generateRandField(5, 1e4);
cells = field.field;
no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.robot_width);
bf = f2c.SG_BruteForce();
swaths = bf.generateSwaths(math.pi, robot.op_width, no_hl.getGeometry(0));

print("####### Tutorial 5.1 Boustrophedon ######");
boustrophedon_sorter = f2c.RP_Boustrophedon();
swaths = boustrophedon_sorter.genSortedSwaths(swaths);

f2c.Visualizer.figure(11);
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(swaths);
f2c.Visualizer.save("Tutorial_5_1_Boustrophedon_1");


for i in range(1, 4):
  f2c.Visualizer.figure(11 + i);
  f2c.Visualizer.plot(cells);
  f2c.Visualizer.plot(no_hl);
  f2c.Visualizer.plot(boustrophedon_sorter.genSortedSwaths(swaths, i));
  f2c.Visualizer.save("Tutorial_5_1_Boustrophedon_" + str(i+1));



print("####### Tutorial 5.2 Snake order ######");
snake_sorter = f2c.RP_Snake();
swaths = snake_sorter.genSortedSwaths(swaths);

f2c.Visualizer.figure(21);
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(swaths);
f2c.Visualizer.save("Tutorial_5_2_Snake_1");

for i in range(1, 4):
  f2c.Visualizer.figure(21 + i);
  f2c.Visualizer.plot(cells);
  f2c.Visualizer.plot(no_hl);
  f2c.Visualizer.plot(snake_sorter.genSortedSwaths(swaths, i));
  f2c.Visualizer.save("Tutorial_5_2_Snake_" + str(i+1));

swaths = bf.generateSwaths(math.pi, robot.op_width, no_hl.getGeometry(0));
print("####### Tutorial 5.3 Spiral order ######");
spiral_sorter = f2c.RP_Spiral(6);
swaths = spiral_sorter.genSortedSwaths(swaths);

f2c.Visualizer.figure(31);
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(swaths);
f2c.Visualizer.save("Tutorial_5_3_Spiral_1");

for i in range(1, 4):
  f2c.Visualizer.figure(31 + i);
  f2c.Visualizer.plot(cells);
  f2c.Visualizer.plot(no_hl);
  f2c.Visualizer.plot(spiral_sorter.genSortedSwaths(swaths, i));
  f2c.Visualizer.save("Tutorial_5_3_Spiral_" + str(i+1));


