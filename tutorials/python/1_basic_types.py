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

  
print()
print()
print("####### Tutorial 1.2 Basic types are shared pointers ######")
# Access to pointers
# Difference between cloning and copying a point
old_p = f2c.Point(1, 2)
cloned_p = old_p.clone()
print("The old point is ", old_p)
cloned_p *= 5.0
print("Old point is: ", old_p, " and cloned point is: ", cloned_p)
copy_p = old_p

# operator*= NOT WORKING
copy_p *= 5.5
print("Old point is: ", old_p, " and copied point is: ", copy_p)

print()
print()
print("####### Tutorial 1.3 Initialize a F2CLineString ######")

line1 = f2c.LineString()
line1.addPoint(3,0)
line1.addPoint(p5)
print("Length of line 1: ", line1.getLength())

# NOT WORKING
# line2 = f2c.LineString([f2c.Point(1, 0), f2c.Point(1, 1), f2c.Point(0, 1)]);
# print("Length of line 2: ", line2.getLength());


print()
print()
print("####### Tutorial 1.4 Initialize a F2CLinearRing ######")

# NOT WORKING
# ring = f2c.LinearRing(F2CPoint(1,1), F2CPoint(1,2), F2CPoint(2,2), F2CPoint(1,1));
# print("Area of the ring: ", ring.getArea())

print()
print()
print("####### Tutorial 1.5 Initializing other collections ######")
lines = f2c.MultiLineString()
lines.addGeometry(line1);
"""
lines.addGeometry(line2);
print("Lines have length: ", end="")
for line in lines:
  print(line.getLength(), end = ", ")
print()
print()

  F2CLinearRing outter_ring{
    F2CPoint(0, 0), F2CPoint(2, 0),F2CPoint(2, 2), F2CPoint(0, 2), F2CPoint(0, 0)};
  F2CLinearRing inner_ring{
    F2CPoint(0.5, 0.5), F2CPoint(1.5, 0.5), F2CPoint(1.5, 1.5),
    F2CPoint(0.5, 1.5), F2CPoint(0.5, 0.5)};
  F2CCell cell;
  cell.addRing(outter_ring);
  cell.addRing(inner_ring);
  std::cout << "The area of the cell is: " << cell.getArea()
    << std::endl << std::endl;
  F2CCells cells;
  cells.addGeometry(cell);
  std::cout << "The area of the cells is: " << cells.getArea() << std::endl;

  std::cout << std::endl << std::endl;
  F2CMultiPoint points {F2CPoint(1, 2), F2CPoint(3, 4)};
  std::cout << "Points contains " << points.size() << " points." << std::endl;
  points.addPoint(5, 6);
  std::cout << "Points contains " << points.size() << " points." << std::endl;
  points.addPoint(p5);
  std::cout << "Points contains " << points.size() << " points." << std::endl;


  std::cout << std::endl << std::endl;
  std::cout << "####### Tutorial 1.6 Accessing elements in collections ######"
    << std::endl;

  F2CPoint p_0 = points.getGeometry(0);
  std::cout << "First point in points: " << p_0 << std::endl;
  std::cout << std::endl;

  p_0 *= 1e5;
  std::cout << "Modified p_0: " << p_0 << std::endl;
  std::cout << "First point in points without modification: "
    << points.getGeometry(0) << std::endl;
  points.setGeometry(0, p_0);
  std::cout << "Modified first point in points: " << points.getGeometry(0) << std::endl;

  
  std::cout << std::endl << std::endl;
  std::cout << "####### Tutorial 1.9 Visualizing Fields2Cover data ######"
    << std::endl;

  f2c::Visualizer::figure(100);
  f2c::Visualizer::plot(lines);
  //f2c::Visualizer::show();
  f2c::Visualizer::save("Tutorial_image");

  return 0;
}

"""
