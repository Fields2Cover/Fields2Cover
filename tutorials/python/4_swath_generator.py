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

print("####### Tutorial 4.1 Brute force swath generator ######");
n_swath = f2c.OBJ_NSwath();
bf_sw_gen = f2c.SG_BruteForce();
swaths_bf_nswath = bf_sw_gen.generateBestSwaths(n_swath, robot.op_width, no_hl.getGeometry(0));

f2c.Visualizer.figure(4);
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(swaths_bf_nswath);
f2c.Visualizer.save("Tutorial_4_1_Brute_force_NSwath");

swath_length = f2c.OBJ_SwathLength();

swaths_bf_swathlength = bf_sw_gen.generateBestSwaths(swath_length, robot.op_width, no_hl.getGeometry(0));


f2c.Visualizer.figure(5);
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(swaths_bf_swathlength);
f2c.Visualizer.save("Tutorial_4_1_Brute_force_SwathLength");

swaths_bf_angle = bf_sw_gen.generateSwaths(math.pi, robot.op_width, no_hl.getGeometry(0));
f2c.Visualizer.figure(6);
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(swaths_bf_angle);
f2c.Visualizer.save("Tutorial_4_1_Brute_force_Angle");


