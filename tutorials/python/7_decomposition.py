#=============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#=============================================================================

import math
import fields2cover as f2c

print("####### Tutorial 7.1 Why and when decompose ######")

cells = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint([
        f2c.Point(  0,  0),
        f2c.Point( 90,  0),
        f2c.Point( 90, 20),
        f2c.Point( 20, 20),
        f2c.Point( 20, 90),
        f2c.Point(  0, 90),
        f2c.Point(  0,  0)]))));

f2c.Visualizer.figure();
f2c.Visualizer.plot(cells);
f2c.Visualizer.xlim(-5,95);
f2c.Visualizer.ylim(-5,95);
f2c.Visualizer.save("Tutorial_7_1_field.png");

robot = f2c.Robot(1.0, 1.0);
r_w = robot.getCovWidth();

const_hl = f2c.HG_Const_gen()
bf = f2c.SG_BruteForce();
obj = f2c.OBJ_NSwathModified();

no_hl_wo_decomp = const_hl.generateHeadlands(cells, 3.0 * r_w);
swaths_wo_decomp = bf.generateBestSwaths(obj, r_w, no_hl_wo_decomp);
print("Without decomposition >> ", obj.computeCost(swaths_wo_decomp));

decomp = f2c.DECOMP_TrapezoidalDecomp();
decomp.setSplitAngle(0.5*math.pi);
decomp_cell = decomp.decompose(cells);

no_hl_decomp = const_hl.generateHeadlands(decomp_cell, 3.0 * r_w);
swaths_decomp = bf.generateBestSwaths(obj, r_w, no_hl_decomp);
print("With decomposition >> ", obj.computeCost(swaths_decomp));


f2c.Visualizer.figure();
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl_wo_decomp);
f2c.Visualizer.plot(swaths_wo_decomp);
f2c.Visualizer.xlim(-5,95);
f2c.Visualizer.ylim(-5,95);
f2c.Visualizer.save("Tutorial_7_1_without_decomposition.png");

f2c.Visualizer.figure();
f2c.Visualizer.plot(decomp_cell);
f2c.Visualizer.plot(no_hl_decomp);
f2c.Visualizer.plot(swaths_decomp);
f2c.Visualizer.xlim(-5,95);
f2c.Visualizer.ylim(-5,95);
f2c.Visualizer.save("Tutorial_7_1_with_decomposition.png");

print("####### Tutorial 7.2 Decomposition with route planner ######");

mid_hl = const_hl.generateHeadlands(cells, 1.5 * r_w);
decomp_mid_hl = decomp.decompose(mid_hl);
no_hl = const_hl.generateHeadlands(decomp_mid_hl, 1.5 * r_w);
swaths = bf.generateBestSwaths(obj, r_w, no_hl);

route_planner = f2c.RP_RoutePlannerBase();
route = route_planner.genRoute(mid_hl, swaths);

f2c.Visualizer.figure();
f2c.Visualizer.plot(cells);
f2c.Visualizer.plot(no_hl);
f2c.Visualizer.plot(route);
f2c.Visualizer.xlim(-5,95);
f2c.Visualizer.ylim(-5,95);
f2c.Visualizer.save("Tutorial_7_2_route.png");



