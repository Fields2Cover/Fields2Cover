


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
    size_t size = $self->getNumPoints();
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
      size_t size_s = $self->at(i).getNumPoints();
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
      ss << "(" << $self->points.at(i).getX();
      ss << ", " << $self->points.at(i).getY();
      ss << ", " << $self->points.at(i).getZ() << "), ";
      ss << "ang = " << $self->angles.at(i) << ", ";
      ss << "vel = " << $self->velocities.at(0) << "]";
    }
    ss << ")";
    return ss.str();
  }  
}
