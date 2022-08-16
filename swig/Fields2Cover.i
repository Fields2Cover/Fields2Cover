/* File: fields2cover.i */

#pragma SWIG nowarn=315,320,362,503,509

%module fields2cover
 %include <std_string.i>
 %include <std_shared_ptr.i>
 %include <stdint.i>
 %include <typemaps.i>
 %include <std_vector.i>
 %include <optional.i>
 %include <exception.i>

#define __version__ "1.0.2"

%inline %{
  #include "fields2cover.h"
%}


%ignore f2c::types::Geometries::Iterator;
%ignore f2c::types::Geometries::ConstIterator;
%ignore f2c::types::Geometries::begin;
%ignore f2c::types::Geometries::end;
%ignore Transform::generateCoordTransf;
%ignore Transform::createSptRef;
%ignore Transform::createCoordTransf;

%include "fields2cover/types/Geometry.h"
%template(GeomPoint) f2c::types::Geometry<OGRPoint, wkbPoint>;
%template(GeomMultiPoint) f2c::types::Geometry<OGRMultiPoint, wkbMultiPoint>;
%template(GeomLinearRing) f2c::types::Geometry<OGRLinearRing, wkbLinearRing>;
%template(GeomLineString) f2c::types::Geometry<OGRLineString, wkbLineString>;
%template(GeomMultiLineString) f2c::types::Geometry<OGRMultiLineString, wkbMultiLineString>;
%template(GeomPolygon) f2c::types::Geometry<OGRPolygon, wkbPolygon>;
%template(GeomMultiPolygon) f2c::types::Geometry<OGRMultiPolygon, wkbMultiPolygon>;

%include "fields2cover/types/Geometries.h"
%template(GeomsMultiPoint) f2c::types::Geometries<f2c::types::MultiPoint, OGRMultiPoint, wkbMultiPoint, f2c::types::Point>;
%template(GeomsLinearRing) f2c::types::Geometries<f2c::types::LinearRing, OGRLinearRing, wkbLinearRing, f2c::types::Point>;
%template(GeomsLineString) f2c::types::Geometries<f2c::types::LineString, OGRLineString, wkbLineString, f2c::types::Point>;
%template(GeomsMultiLineString) f2c::types::Geometries<f2c::types::MultiLineString, OGRMultiLineString, wkbMultiLineString, f2c::types::LineString>;
%template(GeomsPolygon) f2c::types::Geometries<f2c::types::Cell, OGRPolygon, wkbPolygon, f2c::types::LinearRing>;
%template(GeomsMultiPolygon) f2c::types::Geometries<f2c::types::Cells, OGRMultiPolygon, wkbMultiPolygon, f2c::types::Cell>;


%include "fields2cover/types/Point.h"

%extend f2c::types::Point {
  char *__str__() {
  static char temp[256];
  sprintf(temp,"Point(%g, %g, %g)", $self->getX(),$self->getY(),$self->getZ());
  return &temp[0];
  }
}


%ignore f2c::types::MultiPoint::MultiPoint(std::initializer_list<Point> const &);
%include "fields2cover/types/MultiPoint.h"
%ignore f2c::types::LinearRing::LinearRing(std::initializer_list<Point> const &);
%include "fields2cover/types/LinearRing.h"
%ignore f2c::types::LineString::LineString(std::initializer_list<Point> const &);
%include "fields2cover/types/LineString.h"
%include "fields2cover/types/MultiLineString.h"
%include "fields2cover/types/Cell.h"
%include "fields2cover/types/Cells.h"
%include "fields2cover/types/Swath.h"
%ignore f2c::types::Swaths::operator[];
%include "fields2cover/types/Swaths.h"
%extend f2c::types::Swaths {
  inline size_t __len__() const { return self->size(); }
  inline f2c::types::Swath& __getitem__(size_t i) throw(std::out_of_range) {
    if (i >= self->size() || i < 0)
      throw std::out_of_range("out of bounds access");
    return self->at(i);
  }
  inline void __setitem__(size_t i, const f2c::types::Swath& v) throw(std::out_of_range) {
    if (i >= self->size() || i < 0)
      throw std::out_of_range("out of bounds access");
    self->at(i) = v;
  }
}

%include "fields2cover/types/Field.h"
%include "fields2cover/types/Route.h"
%include "fields2cover/types/Path.h"
%include "fields2cover/types/OptimizationParams.h"
%include "fields2cover/types/Robot.h"
%include "fields2cover/types.h"

%template(optional_double) std::optional<double>;
%template(optional_Point) std::optional<f2c::types::Point>;

%template(VectorDouble) std::vector<double>;
%template(VectorInt) std::vector<int>;
%template(VectorPoint) std::vector<F2CPoint>;
%template(VectorMultiPoint) std::vector<F2CMultiPoint>;
%template(SwathsByCells) std::vector<f2c::types::Swaths>;
%template(Fields) std::vector<f2c::types::Field>;
%template(VectorPathDirection) std::vector<f2c::types::PathDirection>;
%template(VectorPathSectionType) std::vector<f2c::types::PathSectionType>;

%include "fields2cover/utils/random.h"
%include "fields2cover/utils/parser.h"
%include "fields2cover/utils/visualizer.h"

%include "fields2cover/objectives/global_objective.h"
%import "fields2cover/objectives/optimization_class.h"
%include "fields2cover/objectives/n_swath.h"
%include "fields2cover/objectives/field_coverage.h"
%include "fields2cover/objectives/overlaps.h"
%include "fields2cover/objectives/swath_length.h"

%include "fields2cover/objectives/path_objective.h"
%include "fields2cover/objectives/direct_dist_path_obj.h"
%include "fields2cover/objectives/complete_turn_path_obj.h"


%include "fields2cover/headland_generator/headland_generator_base.h"
%include "fields2cover/headland_generator/constant_headland.h"


%include "fields2cover/swath_generator/swath_generator_base.h"
%include "fields2cover/swath_generator/brute_force.h"




%rename(RP_Single_cell_order_base_class) f2c::rp::SingleCellSwathsOrderBase;
%include "fields2cover/route_planning/single_cell_swaths_order_base.h"
%rename(RP_Boustrophedon) f2c::rp::BoustrophedonOrder;
%include "fields2cover/route_planning/boustrophedon_order.h"
%rename(RP_Snake) f2c::rp::SnakeOrder;
%include "fields2cover/route_planning/snake_order.h"
%rename(RP_Spiral) f2c::rp::SpiralOrder;
%include "fields2cover/route_planning/spiral_order.h"

%rename(PP_Turning_base_class) f2c::pp::TurningBase;
%include "fields2cover/path_planning/turning_base.h"

%define DEFINE_PP(file_src, alg)
%ignore f2c::pp::alg::alg();
%include file_src
%extend f2c::pp::alg {
    alg(const f2c::types::Robot& robot) {
      f2c::pp::alg* newAlg = new f2c::pp::alg();
      newAlg->setRobotParams(robot);
      return newAlg;
    }
};
%enddef

DEFINE_PP("fields2cover/path_planning/dubins_curves.h", DubinsCurves)
DEFINE_PP("fields2cover/path_planning/dubins_curves_cc.h", DubinsCurvesCC)
DEFINE_PP("fields2cover/path_planning/reeds_shepp_curves.h", ReedsSheppCurves)
DEFINE_PP("fields2cover/path_planning/reeds_shepp_curves_hc.h", ReedsSheppCurvesHC)
%include "fields2cover/path_planning/path_planning.h"


%define TEMPLATE_COST_FUNC_NAME(prefix, T, OBJ_NAME, OBJ)
  %template(prefix ## _ ## OBJ_NAME) T<f2c::obj::OBJ>;
%enddef

%define TEMPLATE_COST_FUNC(prefix, T, OBJ)
  TEMPLATE_COST_FUNC_NAME(prefix, T, OBJ, OBJ)
%enddef

%define TEMPLATE_GLOBAL_COST_FUNC_WRAP(prefix, T)
  TEMPLATE_COST_FUNC(prefix, T, NSwath)
  TEMPLATE_COST_FUNC(prefix, T, FieldCoverage)
  TEMPLATE_COST_FUNC(prefix, T, Overlaps)
  TEMPLATE_COST_FUNC(prefix, T, SwathLength)
%enddef


%define TEMPLATE_PATH_COST_FUNC_WRAP(prefix, T)
  TEMPLATE_COST_FUNC(prefix, T, DirectDistPathObj)
  TEMPLATE_COST_FUNC_NAME(prefix, T, DistDubins, CompleteTurnPathObj<f2c::pp::DubinsCurves>)
  TEMPLATE_COST_FUNC_NAME(prefix, T, DistDubinsCC, CompleteTurnPathObj<f2c::pp::DubinsCurvesCC>)
  TEMPLATE_COST_FUNC_NAME(prefix, T, DistReedsShepp, CompleteTurnPathObj<f2c::pp::ReedsSheppCurves>)
  TEMPLATE_COST_FUNC_NAME(prefix, T, DistReedsSheppHC, CompleteTurnPathObj<f2c::pp::ReedsSheppCurvesHC>)
%enddef

TEMPLATE_GLOBAL_COST_FUNC_WRAP(Optimization_base_class, f2c::obj::OptimizationClass)
TEMPLATE_GLOBAL_COST_FUNC_WRAP(HG_base_class, f2c::hg::HeadlandGeneratorBase)
TEMPLATE_GLOBAL_COST_FUNC_WRAP(SG_base_class, f2c::sg::SwathGeneratorBase)


TEMPLATE_GLOBAL_COST_FUNC_WRAP(HG_Const_gen, f2c::hg::ConstHL)
%template(HG_Const_gen) f2c::hg::ConstHL<>;

TEMPLATE_GLOBAL_COST_FUNC_WRAP(SG_BruteForce, f2c::sg::BruteForce)


