//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/overlaps.h"

namespace f2c::obj {

double Overlaps::computeCost(const F2CCell& poly, const F2CSwaths& swaths)
    const {
  return computeCost(F2CCells(poly), swaths);
}

double Overlaps::computeCost(const F2CCells& poly, const F2CSwaths& swaths)
    const {
  double area_overlaped {0.0};
  std::vector<F2CCells> areas;
  for (int i = swaths.size() - 1; i >= 0; --i) {
    areas.emplace_back(swaths[i].computeAreaCovered(poly));
    for (int j = i + 1; j < areas.size(); ++j) {
      if (areas[i].Intersects(areas[j])) {
        area_overlaped += areas[i].Intersection(areas[j]).getArea();
      }
    }
  }
  return area_overlaped / poly.getArea();
}

double Overlaps::computeCost(const F2CCells& poly) const {
  double area_overlaped {0.0};
  for (int i = poly.size() - 1; i >= 0; --i) {
    for (int j = i + 1; j < poly.size(); ++j) {
      area_overlaped +=
        poly.getGeometry(i).Intersection(poly.getGeometry(j)).getArea();
    }
  }
  return area_overlaped / poly.getArea();
}

}  // namespace f2c::obj

