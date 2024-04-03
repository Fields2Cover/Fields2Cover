#=============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#=============================================================================


import math
import fields2cover as f2c

print("####### Tutorial 2.1.1 Remaining Area ######");

total_field = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint([
    f2c.Point(-2,-2), f2c.Point(6,-2), f2c.Point(6,6),
    f2c.Point(-2,6), f2c.Point(-2,-2)]))));
field = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint([
    f2c.Point(0,0), f2c.Point(4,0), f2c.Point(4,4),
    f2c.Point(0,4), f2c.Point(0,0)]))));

rem_area = f2c.OBJ_RemArea();
print("The remaining area is ",
    rem_area.computeCost(total_field, field), ", and with sign is ",
    rem_area.computeCostWithMinimizingSign(total_field, field));




print("\n\n####### Tutorial 2.2.1 Field Coverage ######");
width = 2.0;
swath1 = f2c.Swath(f2c.LineString(f2c.VectorPoint([
    f2c.Point(0.0, 1.0), f2c.Point(4.0, 1.0)])), width);
swath2 = f2c.Swath(f2c.LineString(f2c.VectorPoint([
    f2c.Point(0.0, 3.0), f2c.Point(4.0, 3.0)])), width);
swath3 = f2c.Swath(f2c.LineString(f2c.VectorPoint([
    f2c.Point(0.0, 2.0), f2c.Point(4.0, 2.0)])), width);

swaths1 = f2c.Swaths();
swaths1.push_back(swath1);
swaths3 = f2c.Swaths();
[swaths3.push_back(s) for s in [swath1, swath2, swath3]];

field_cov = f2c.OBJ_FieldCoverage();
print("The field coverage with swath1 is ",
    field_cov.computeCost(field, swaths1), " and with all of the swaths ",
    field_cov.computeCost(field, swaths3));
print("The field coverage with sign for all of the swaths is ",
    field_cov.computeCostWithMinimizingSign(field, swaths3));

print("\n####### Tutorial 2.2.2 Number of swaths ######");
n_swaths = f2c.OBJ_NSwath();
print("The number of swaths with swath1 is ",
    n_swaths.computeCost(swaths1), " and with all of the swaths ",
    n_swaths.computeCost(field, swaths3));

print("\n####### Tutorial 2.2.3 Overlap ######");
overlaps = f2c.OBJ_Overlaps();
print("The field overlapping with swath1 is ",
    overlaps.computeCost(field, swaths1), " and with all of the swaths ",
    overlaps.computeCost(field, swaths3));

print("\n####### Tutorial 2.2.4 Swath length ######");
swath_length = f2c.OBJ_SwathLength();
print("The swath length with swath1 is ",
    swath_length.computeCost(field, swaths1), " and with all of the swaths ",
    swath_length.computeCost(field, swaths3));


print("\n\n####### Tutorial 2.3.1 Distance with turns ######");
line1 = f2c.LineString(f2c.VectorPoint([f2c.Point(0.0, 0.0), f2c.Point(0.0, 1.0)]));
swath1 = f2c.Swath(line1);
line2 = f2c.LineString(f2c.VectorPoint([f2c.Point(1.0, 1.0), f2c.Point(1.0, 0.0)]));
swath2 = f2c.Swath(line2);
swaths_path = f2c.Swaths();
swaths_path.push_back(swath1);
swaths_path.push_back(swath2);
robot = f2c.Robot(2.0, 3.0);
robot.setMinTurningRadius(0.5);
complete_length = f2c.OBJ_CompleteTurnPathObj_Dubins(robot);
print("The complete length is: ", complete_length.computeCost(swaths_path),
    " =~= ", 1 + 1 + math.pi/2.0);

print("\n####### Tutorial 2.3.2 Direct distance without turns ######");
direct_dist = f2c.OBJ_DirectDistPathObj();
print("The aproximated length is: ", direct_dist.computeCost(swaths_path));


print("\n\n####### Tutorial 2.4.1 Path length ######");
path = f2c.Path()
path.appendSwath(swaths_path.at(0), 1);
path.appendSwath(swaths_path.at(1), 1);

path_length = f2c.OBJ_PathLength();
print("The path length is: ", path_length.computeCost(path));





