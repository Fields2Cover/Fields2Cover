//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include "fields2cover.h"
#include <iostream>

int main() {
  std::cout << "####### Tutorial 2.1.1 Field Coverage ######" << std::endl;
  double width {2.0};
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 1.0), F2CPoint(4.0, 1.0)}), width);
  F2CSwath swath2(F2CLineString({F2CPoint(0.0, 3.0), F2CPoint(4.0, 3.0)}), width);
  F2CSwath swath3(F2CLineString({F2CPoint(0.0, 2.0), F2CPoint(4.0, 2.0)}), width);

  F2CCells field(F2CCell(F2CLinearRing(
    {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,4), F2CPoint(0,4), F2CPoint(0,0)})));

  f2c::obj::FieldCoverage field_cov;
  std::cout << "The field coverage with swath1 is "
    << field_cov.computeCost(field, F2CSwaths({swath1})) << " and with all of the swaths "
    << field_cov.computeCost(field, F2CSwaths({swath1, swath2, swath3})) << std::endl;
  std::cout << "The field coverage with sign for all of the swaths is "
    << field_cov.computeCostWithMinimizingSign(
        field, F2CSwaths({swath1, swath2, swath3})) <<std::endl;

  std::cout << std::endl;
  std::cout << "####### Tutorial 2.1.2 Number of swaths ######" << std::endl;
  f2c::obj::NSwath n_swaths;
  std::cout << "The number of swaths with swath1 is "
    << n_swaths.computeCost(F2CSwaths({swath1})) << " and with all of the swaths "
    << n_swaths.computeCost(field, F2CSwaths({swath1, swath2, swath3})) <<std::endl;

  std::cout << std::endl;
  std::cout << "####### Tutorial 2.1.3 Overlap ######" << std::endl;
  f2c::obj::Overlaps overlaps;
  std::cout << "The field overlapping with swath1 is "
    << overlaps.computeCost(field, F2CSwaths({swath1})) << " and with all of the swaths "
    << overlaps.computeCost(field, F2CSwaths({swath1, swath2, swath3})) << std::endl;

  std::cout << std::endl;
  std::cout << "####### Tutorial 2.1.4 Swath length ######" << std::endl;
  f2c::obj::SwathLength swath_length;
  std::cout << "The swath length with swath1 is "
    << swath_length.computeCost(field, F2CSwaths({swath1})) << " and with all of the swaths "
    << swath_length.computeCost(field, F2CSwaths({swath1, swath2, swath3})) << std::endl;

  std::cout << std::endl << std::endl;
  std::cout << "####### Tutorial 2.2.1 Distance with turns ######" << std::endl;
  F2CSwaths swaths_path({
      F2CSwath(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)})),
      F2CSwath(F2CLineString({F2CPoint(1.0, 1.0), F2CPoint(1.0, 0.0)}))});
  F2CRobot robot(2.0, 3.0);
  robot.setMinRadius(0.5);
  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves> complete_length(robot);
  std::cout << "The complete length is: " <<
    complete_length.computeCost(swaths_path) <<
    " =~= " << 1 + 1 + M_PI/2.0 << std::endl;

  std::cout << std::endl;
  std::cout << "####### Tutorial 2.2.2 Direct distance without turns ######" << std::endl;
  f2c::obj::DirectDistPathObj direct_dist;
  std::cout << "The aproximated length is: " <<
    direct_dist.computeCost(swaths_path) << std::endl;

  return 0;
}

