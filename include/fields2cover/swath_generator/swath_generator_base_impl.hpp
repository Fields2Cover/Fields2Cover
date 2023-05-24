//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           MIT License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_IMPL_HPP_
#define FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_IMPL_HPP_

namespace f2c::sg {

template <typename T>
F2CSwathsByCells SwathGeneratorBase<T>::generateBestSwaths(
    f2c::obj::SGObjective& obj,
    double op_width, const F2CCells& polys) {
  F2CSwathsByCells swaths;
  for (auto&& p : polys) {
    swaths.emplace_back(generateBestSwaths(obj, op_width, p));
  }
  return swaths;
}

template <typename T>
F2CSwathsByCells SwathGeneratorBase<T>::generateSwaths(double angle,
    double op_width, const F2CCells& polys) {
  F2CSwathsByCells swaths;
  for (auto&& p : polys) {
    swaths.emplace_back(generateSwaths(angle, op_width, p));
  }
  return swaths;
}

template <typename T>
F2CSwaths SwathGeneratorBase<T>::generateSwaths(double angle,
    double op_width, const F2CCell& poly) {
  auto rot_poly {F2CPoint(0.0, 0.0).rotateFromPoint(angle, poly)};

  double field_width {rot_poly.getWidth()};
  int n_swaths {static_cast<int>(ceil(field_width / op_width))};
  auto seed_curve = rot_poly.getStraightLongCurve(
      F2CPoint(rot_poly.getDimMinX(), rot_poly.getDimMinY()),
      boost::math::constants::half_pi<double>());

  F2CSwaths swaths;
  for (int i = 0; i < n_swaths; ++i) {
    auto path = F2CPoint(0.0, 0.0).rotateFromPoint(-angle,
        seed_curve + F2CPoint(op_width, 0.0) * (i + 0.5));
    swaths.append(path, poly, op_width);
  }

  if (allow_overlap && field_width - ((n_swaths - 1) * op_width) < op_width / 2) {
    auto path = F2CPoint(0.0, 0.0).rotateFromPoint(-angle,
                                                    seed_curve + F2CPoint(field_width - op_width / 2, 0.0));
    swaths.append(path, poly, op_width);
  }
  return swaths;
}

}  // namespace f2c::sg

#endif  // FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_IMPL_HPP_
