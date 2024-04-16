


%extend f2c::types::Point {
  std::string __repr__()
  {
    std::ostringstream ss;
    ss << $self->exportToWkt();
    return ss.str();
  }  
}
%extend f2c::types::LinearRing {
  std::string __repr__()
  {
    std::ostringstream ss;
    ss << $self->exportToWkt();
    return ss.str();
  }  
}
%extend f2c::types::LineString {
  std::string __repr__()
  {
    std::ostringstream ss;
    ss << $self->exportToWkt();
    return ss.str();
  }  
}
%extend f2c::types::MultiPoint {
  std::string __repr__()
  {
    std::ostringstream ss;
    ss << $self->exportToWkt();
    return ss.str();
  }  
}
%extend f2c::types::Cell {
  std::string __repr__()
  {
    std::ostringstream ss;
    ss << $self->exportToWkt();
    return ss.str();
  }  
}
%extend f2c::types::Cells {
  std::string __repr__()
  {
    std::ostringstream ss;
    ss << $self->exportToWkt();
    return ss.str();
  }  
}
%extend f2c::types::MultiLineString {
  std::string __repr__()
  {
    std::ostringstream ss;
    ss << $self->exportToWkt();
    return ss.str();
  }  
}

%extend f2c::types::Swath {
  std::string __repr__() {
    std::ostringstream ss;
    size_t size = $self->numPoints();
    ss << "SWATH([";
    for(size_t i = 0; i < size; ++i) {
      if (i > 0) { ss << ", "; }
      ss << "("<<$self->getPoint(i).getX();
      ss << ", " << $self->getPoint(i).getY();
      ss << ", " << $self->getPoint(i).getZ() << ")";
    }
    ss << "], width = " << $self->getWidth() << ")";
    return ss.str();
  }  
}

%extend f2c::types::Swaths {
  std::string __repr__() {
    std::ostringstream ss;
    ss << "SWATHS(";
    for(size_t i = 0; i < $self->size(); ++i) {
      if (i > 0) {
        ss <<", ";
      }
      size_t size_s = $self->at(i).numPoints();
      ss << "SWATH([";
      for(size_t j = 0; j < size_s; ++j) {
        if (j>0) { ss << ", ";}
        ss << "("<<$self->at(i).getPoint(j).getX();
        ss << ", " << $self->at(i).getPoint(j).getY();
        ss << ", " << $self->at(i).getPoint(j).getZ() << ")";
      }
      ss << "], width = " << $self->at(i).getWidth() << ")";
    }
    ss << ")";
    return ss.str();
  }
}

%extend f2c::types::Path {
  std::string __repr__() {
    std::ostringstream ss;
    size_t size = $self->size();
    ss << "PATH(";
    for(size_t i = 0; i < size; ++i) {
      if (i>0) { ss << ", ";}
      ss << "[Point";
      ss << "(" << $self->operator[](i).point.getX();
      ss << ", " << $self->operator[](i).point.getY();
      ss << ", " << $self->operator[](i).point.getZ() << "), ";
      ss << "ang = " << $self->operator[](i).angle << ", ";
      ss << "vel = " << $self->operator[](i).velocity << "]";
    }
    ss << ")";
    return ss.str();
  }  
}

%extend f2c::types::Strip {
  std::string __repr__()
  {
    std::ostringstream ss;
    ss << "Strip{ Name: '" << $self->getName() << "', ";
    ss << $self->getCell().exportToWkt() << "}";
    return ss.str();
  }  
}

%define GeometryExtend(VT, T)
%extend VT {
  inline size_t __len__() const {return self->size();}
  T __getitem__(int i)  throw(std::out_of_range) {
    if (i >= self->size() || i < 0) {
      throw std::out_of_range("out of bounds access");
    }
    return (*self).getGeometry(i);
  }
  inline void __setitem__(size_t i, const T& v) throw(std::out_of_range) {
    if (i >= self->size() || i < 0) {
      throw std::out_of_range("out of bounds access");
    }
    self->setGeometry(i, v);
  }
}
%enddef
%define ArrayExtend(VT, T)
%extend VT {
  T __getitem__(int i)  throw(std::out_of_range) {
    if (i >= self->size() || i < 0) {
      throw std::out_of_range("out of bounds access");
    }
    return self->at(i);
  }
  inline void __setitem__(size_t i, const T& v) throw(std::out_of_range) {
    if (i >= self->size() || i < 0) {
      throw std::out_of_range("out of bounds access");
    }
    (*self)[i] = v;
  }
}
%enddef
%define VectorExtend(T)
  ArrayExtend(std::vector<T>, T)
%enddef

VectorExtend(double)
VectorExtend(int)
VectorExtend(size_t)
VectorExtend(f2c::types::Point)
VectorExtend(f2c::types::MultiPoint)
VectorExtend(f2c::types::Swath)
VectorExtend(f2c::types::Swaths)
VectorExtend(f2c::types::Cell)
VectorExtend(f2c::types::Cells)
VectorExtend(f2c::types::LinearRing)
VectorExtend(f2c::types::LineString)
VectorExtend(f2c::types::Strip)
VectorExtend(f2c::types::PathState)
VectorExtend(f2c::types::Field)
VectorExtend(f2c::types::PathDirection)
VectorExtend(f2c::types::PathSectionType)

GeometryExtend(f2c::types::Cells, f2c::types::Cell)
GeometryExtend(f2c::types::Cell, f2c::types::LinearRing)
GeometryExtend(f2c::types::LinearRing, f2c::types::Point)
GeometryExtend(f2c::types::LineString, f2c::types::Point)
GeometryExtend(f2c::types::MultiPoint, f2c::types::Point)
GeometryExtend(f2c::types::MultiLineString, f2c::types::LineString)

ArrayExtend(f2c::types::Swaths, f2c::types::Swath)
ArrayExtend(f2c::types::SwathsByCells, f2c::types::Swaths)


%extend f2c::types::Path {
  f2c::types::PathState __getitem__(int i) {
    return (*self).getState(i);
  }
}






