//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include "fields2cover.h"
#include <iostream>

int main() {
  std::cout << "####### Tutorial 1.1 Initialize a Point ######" << std::endl;
  // Create a point with x and y
  F2CPoint p1 (1.2, 3.4);
  std::cout << "Point 1: " << p1 << std::endl;

  // Create a point with x, y and z
  F2CPoint p2 (9.8, 7.6, 5.4);
  std::cout << "Point 2: " << p2 << std::endl;

  // Create a point using a OGRPoint
  F2CPoint p3 (OGRPoint(11, 22));
  std::cout << "Point 3: " << p3 << std::endl;


  // Create an empty point and set its components
  F2CPoint p4;
  p4.setX(3.0);
  p4.setZ(-1.0);
  std::cout << "Point 4: " << p4;
  std::cout << ". Its components are: {x: " << p4.getX();
  std::cout << ", y: " << p4.getY();
  std::cout << ", z: " << p4.getZ() << "}\n";

  // Create an empty point and import its components
  F2CPoint p5;
  p5.importFromWkt("POINT (0 4 4)");
  std::cout << "Point 5: " << p5 << std::endl;



  std::cout << std::endl << std::endl;
  std::cout << "####### Tutorial 1.2 Basic types are shared pointers ######"
    << std::endl;

  // Access to pointers
  std::cout << "Access to OGRPoints: " << p4->Distance(p5.get()) << std::endl;
  std::cout << "Without accessing: " << p4.distance(p5) << std::endl;

  std::cout << std::endl;

  std::cout << std::endl << std::endl;
  std::cout << "####### Tutorial 1.3 Initialize a F2CLineString ######"
    << std::endl;

  F2CLineString line1;
  line1.addPoint(3,0);
  line1.addPoint(p5);
  std::cout << "Length of line 1: " << line1.length() << std::endl;

  F2CLineString line2({F2CPoint(1, 0), F2CPoint(1, 1), F2CPoint(0, 1)});
  std::cout << "Length of line 2: " << line2.length() << std::endl;


  std::cout << std::endl << std::endl;
  std::cout << "####### Tutorial 1.4 Initialize a F2CLinearRing ######"
    << std::endl;

  F2CLinearRing ring{F2CPoint(1,1), F2CPoint(1,2), F2CPoint(2,2), F2CPoint(1,1)};
  std::cout << "Area of the ring: " << ring.area() << std::endl;

  std::cout << std::endl << std::endl;
  std::cout << "####### Tutorial 1.5 Initializing other collections ######"
    << std::endl;

  F2CMultiLineString lines;
  lines.addGeometry(line1);
  lines.addGeometry(line2);
  std::cout << "Lines have length: ";
  for (auto line : lines) {
    std::cout << line.length() << ", ";
  }
  std::cout << std::endl << std::endl;

  F2CLinearRing outter_ring{
    F2CPoint(0, 0), F2CPoint(2, 0),F2CPoint(2, 2), F2CPoint(0, 2), F2CPoint(0, 0)};
  F2CLinearRing inner_ring{
    F2CPoint(0.5, 0.5), F2CPoint(1.5, 0.5), F2CPoint(1.5, 1.5),
    F2CPoint(0.5, 1.5), F2CPoint(0.5, 0.5)};
  F2CCell cell;
  cell.addRing(outter_ring);
  cell.addRing(inner_ring);
  std::cout << "The area of the cell is: " << cell.area()
    << std::endl << std::endl;
  F2CCells cells;
  cells.addGeometry(cell);
  std::cout << "The area of the cells is: " << cells.area() << std::endl;

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

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(lines);
  //f2c::Visualizer::show();
  f2c::Visualizer::save("Tutorial_image.png");

  return 0;
}

