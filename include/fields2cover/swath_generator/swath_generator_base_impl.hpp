//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           MIT License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_IMPL_HPP_
#define FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_IMPL_HPP_

namespace f2c {
namespace sg {

template <typename T>
F2CSwathsByCells SwathGeneratorBase<T>::generateBestSwaths(
    double _op_width, const F2CCells& _polys) {
  F2CSwathsByCells swaths;
  for (auto&& p : _polys) {
    swaths.emplace_back(generateBestSwaths(_op_width, p));
  }
  return swaths;
}

template <typename T>
F2CSwathsByCells SwathGeneratorBase<T>::generateSwaths(double _angle,
    double _op_width, const F2CCells& _polys) {
  F2CSwathsByCells swaths;
  for (auto&& p : _polys) {
    swaths.emplace_back(generateSwaths(_angle, _op_width, p));
  }
  return swaths;
}

template <typename T>
F2CSwaths SwathGeneratorBase<T>::generateSwaths(double _angle,
    double _op_width, const F2CCell& _poly) {
  auto rot_poly {F2CPoint(0.0, 0.0).rotateFromPoint(_angle, _poly)};

  double field_width {rot_poly.getWidth()};
  int n_swaths {static_cast<int>(ceil(field_width / _op_width))};
  auto seed_curve = rot_poly.getStraightLongCurve(
      F2CPoint(rot_poly.getDimMinX(), rot_poly.getDimMinY()),
      boost::math::constants::half_pi<double>());

  F2CSwaths swaths;
  for (int i = 0; i < n_swaths; ++i) {
    auto path = F2CPoint(0.0, 0.0).rotateFromPoint(-_angle,
        seed_curve + F2CPoint(_op_width, 0.0) * (i + 0.5));
    swaths.append(path, _poly, _op_width);
  }
  return swaths;
}

template <typename T>
double SwathGeneratorBase<T>::getBestAngle() const {
  return this->best_angle;
}

}  // namespace sg
}  // namespace f2c

#endif  // FIELDS2COVER_SWATH_GENERATOR_SWATH_GENERATOR_BASE_IMPL_HPP_
