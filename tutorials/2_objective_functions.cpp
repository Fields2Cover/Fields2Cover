//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================


#include "fields2cover.h"
#include <iostream>

int main() {
  std::cout << "####### Tutorial 2.1.1 Remaining Area ######" << std::endl;
  F2CCells total_field(F2CCell(F2CLinearRing(
    {F2CPoint(-2,-2), F2CPoint(6,-2), F2CPoint(6,6), F2CPoint(-2,6), F2CPoint(-2,-2)})));
  F2CCells field(F2CCell(F2CLinearRing(
    {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,4), F2CPoint(0,4), F2CPoint(0,0)})));


  f2c::obj::RemArea rem_area;
  std::cout << "The remaining area is "
    << rem_area.computeCost(total_field, field) << std::endl;
  std::cout << "The remaining area with sign is "
    << rem_area.computeCostWithMinimizingSign(total_field, field) <<std::endl;
  std::cout << std::endl << std::endl;

  std::cout << "####### Tutorial 2.2.1 Field Coverage ######" << std::endl;
  double width {2.0};
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 1.0), F2CPoint(4.0, 1.0)}), width);
  F2CSwath swath2(F2CLineString({F2CPoint(0.0, 3.0), F2CPoint(4.0, 3.0)}), width);
  F2CSwath swath3(F2CLineString({F2CPoint(0.0, 2.0), F2CPoint(4.0, 2.0)}), width);


  f2c::obj::FieldCoverage field_cov;
  std::cout << "The field coverage with swath1 is "
    << field_cov.computeCost(field, F2CSwaths({swath1})) << " and with all of the swaths "
    << field_cov.computeCost(field, F2CSwaths({swath1, swath2, swath3})) << std::endl;
  std::cout << "The field coverage with sign for all of the swaths is "
    << field_cov.computeCostWithMinimizingSign(
        field, F2CSwaths({swath1, swath2, swath3})) <<std::endl;

  std::cout << std::endl;
  std::cout << "####### Tutorial 2.2.2 Number of swaths ######" << std::endl;
  f2c::obj::NSwath n_swaths;
  std::cout << "The number of swaths with swath1 is "
    << n_swaths.computeCost(F2CSwaths({swath1})) << " and with all of the swaths "
    << n_swaths.computeCost(field, F2CSwaths({swath1, swath2, swath3})) <<std::endl;

  f2c::obj::NSwathModified n_swaths_mod;
  std::cout << "The number of swaths with swath1 is "
    << n_swaths_mod.computeCost(F2CSwaths({swath1})) << " and with all of the swaths "
    << n_swaths_mod.computeCost(field, F2CSwaths({swath1, swath2, swath3})) <<std::endl;


  std::cout << std::endl;
  std::cout << "####### Tutorial 2.2.3 Overlap ######" << std::endl;
  f2c::obj::Overlaps overlaps;
  std::cout << "The field overlapping with swath1 is "
    << overlaps.computeCost(field, F2CSwaths({swath1})) << " and with all of the swaths "
    << overlaps.computeCost(field, F2CSwaths({swath1, swath2, swath3})) << std::endl;

  std::cout << std::endl;
  std::cout << "####### Tutorial 2.2.4 Swath length ######" << std::endl;
  f2c::obj::SwathLength swath_length;
  std::cout << "The swath length with swath1 is "
    << swath_length.computeCost(field, F2CSwaths({swath1})) << " and with all of the swaths "
    << swath_length.computeCost(field, F2CSwaths({swath1, swath2, swath3})) << std::endl;

  std::cout << std::endl << std::endl;
  std::cout << "####### Tutorial 2.3.1 Distance with turns ######" << std::endl;
  F2CSwaths swaths_path({
      F2CSwath(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)})),
      F2CSwath(F2CLineString({F2CPoint(1.0, 1.0), F2CPoint(1.0, 0.0)}))});
  F2CRobot robot(2.0, 3.0);
  robot.setMinTurningRadius(0.5);
  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves> complete_length(robot);
  std::cout << "The complete length is: " <<
    complete_length.computeCost(swaths_path) <<
    " =~= " << 1 + 1 + M_PI/2.0 << std::endl;

  std::cout << std::endl;
  std::cout << "####### Tutorial 2.3.2 Direct distance without turns ######" << std::endl;
  f2c::obj::DirectDistPathObj direct_dist;
  std::cout << "The aproximated length is: " <<
    direct_dist.computeCost(swaths_path) << std::endl;

  std::cout << std::endl << std::endl;
  std::cout << "####### Tutorial 2.4.1 Path length ######" << std::endl;
  F2CPath path;
  path.appendSwath(swaths_path.at(0), 1);
  path.appendSwath(swaths_path.at(1), 1);

  f2c::obj::PathLength path_length;
  std::cout << "The path length is: " <<
    path_length.computeCost(path) << std::endl;


  return 0;
}

