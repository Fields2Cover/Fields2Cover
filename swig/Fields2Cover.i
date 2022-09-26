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

#define __version__ "1.1.0"

%inline %{
  #include "fields2cover.h"
%}

%exception {
  try {
    $action
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}



%ignore f2c::types::Geometries::Iterator;
%ignore f2c::types::Geometries::ConstIterator;
%ignore f2c::types::Geometries::begin;
%ignore f2c::types::Geometries::end;
%ignore f2c::types::EmptyDestructor;

%define DEFINE_GEOM_ALGS(alg)
  %extend f2c::types::Geometry {
    %template(alg) alg<OGRPoint, wkbPoint>;
    %template(alg) alg<OGRMultiPoint, wkbMultiPoint>;
    %template(alg) alg<OGRLinearRing, wkbLinearRing>;
    %template(alg) alg<OGRLineString, wkbLineString>;
    %template(alg) alg<OGRMultiLineString, wkbMultiLineString>;
    %template(alg) alg<OGRPolygon, wkbPolygon>;
    %template(alg) alg<OGRMultiPolygon, wkbMultiPolygon>;
  }
%enddef


%include "fields2cover/types/Geometry.h"

DEFINE_GEOM_ALGS(Distance)
DEFINE_GEOM_ALGS(Disjoint)
DEFINE_GEOM_ALGS(Crosses)
DEFINE_GEOM_ALGS(Touches)
DEFINE_GEOM_ALGS(Within)
DEFINE_GEOM_ALGS(Intersects)
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


%define EXTEND_ALGS(geom, alg)
  %extend f2c::types::geom {
    %template(alg) alg<f2c::types::LineString>;
    %template(alg) alg<f2c::types::MultiLineString>;
    %template(alg) alg<f2c::types::LinearRing>;
    %template(alg) alg<f2c::types::MultiPoint>;
    %template(alg) alg<f2c::types::Cell>;
    %template(alg) alg<f2c::types::Cells>;
  }
%enddef


%include "fields2cover/types/Point.h"
EXTEND_ALGS(Point, rotateFromPoint)

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
%ignore f2c::types::Swaths::Swaths(std::initializer_list<Swath> const &);
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
%inline %{
typedef long unsigned int size_t;
%}
%template(VectorSize) std::vector<size_t>;
%template(VectorPoint) std::vector<F2CPoint>;
%template(VectorMultiPoint) std::vector<F2CMultiPoint>;
%template(VectorSwath) std::vector<f2c::types::Swath>;
%template(SwathsByCells) std::vector<f2c::types::Swaths>;
%template(Fields) std::vector<f2c::types::Field>;
%template(VectorPathDirection) std::vector<f2c::types::PathDirection>;
%template(VectorPathSectionType) std::vector<f2c::types::PathSectionType>;

%include "fields2cover/utils/random.h"
%include "fields2cover/utils/parser.h"
%include "fields2cover/utils/visualizer.h"

%template(plot) f2c::Visualizer::plot<f2c::types::Point>;
%template(plot) f2c::Visualizer::plot<f2c::types::MultiPoint>;
%template(plot) f2c::Visualizer::plot<f2c::types::LineString>;
%template(plot) f2c::Visualizer::plot<f2c::types::LinearRing>;

%ignore f2c::Transform::generateCoordTransf;
%ignore f2c::Transform::createSptRef;
%ignore f2c::Transform::createCoordTransf;
%include "fields2cover/utils/transformation.h"



%define DEFINE_GLOBAL_COSTS(class_name, alg)
  %extend f2c::obj::class_name {
    %template(alg) alg<F2CSwath>;
    %template(alg) alg<F2CSwaths>;
    %template(alg) alg<F2CSwathsByCells>;
    %template(alg) alg<F2CCell>;
    %template(alg) alg<F2CCells>;
    %template(alg) alg<F2CRoute>;
    %template(alg) alg<F2CPath>;
    %template(alg) alg<F2CCell, F2CSwath>;
    %template(alg) alg<F2CCell, F2CSwaths>;
    %template(alg) alg<F2CCell, F2CSwathsByCells>;
    %template(alg) alg<F2CCells, F2CSwath>;
    %template(alg) alg<F2CCells, F2CSwaths>;
    %template(alg) alg<F2CCells, F2CSwathsByCells>;
    %template(alg) alg<F2CRoute, F2CPath>;
    %template(alg) alg<F2CCell, F2CSwath, F2CRoute>;
    %template(alg) alg<F2CCell, F2CSwath, F2CPath>;
    %template(alg) alg<F2CCell, F2CSwath, F2CRoute, F2CPath>;
    %template(alg) alg<F2CCell, F2CSwaths, F2CRoute>;
    %template(alg) alg<F2CCell, F2CSwaths, F2CPath>;
    %template(alg) alg<F2CCell, F2CSwaths, F2CRoute, F2CPath>;
    %template(alg) alg<F2CCell, F2CSwathsByCells, F2CRoute>;
    %template(alg) alg<F2CCell, F2CSwathsByCells, F2CPath>;
    %template(alg) alg<F2CCell, F2CSwathsByCells, F2CRoute, F2CPath>;
    %template(alg) alg<F2CCells, F2CSwath, F2CRoute>;
    %template(alg) alg<F2CCells, F2CSwath, F2CPath>;
    %template(alg) alg<F2CCells, F2CSwath, F2CRoute, F2CPath>;
    %template(alg) alg<F2CCells, F2CSwaths, F2CRoute>;
    %template(alg) alg<F2CCells, F2CSwaths, F2CPath>;
    %template(alg) alg<F2CCells, F2CSwaths, F2CRoute, F2CPath>;
    %template(alg) alg<F2CCells, F2CSwathsByCells, F2CRoute>;
    %template(alg) alg<F2CCells, F2CSwathsByCells, F2CPath>;
    %template(alg) alg<F2CCells, F2CSwathsByCells, F2CRoute, F2CPath>;
  }
%enddef
%define DEFINE_PATH_COSTS(class_name, alg)
  %extend f2c::obj::class_name {
    %template(alg) alg<F2CPoint, F2CPoint>;
    %template(alg) alg<F2CPoint, double, F2CPoint>;
    %template(alg) alg<F2CPoint, double, F2CPoint, double>;
    %template(alg) alg<F2CPoint, F2CPoint, double>;
    %template(alg) alg<F2CSwath, F2CPoint>;
    %template(alg) alg<F2CSwath, F2CSwath>;
    %template(alg) alg<F2CSwath, F2CPoint, double>;
    %template(alg) alg<F2CPoint, F2CSwath>;
    %template(alg) alg<F2CPoint, double, F2CSwath>;
    %template(alg) alg<std::vector<F2CPoint>>;
    %template(alg) alg<F2CMultiPoint>;
    %template(alg) alg<F2CSwath, F2CMultiPoint>;
    %template(alg) alg<F2CSwaths, F2CMultiPoint>;
    %template(alg) alg<F2CMultiPoint, F2CSwath>;
    %template(alg) alg<F2CMultiPoint, F2CSwaths>;
    %template(alg) alg<F2CSwath>;
    %template(alg) alg<F2CSwaths>;
    %template(alg) alg<F2CRoute>;
    %template(alg) alg<F2CPath>;
  }
%enddef

%rename(OBJ_GlobalObjective) f2c::obj::GlobalObjective;
%include "fields2cover/objectives/sg_objective.h"
DEFINE_GLOBAL_COSTS(SGObjective, computeCostWithMinimizingSign)

%include "fields2cover/objectives/optimization_class.h"
DEFINE_GLOBAL_COSTS(OptimizationClass, computeCost)
DEFINE_GLOBAL_COSTS(OptimizationClass, computeCostWithMinimizingSign)

%rename(OBJ_NSwath) f2c::obj::NSwath;
%include "fields2cover/objectives/n_swath.h"
%rename(OBJ_FieldCoverage) f2c::obj::FieldCoverage;
%include "fields2cover/objectives/field_coverage.h"
%rename(OBJ_Overlaps) f2c::obj::Overlaps;
%include "fields2cover/objectives/overlaps.h"
%rename(OBJ_SwathLength) f2c::obj::SwathLength;
%include "fields2cover/objectives/swath_length.h"

%rename(OBJ_RPObjective) f2c::obj::RPObjective;
%include "fields2cover/objectives/rp_objective.h"
DEFINE_PATH_COSTS(RPObjective, computeCostWithMinimizingSign)

%rename(OBJ_DirectDistPathObj) f2c::obj::DirectDistPathObj;
%include "fields2cover/objectives/direct_dist_path_obj.h"
%include "fields2cover/objectives/complete_turn_path_obj.h"

%template(OBJ_Optimization_NSwath) f2c::obj::OptimizationClass<f2c::obj::NSwath>;
%template(OBJ_Optimization_FieldCoverage) f2c::obj::OptimizationClass<f2c::obj::FieldCoverage>;
%template(OBJ_Optimization_Overlaps) f2c::obj::OptimizationClass<f2c::obj::Overlaps>;
%template(OBJ_Optimization_SwathLength) f2c::obj::OptimizationClass<f2c::obj::SwathLength>;

%template(OBJ_CompleteTurnPathObj_Dubins) f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves>;
%template(OBJ_CompleteTurnPathObj_DubinsCC) f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurvesCC>;
%template(OBJ_CompleteTurnPathObj_ReedsShepp) f2c::obj::CompleteTurnPathObj<f2c::pp::ReedsSheppCurves>;
%template(OBJ_CompleteTurnPathObj_ReedsSheppHC) f2c::obj::CompleteTurnPathObj<f2c::pp::ReedsSheppCurvesHC>;

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
%rename(RP_CustomOrder) f2c::rp::CustomOrder;
%include "fields2cover/route_planning/custom_order.h"

%rename(PP_Turning_base_class) f2c::pp::TurningBase;
%include "fields2cover/path_planning/turning_base.h"

%define DEFINE_PP(file_src, alg)
  %ignore f2c::pp::alg::alg();
  %rename(PP_ ## alg) f2c::pp::alg;
  %include file_src
%enddef

DEFINE_PP("fields2cover/path_planning/dubins_curves.h", DubinsCurves)
DEFINE_PP("fields2cover/path_planning/dubins_curves_cc.h", DubinsCurvesCC)
DEFINE_PP("fields2cover/path_planning/reeds_shepp_curves.h", ReedsSheppCurves)
DEFINE_PP("fields2cover/path_planning/reeds_shepp_curves_hc.h", ReedsSheppCurvesHC)
%rename(PP_PathPlanning) f2c::pp::PathPlanning;
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

TEMPLATE_GLOBAL_COST_FUNC_WRAP(OBJ_Optimization_base_class, f2c::obj::OptimizationClass)
TEMPLATE_GLOBAL_COST_FUNC_WRAP(HG_base_class, f2c::hg::HeadlandGeneratorBase)
TEMPLATE_GLOBAL_COST_FUNC_WRAP(SG_base_class, f2c::sg::SwathGeneratorBase)


TEMPLATE_GLOBAL_COST_FUNC_WRAP(HG_Const_gen, f2c::hg::ConstHL)
%template(HG_Const_gen) f2c::hg::ConstHL<>;

TEMPLATE_GLOBAL_COST_FUNC_WRAP(SG_BruteForce, f2c::sg::BruteForce)



%include "python/Fields2Cover.i"
