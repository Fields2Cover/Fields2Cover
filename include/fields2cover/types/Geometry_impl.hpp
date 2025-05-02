//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_GEOMETRY_IMPL_HPP_
#define FIELDS2COVER_TYPES_GEOMETRY_IMPL_HPP_

#include <cpl_conv.h>
#include <geos_c.h>
#include <memory>
#include <vector>
#include <string>


namespace f2c::types {

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry() : data_(
  downCast<T*>(OGRGeometryFactory::createGeometry(R)),
  [](T* f) {OGRGeometryFactory::destroyGeometry(f);}) {}

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(const T& g) : data_(downCast<T*>(g.clone()),
  [](T* f) {OGRGeometryFactory::destroyGeometry(f);}) {}

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(std::shared_ptr<T> g) : data_(g) {}

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(T* g, EmptyDestructor) : data_(g, [](T* f) {}) {}

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(const T* g) : data_(downCast<T*>(g->clone()),
  [](T* f) {OGRGeometryFactory::destroyGeometry(f);}) {}

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(OGRGeometry* g, EmptyDestructor) :
  data_(downCast<T*>(g), [](T* f) {}) {}

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(const OGRGeometry* g) : data_(downCast<T*>(g->clone()),
  [](T* f) {OGRGeometryFactory::destroyGeometry(f);}) {}

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::~Geometry() = default;

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(const Geometry& g) = default;

template <class T, OGRwkbGeometryType R>
Geometry<T, R>::Geometry(Geometry&& g) = default;

template <class T, OGRwkbGeometryType R>
Geometry<T, R>& Geometry<T, R>::operator=(Geometry&& g) = default;

template <class T, OGRwkbGeometryType R>
Geometry<T, R>& Geometry<T, R>::operator=(const Geometry& g) = default;

template <class T, OGRwkbGeometryType R>
std::shared_ptr<T> Geometry<T, R>::operator->() {return data_;}

template <class T, OGRwkbGeometryType R>
std::shared_ptr<const T> Geometry<T, R>::operator->() const {return data_;}

template <class T, OGRwkbGeometryType R>
T* Geometry<T, R>::get() {return data_.get();}

template <class T, OGRwkbGeometryType R>
const T* Geometry<T, R>::get() const {return data_.get();}

template <class T, OGRwkbGeometryType R>
bool Geometry<T, R>::operator!=(const Geometry<T, R>& geom2) const {
  return !geom2->Equals(data_.get());
}

template <class T, OGRwkbGeometryType R>
bool Geometry<T, R>::operator==(const Geometry<T, R>& geom2) const {
  return geom2->Equals(data_.get());
}


template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getDimMinX() const {
  OGREnvelope envelope;
  data_->getEnvelope(&envelope);
  return envelope.MinX;
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getDimMaxX() const {
  OGREnvelope envelope;
  data_->getEnvelope(&envelope);
  return envelope.MaxX;
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getDimMinY() const {
  OGREnvelope envelope;
  data_->getEnvelope(&envelope);
  return envelope.MinY;
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getDimMaxY() const {
  OGREnvelope envelope;
  data_->getEnvelope(&envelope);
  return envelope.MaxY;
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getHeight() const {
  OGREnvelope envelope;
  data_->getEnvelope(&envelope);
  return envelope.MaxY - envelope.MinY;
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getWidth() const {
  OGREnvelope envelope;
  data_->getEnvelope(&envelope);
  return envelope.MaxX - envelope.MinX;
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getMinSafeLength() const {
  OGREnvelope envelope;
  data_->getEnvelope(&envelope);
  return (envelope.MaxX - envelope.MinX) +
    (envelope.MaxY - envelope.MinY);
}

template <class T, OGRwkbGeometryType R>
template <class T2, OGRwkbGeometryType R2>
double Geometry<T, R>::distance(const Geometry<T2, R2>& p) const {
  return data_->Distance(p.get());
}

template <class T, OGRwkbGeometryType R>
template <class T2, OGRwkbGeometryType R2>
bool Geometry<T, R>::disjoint(const Geometry<T2, R2>& geom) const {
  return data_->Disjoint(geom.get());
}

template <class T, OGRwkbGeometryType R>
template <class T2, OGRwkbGeometryType R2>
bool Geometry<T, R>::crosses(const Geometry<T2, R2>& geom) const {
  return data_->Crosses(geom.get());
}

template <class T, OGRwkbGeometryType R>
template <class T2, OGRwkbGeometryType R2>
bool Geometry<T, R>::touches(const Geometry<T2, R2>& geom) const {
  return data_->Touches(geom.get());
}

template <class T, OGRwkbGeometryType R>
template <class T2, OGRwkbGeometryType R2>
bool Geometry<T, R>::within(const Geometry<T2, R2>& geom) const {
  return data_->Within(geom.get());
}

template <class T, OGRwkbGeometryType R>
template <class T2, OGRwkbGeometryType R2>
bool Geometry<T, R>::intersects(const Geometry<T2, R2>& geom) const {
  return data_->Intersects(geom.get());
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::mod_2pi(double val) {
  return mod(val, boost::math::constants::two_pi<double>());
}

template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::getAngContinuity(double prev_val, double val) {
  const auto two_pi = boost::math::constants::two_pi<double>();
  auto val_id = round(prev_val / two_pi);
  auto getNearVal =
    [two_pi] (int i, double value) {return i * two_pi + mod_2pi(value);};
  for (int i = -2; i <= 0; ++i) {
    if (fabs(getNearVal(val_id + i + 1, val) - prev_val) >
      fabs(getNearVal(val_id + i, val) - prev_val)) {
      return getNearVal(val_id + i, val);
    }
  }
  return getNearVal(val_id + 1, val);
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
double Geometry<T, R>::getAngleAvg(double a, double b) {
  const auto pi = boost::math::constants::pi<double>();
  double ma = mod_2pi(a);
  double mb = mod_2pi(b);
  if (abs(mb - ma) < pi) {
    return 0.5 * (ma + mb);
  } else {
    return 0.5 * (ma + mb) + pi;
  }
}

template <class T, OGRwkbGeometryType R>
bool Geometry<T, R>::isEmpty() const {
  return (!data_ || data_->IsEmpty());
}

template <class T, OGRwkbGeometryType R>
std::string Geometry<T, R>::exportToWkt() const {
  char *pszWKT = nullptr;
  data_->exportToWkt(&pszWKT);
  std::string result{pszWKT};
  CPLFree(pszWKT);
  return result;
}

template <class T, OGRwkbGeometryType R>
std::string Geometry<T, R>::exportToGML() const {
  char *s = data_->exportToGML();
  std::string str(s);
  CPLFree(s);
  return str;
}

template <class T, OGRwkbGeometryType R>
std::string Geometry<T, R>::exportToKML() const {
  char *s = data_->exportToKML();
  std::string str(s);
  CPLFree(s);
  return str;
}

template <class T, OGRwkbGeometryType R>
std::string Geometry<T, R>::exportToJson() const {
  char *s = data_->exportToJson();
  std::string str(s);
  CPLFree(s);
  return str;
}

template <class T, OGRwkbGeometryType R>
void Geometry<T, R>::importFromWkt(const std::string& text) {
  auto char_text = text.c_str();
  data_->importFromWkt(&char_text);
}

// ###############################
// Code extracted from:
// https://github.com/OSGeo/gdal/blob/b0aa6065a39b252cb8306e9c2e2535d6dda0fb55/port/cpl_conv.h#L397
template <class T, OGRwkbGeometryType R>
template <typename To, typename From>
inline To Geometry<T, R>::downCast(From *f) const {
  static_assert(
    (std::is_base_of<From, typename std::remove_pointer<To>::type>::value),
    "target type not derived from source type");
  CPLAssert(f == nullptr || dynamic_cast<To>(f) != nullptr);
  return static_cast<To>(f);
}
// ###############################

template <class T, OGRwkbGeometryType R>
template<typename RetType>
RetType Geometry<T, R>::destroyResGeom(OGRGeometry* g) {
  RetType res(g);
  OGRGeometryFactory::destroyGeometry(g);
  return res;
}

// ###############################
// Code extracted from:
// https://github.com/OSGeo/gdal/blob/717dcc0eed252e2f78c142b1f7866e49c5511224/ogr/ogrgeometry.cpp#L4309
template <class T, OGRwkbGeometryType R>
OGRGeometry* Geometry<T, R>::OGRBuffer(double dfDist, int side) const {
  OGRGeometry *poOGRProduct = nullptr;

  GEOSContextHandle_t hGEOSCtxt = OGRGeometry::createGEOSContext();
  GEOSGeom hGeosGeom = this->data_->exportToGEOS(hGEOSCtxt);
  if (hGeosGeom != nullptr) {
    GEOSBufferParams* hBufParams = GEOSBufferParams_create_r(hGEOSCtxt);
    GEOSBufferParams_setEndCapStyle_r(hGEOSCtxt, hBufParams,
        GEOSBufCapStyles::GEOSBUF_CAP_FLAT);
    GEOSBufferParams_setJoinStyle_r(hGEOSCtxt, hBufParams,
        GEOSBufJoinStyles::GEOSBUF_JOIN_MITRE);
    GEOSBufferParams_setSingleSided_r(hGEOSCtxt, hBufParams, side != 0);

    GEOSGeom hGeosProduct = GEOSBufferWithParams_r(hGEOSCtxt, hGeosGeom,
        hBufParams, side >= 0 ? dfDist : -dfDist);
    GEOSGeom_destroy_r(hGEOSCtxt, hGeosGeom);
    GEOSBufferParams_destroy_r(hGEOSCtxt, hBufParams);

    poOGRProduct = buildGeometryFromGEOS(hGEOSCtxt, hGeosProduct,
        this->data_.get(), nullptr);
  }
  OGRGeometry::freeGEOSContext(hGEOSCtxt);
  return poOGRProduct;
}
////////////////////////

// ###############################
// Code extracted from:
// https://github.com/OSGeo/gdal/blob/717dcc0eed252e2f78c142b1f7866e49c5511224/ogr/ogrgeometry.cpp#L4309
template <class T, OGRwkbGeometryType R>
OGRGeometry* Geometry<T, R>::OGRGeometryRebuildCurves(
    const OGRGeometry *poGeom, const OGRGeometry *poOtherGeom,
    OGRGeometry *poOGRProduct) const {
  if (poOGRProduct != nullptr &&
      wkbFlatten(poOGRProduct->getGeometryType()) != wkbPoint &&
      (poGeom->hasCurveGeometry(true) ||
      (poOtherGeom && poOtherGeom->hasCurveGeometry(true)))) {
    OGRGeometry *poCurveGeom = poOGRProduct->getCurveGeometry();
    delete poOGRProduct;
    return poCurveGeom;
  }
  return poOGRProduct;
}
template <class T, OGRwkbGeometryType R>
OGRGeometry* Geometry<T, R>::buildGeometryFromGEOS(
    GEOSContextHandle_t hGEOSCtxt, GEOSGeom hGeosProduct,
    const OGRGeometry *poSelf, const OGRGeometry *poOtherGeom) const {
  OGRGeometry *poOGRProduct = nullptr;
  if (hGeosProduct != nullptr) {
    poOGRProduct = OGRGeometryFactory::createFromGEOS(hGEOSCtxt, hGeosProduct);
    if (poOGRProduct != nullptr &&
        poSelf->getSpatialReference() != nullptr &&
        (poOtherGeom == nullptr ||
          (poOtherGeom->getSpatialReference() != nullptr &&
        poOtherGeom->getSpatialReference()->IsSame(
          poSelf->getSpatialReference())))) {
      poOGRProduct->assignSpatialReference(poSelf->getSpatialReference());
    }
    poOGRProduct = OGRGeometryRebuildCurves(poSelf, poOtherGeom, poOGRProduct);
    GEOSGeom_destroy_r(hGEOSCtxt, hGeosProduct);
  }
  return poOGRProduct;
}
// ###############################


template <class T, OGRwkbGeometryType R>
double Geometry<T, R>::mod(double a, double b) {
  return fmod(fmod(a, b) + b, b);
}


}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_GEOMETRY_IMPL_HPP_
