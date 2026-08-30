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


print("####### Tutorial 3.2 Required width headland generator ######");
robot.setMinTurningRadius(8.0);
req_hl = f2c.HG_Req_gen();
req_no_hl = req_hl.generateHeadlands(cells, robot, f2c.VectorDouble([0.0]));
const_no_hl = const_hl.generateHeadlands(cells, robot, f2c.VectorDouble([0.0]));
print("With swaths at 0 rad, the area without headlands is ", req_no_hl.area(),
    ", while a constant headland leaves ", const_no_hl.area());

f2c.Visualizer.figure();
f2c.Visualizer.plot(field);
f2c.Visualizer.plot(const_no_hl);
f2c.Visualizer.plot(req_no_hl);
f2c.Visualizer.show();

f2c.Visualizer.figure();
f2c.Visualizer.plot(field);
f2c.Visualizer.plot(const_no_hl);
f2c.Visualizer.plot(req_no_hl);
f2c.Visualizer.save("Tutorial_3_2_Req_width.png");

print("####### Tutorial 3.3 Corridor headland generator ######");
l_cells = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint([
        f2c.Point(  0,  0), f2c.Point( 90,  0), f2c.Point( 90, 20),
        f2c.Point( 20, 20), f2c.Point( 20, 90), f2c.Point(  0, 90),
        f2c.Point(  0,  0)]))));
hl_width = robot.getWidth();
r_w = 3.0;  # narrower than getCovWidth(), so more swaths fit

single_no_hl = const_hl.generateHeadlands(l_cells, hl_width);
decomp = f2c.DECOMP_TrapezoidalDecomp();
decomp.setSplitAngle(0.5*math.pi);
decomp_cells = decomp.decompose(single_no_hl);

bf = f2c.SG_BruteForce();
obj = f2c.OBJ_NSwathModified();
route_planner = f2c.RP_RoutePlannerBase();
corridor_hl = f2c.HG_Corridor_gen();
robot.setMinTurningRadius(0.5);
robot.setMaxDiffCurv(1.5);
path_planner = f2c.PP_PathPlanning();
turn = f2c.PP_DubinsCurvesCC();

# 1. Double headland: decompose, then headland every border again, so the
# new internal border loses a headland from both cells that meet there.
double_no_hl = const_hl.generateHeadlands(decomp_cells, hl_width);
double_swaths = bf.generateBestSwaths(obj, r_w, double_no_hl);
double_route = route_planner.genRoute(single_no_hl, double_swaths);
double_path = path_planner.planPath(robot, double_route, turn);
print("Double headland leaves ", double_no_hl.area());

f2c.Visualizer.figure();
f2c.Visualizer.plot(l_cells);
f2c.Visualizer.plot(double_no_hl);
f2c.Visualizer.plot(double_path);
f2c.Visualizer.xlim(-5, 95);
f2c.Visualizer.ylim(-5, 95);
f2c.Visualizer.save("Tutorial_3_3_Double.png");

# 2. Corridor, ASYMMETRIC (the default): the smaller cell gives up the whole
# corridor and the larger one keeps its full width up to the border.
corridor_hl.setShareMode(f2c.CorridorShareMode_ASYMMETRIC);
corridor_no_hl = corridor_hl.generateHeadlands(decomp_cells, hl_width);
corridor_swaths = bf.generateBestSwaths(obj, r_w, corridor_no_hl);
corridor_route = route_planner.genRoute(single_no_hl, corridor_swaths);
corridor_path = path_planner.planPath(robot, corridor_route, turn);
print("Asymmetric corridor leaves ", corridor_no_hl.area());
for share in corridor_hl.corridorShares(decomp_cells):
  print("Cell", share.cell_i, "gives", share.share,
    "of the corridor over", share.shared_length,
    "m of border with cell", share.cell_k);

f2c.Visualizer.figure();
f2c.Visualizer.plot(l_cells);
f2c.Visualizer.plot(corridor_no_hl);
f2c.Visualizer.plot(corridor_path);
f2c.Visualizer.xlim(-5, 95);
f2c.Visualizer.ylim(-5, 95);
f2c.Visualizer.save("Tutorial_3_3_Corridor.png");

# 3. Shared corridor, SYMMETRIC: both cells give half the corridor,
# regardless of size.
corridor_hl.setShareMode(f2c.CorridorShareMode_SYMMETRIC);
shared_no_hl = corridor_hl.generateHeadlands(decomp_cells, hl_width);
shared_swaths = bf.generateBestSwaths(obj, r_w, shared_no_hl);
shared_route = route_planner.genRoute(single_no_hl, shared_swaths);
shared_path = path_planner.planPath(robot, shared_route, turn);
print("Symmetric shared corridor leaves ", shared_no_hl.area());

f2c.Visualizer.figure();
f2c.Visualizer.plot(l_cells);
f2c.Visualizer.plot(shared_no_hl);
f2c.Visualizer.plot(shared_path);
f2c.Visualizer.xlim(-5, 95);
f2c.Visualizer.ylim(-5, 95);
f2c.Visualizer.save("Tutorial_3_3_Shared.png");
