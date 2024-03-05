#=============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#=============================================================================


import math
import fields2cover as f2c

rand = f2c.Random(42);
robot = f2c.Robot(2.0, 6.0);
field = rand.generateRandField(1e4, 5);
cells = field.getField();
const_hl = f2c.HG_Const_gen();
no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.getWidth());

print("####### Tutorial 4.1 Brute force swath generator ######");
n_swath = f2c.OBJ_NSwath();
bf_sw_gen = f2c.SG_BruteForce();
swaths_bf_nswath = bf_sw_gen.generateBestSwaths(n_swath, robot.getCovWidth(), no_hl.getGeometry(0));

f2c.Visualizer.figure();
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(swaths_bf_nswath);
f2c.Visualizer.save("Tutorial_4_1_Brute_force_NSwath.png");

swath_length = f2c.OBJ_SwathLength();

swaths_bf_swathlength = bf_sw_gen.generateBestSwaths(swath_length, robot.getCovWidth(), no_hl.getGeometry(0));


f2c.Visualizer.figure();
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(swaths_bf_swathlength);
f2c.Visualizer.save("Tutorial_4_1_Brute_force_SwathLength.png");

swaths_bf_angle = bf_sw_gen.generateSwaths(math.pi, robot.getCovWidth(), no_hl.getGeometry(0));
f2c.Visualizer.figure();
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(swaths_bf_angle);
f2c.Visualizer.save("Tutorial_4_1_Brute_force_Angle.png");


