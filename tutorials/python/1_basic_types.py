#=============================================================================
#    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                     Author: Gonzalo Mier
#                        BSD-3 License
#=============================================================================

import fields2cover as f2c

print("####### Tutorial 1.1 Initialize a Point ######")
# Create a point with x and y
p1 = f2c.Point(1.2, 3.4)
print("Point 1: ", p1)

# Create a point with x, y and z
p2 = f2c.Point(9.8, 7.6, 5.4);
print("Point 2: ", p2);

# Create a point using a OGRPoint
from osgeo import ogr
ogrpoint = ogr.Geometry(ogr.wkbPoint)
ogrpoint.AddPoint(11, 22)
p3 = f2c.Point()
p3.importFromWkt(ogrpoint.ExportToWkt())
print("Point 3: ", p3)


# Create an empty point and set its components
p4 = f2c.Point()
p4.setX(3.0)
p4.setZ(-1.0)
print("Point 4: ", p4,
        ". Its components are: {x: ", p4.getX(),
        ", y: ", p4.getY(),
        ", z: ", p4.getZ(), "}")

# Create an empty point and import its components
p5 = f2c.Point()
p5.importFromWkt("POINT (0 4 4)")
print("Point 5: ", p5)


print("\n\n####### Tutorial 1.2 Basic types are shared pointers ######")
# Difference between cloning and copying a point
old_p = f2c.Point(1, 2)
cloned_p = old_p.clone()
print("The old point is ", old_p)
cloned_p *= 5.0
print("Old point is: ", old_p, " and cloned point is: ", cloned_p)

copy_p = old_p
copy_p *= 5.5
print("Old point is: ", old_p, " and copied point is: ", copy_p)

print("\n\n####### Tutorial 1.3 Initialize a LineString ######")

line1 = f2c.LineString()
line1.addPoint(3,0)
line1.addPoint(p5)
print("Length of line 1: ", line1.getLength())

line2 = f2c.LineString();
[line2.addPoint(p) for p in [f2c.Point(1, 0), f2c.Point(1, 1), f2c.Point(0, 1)]];
print("Length of line 2: ", line2.getLength());


print("\n\n####### Tutorial 1.4 Initialize a LinearRing ######")

ring = f2c.LinearRing();
[ring.addPoint(p) for p in [f2c.Point(1,1), f2c.Point(1,2), f2c.Point(2,2), f2c.Point(1,1)]];
print("Area of the ring: ", ring.getArea())

print("\n\n####### Tutorial 1.5 Initializing other collections ######")
lines = f2c.MultiLineString();
lines.addGeometry(line1);
lines.addGeometry(line2);
print("Lines have length: ", end="")
for i in range(lines.size()):
  print(lines.getGeometry(i).getLength(), end = ", ")
print("\n")

outter_ring = f2c.LinearRing();
[outter_ring.addGeometry(p) for p in [  \
    f2c.Point(0, 0), f2c.Point(2, 0), f2c.Point(2, 2), f2c.Point(0, 2), f2c.Point(0, 0)]];
inner_ring = f2c.LinearRing();
[inner_ring.addGeometry(p) for p in [  \
    f2c.Point(0.5, 0.5), f2c.Point(1.5, 0.5), f2c.Point(1.5, 1.5),  \
    f2c.Point(0.5, 1.5), f2c.Point(0.5, 0.5)]];
cell = f2c.Cell();
cell.addRing(outter_ring);
cell.addRing(inner_ring);
print("The area of the cell is: ", cell.getArea(), "\n");

cells = f2c.Cells();
cells.addGeometry(cell);
print("The area of the cells is: ", cells.getArea(), "\n\n")

points = f2c.MultiPoint();
[points.addGeometry(p) for p in [f2c.Point(1, 2), f2c.Point(3, 4)]];

print("Points contains ", points.size(), " points.");
points.addPoint(5, 6);
print("Points contains ", points.size(), " points.");
points.addPoint(p5);
print("Points contains ", points.size(), " points.");


print("\n\n####### Tutorial 1.6 Accessing elements in collections ######")

p_0 = points.getGeometry(0);
print("First point in points: ", p_0, "\n")

p_0 *= 1e5;
print("Modified p_0: ", p_0);
print("First point in points without modification: ", points.getGeometry(0));
points.setGeometry(0, p_0);
print("Modified first point in points: ", points.getGeometry(0));


print("\n\n####### Tutorial 1.9 Visualizing Fields2Cover data ######")

f2c.Visualizer.figure(100);
f2c.Visualizer.plot(lines);
f2c.Visualizer.show();

f2c.Visualizer.figure(101);
f2c.Visualizer.plot(lines);
f2c.Visualizer.save("Tutorial_image");


