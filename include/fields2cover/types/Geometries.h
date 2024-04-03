//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_GEOMETRIES_H_
#define FIELDS2COVER_TYPES_GEOMETRIES_H_

#include <gdal/ogr_geometry.h>
#include <memory>
#include <type_traits>
#include <iterator>
#include <cstddef>
#include "fields2cover/types/Geometry.h"

namespace f2c::types {

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
struct Geometries : public Geometry<T, R> {
 public:
  using Geometry<T, R>::Geometry;
  /// Compute area of the geometry
  double area() const;
  SAMETYPE clone() const;
  SAMETYPE simplify(double d_tol) const;

  #ifndef SWIG
  // Code adapted from OGRCurve::Iterator
  class Iterator {
    struct Private {
      Private();
      ~Private();
      Private(const Private &) = delete;
      Private &operator=(const Private&) = delete;

      bool m_bUpdateChecked = true;
      CHILDRENTYPE m_oChild{};
      Geometries<SAMETYPE, T, R, CHILDRENTYPE>* m_poSelf = nullptr;
      int m_nPos = 0;
    };
    std::unique_ptr<Private> m_poPrivate;

    void update();

   public:
    Iterator(Geometries<SAMETYPE, T, R, CHILDRENTYPE>* poSelf, int nPos);
    ~Iterator();

    CHILDRENTYPE& operator*();

    Iterator& operator++();

    bool operator!=(const Iterator& it) const;
  };

  // Code adapted from OGRCurve::ConstIterator
  class ConstIterator {
    struct Private {
      Private();
      Private(const Private &) = delete;
      Private &operator=(const Private&) = delete;


      CHILDRENTYPE m_oChild{};
      const Geometries<SAMETYPE, T, R, CHILDRENTYPE>* m_poSelf = nullptr;
      int m_nPos = 0;
    };
    std::unique_ptr<Private> m_poPrivate;

   public:
    ConstIterator(ConstIterator& oOther);
    ConstIterator(const Geometries<SAMETYPE, T, R, CHILDRENTYPE>* poSelf,
        int nPos);

    ~ConstIterator();

    const CHILDRENTYPE& operator*() const;

    ConstIterator& operator++();

    bool operator!=(const ConstIterator& it) const;

    bool operator==(const Iterator& it) const;
  };

  Iterator begin();
  Iterator end();
  ConstIterator begin() const;
  ConstIterator end() const;
  ConstIterator begin(const SAMETYPE* poSelf);
  ConstIterator end(const SAMETYPE* poSelf);

  const CHILDRENTYPE operator[](int i) const;
  const CHILDRENTYPE at(int i) const;
  const CHILDRENTYPE back() const;

  SAMETYPE& append(const SAMETYPE& other);

  #endif
};

}  // namespace f2c::types

#include "fields2cover/types/Geometries_impl.hpp"


#endif  // FIELDS2COVER_TYPES_GEOMETRIES_H_
