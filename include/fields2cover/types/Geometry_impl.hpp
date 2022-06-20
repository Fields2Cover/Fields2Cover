//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           MIT License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_GEOMETRY_IMPL_HPP_
#define FIELDS2COVER_TYPES_GEOMETRY_IMPL_HPP_

#include <memory>
#include <vector>
#include <string>

namespace f2c {
namespace types {

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry() : data(
  static_cast<T*>(OGRGeometryFactory::createGeometry(R)),
  [](T* f) {OGRGeometryFactory::destroyGeometry(f);}) {}

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(const T& g) : data(static_cast<T*>(g.clone()),
  [](T* f) {OGRGeometryFactory::destroyGeometry(f);}) {}

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(std::shared_ptr<T> g) : data(g) {}

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(T* g, EmptyDestructor) : data(g, [](T* f) {}) {}

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(const T* g) : data(static_cast<T*>(g->clone()),
  [](T* f) {OGRGeometryFactory::destroyGeometry(f);}) {}

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::~Geometry() = default;

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(const Geometry& g) = default;

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(Geometry&& g) = default;

template <class T, OGRwkbGeometryType R>
typename Geometry<T, R>::Geometry& Geometry<T, R>::operator=(
    Geometry&& g) = default;

template <class T, OGRwkbGeometryType R>
typename Geometry<T, R>::Geometry& Geometry<T, R>::operator=(
    const Geometry& g) = default;

template <class T, OGRwkbGeometryType R>
std::shared_ptr<T> Geometry<T, R>::operator->() {return data;}

template <class T, OGRwkbGeometryType R>
std::shared_ptr<T> Geometry<T, R>::operator->() const {return data;}

template <class T, OGRwkbGeometryType R>
T* Geometry<T, R>::get() const {return data.get();}

template <class T, OGRwkbGeometryType R>
bool Geometry<T, R>::operator!=(const Geometry<T, R>& geom2) const {
  return !geom2->Equals(data.get());
}

template <class T, OGRwkbGeometryType R>
bool Geometry<T, R>::operator==(const Geometry<T, R>& geom2) const {
  return geom2->Equals(data.get());
}


template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getDimMinX() const {
  OGREnvelope envelope;
  data->getEnvelope(&envelope);
  return envelope.MinX;
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getDimMaxX() const {
  OGREnvelope envelope;
  data->getEnvelope(&envelope);
  return envelope.MaxX;
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getDimMinY() const {
  OGREnvelope envelope;
  data->getEnvelope(&envelope);
  return envelope.MinY;
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getDimMaxY() const {
  OGREnvelope envelope;
  data->getEnvelope(&envelope);
  return envelope.MaxY;
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getHeight() const {
  OGREnvelope envelope;
  data->getEnvelope(&envelope);
  return envelope.MaxY - envelope.MinY;
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getWidth() const {
  OGREnvelope envelope;
  data->getEnvelope(&envelope);
  return envelope.MaxX - envelope.MinX;
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getMinSafeLength() const {
  OGREnvelope envelope;
  data->getEnvelope(&envelope);
  return (envelope.MaxX - envelope.MinX) +
    (envelope.MaxY - envelope.MinY);
}

template <class T, OGRwkbGeometryType R>
template <class T2, OGRwkbGeometryType R2>
double Geometry<T, R>::Distance(const Geometry<T2, R2>& p) const {
  return data->Distance(p.get());
}

template <class T, OGRwkbGeometryType R>
template <class T2, OGRwkbGeometryType R2>
bool Geometry<T, R>::Disjoint(const Geometry<T2, R2>& geom) const {
  return data->Disjoint(geom.get());
}

template <class T, OGRwkbGeometryType R>
bool Geometry<T, R>::Disjoint(const OGRGeometry* geom) const {
  return data->Disjoint(geom);
}

template <class T, OGRwkbGeometryType R>
template <class T2, OGRwkbGeometryType R2>
bool Geometry<T, R>::Crosses(const Geometry<T2, R2>& geom) const {
  return data->Crosses(geom.get());
}

template <class T, OGRwkbGeometryType R>
bool Geometry<T, R>::Crosses(const OGRGeometry* geom) const {
  return data->Crosses(geom);
}

template <class T, OGRwkbGeometryType R>
template <class T2, OGRwkbGeometryType R2>
bool Geometry<T, R>::Touches(const Geometry<T2, R2>& geom) const {
  return data->Touches(geom.get());
}

template <class T, OGRwkbGeometryType R>
bool Geometry<T, R>::Touches(const OGRGeometry* geom) const {
  return data->Touches(geom);
}

template <class T, OGRwkbGeometryType R>
template <class T2, OGRwkbGeometryType R2>
bool Geometry<T, R>::Within(const Geometry<T2, R2>& geom) const {
  return data->Within(geom.get());
}

template <class T, OGRwkbGeometryType R>
bool Geometry<T, R>::Within(const OGRGeometry* geom) const {
  return data->Within(geom);
}

template <class T, OGRwkbGeometryType R>
template <class T2, OGRwkbGeometryType R2>
bool Geometry<T, R>::Intersects(const Geometry<T2, R2>& geom) const {
  return data->Intersects(geom.get());
}

template <class T, OGRwkbGeometryType R>
bool Geometry<T, R>::Intersects(const OGRGeometry* geom) const {
  return data->Intersects(geom);
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::mod_2pi(double val) {
  if (val >= 0) {
    return fmod(val, boost::math::constants::two_pi<double>());
  }
  return boost::math::constants::two_pi<double>()
    - fmod(-val, boost::math::constants::two_pi<double>());
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getAngContinuity(double prev_val, double val) {
  const auto two_pi = boost::math::constants::two_pi<double>();
  auto val_id = round(prev_val / two_pi);
  auto getNearVal =
    [two_pi] (int i, double val) {return i * two_pi + mod_2pi(val);};
  for (int i = -3; i <= 3; ++i) {
    if (fabs(getNearVal(val_id + i + 1, val) - prev_val) >
      fabs(getNearVal(val_id + i, val) - prev_val)) {
      return getNearVal(val_id + i, val);
    }
  }
  return getNearVal(val_id + 4, val);
}

template <class T, OGRwkbGeometryType R>
std::vector<double> Geometry<T, R>::getAngContinuity(
    const std::vector<double>& val) {
  std::vector<double> res {val[0]};
  for (size_t i = 1; i < val.size(); ++i) {
    res.emplace_back(getAngContinuity(res[i - 1], val[i]));
  }
  return res;
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getAngleDiffAbs(double a, double b) {
  const auto pi = boost::math::constants::pi<double>();
  return pi - fabs(mod_2pi(b - a) - pi);
}

template <class T, OGRwkbGeometryType R>
bool Geometry<T, R>::isEmpty() const {
  return (!data || data->IsEmpty());
}

template <class T, OGRwkbGeometryType R>
std::string Geometry<T, R>::exportToWkt() const {
  return data->exportToWkt();
}

template <class T, OGRwkbGeometryType R>
std::string Geometry<T, R>::exportToGML() const {
  return data->exportToGML();
}

template <class T, OGRwkbGeometryType R>
std::string Geometry<T, R>::exportToKML() const {
  return data->exportToKML();
}

template <class T, OGRwkbGeometryType R>
std::string Geometry<T, R>::exportToJson() const {
  return data->exportToJson();
}

template <class T, OGRwkbGeometryType R>
void Geometry<T, R>::importFromWkt(const std::string& text) {
  auto char_text = text.c_str();
  data->importFromWkt(&char_text);
}


}  // namespace types
}  // namespace f2c

#endif  // FIELDS2COVER_TYPES_GEOMETRY_IMPL_HPP_
