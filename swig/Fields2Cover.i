/* File: fields2cover.i */

#pragma SWIG nowarn=315,317,320,362,503,509

%module fields2cover
 %include <std_string.i>
 %include <std_shared_ptr.i>
 %include <stdint.i>
 %include <typemaps.i>
 %include <std_vector.i>
 %include <optional.i>
 %include <exception.i>
 %include <std_pair.i>

#define __version__ "2.0.0"

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

DEFINE_GEOM_ALGS(distance)
DEFINE_GEOM_ALGS(disjoint)
DEFINE_GEOM_ALGS(crosses)
DEFINE_GEOM_ALGS(touches)
DEFINE_GEOM_ALGS(within)
DEFINE_GEOM_ALGS(intersects)
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

%define EXTEND_OPERATOR(geom)
  %extend f2c::types::geom {
    f2c::types::geom __add__(const f2c::types::Point& b) {
      return *$self + b;
    }
  }
%enddef
EXTEND_OPERATOR(LineString)
EXTEND_OPERATOR(LinearRing)
EXTEND_OPERATOR(MultiLineString)
EXTEND_OPERATOR(MultiPoint)
EXTEND_OPERATOR(Cell)
EXTEND_OPERATOR(Cells)





%extend f2c::types::Point {
  char *__str__() {
  static char temp[256];
  sprintf(temp,"Point(%g, %g, %g)", $self->getX(), $self->getY(), $self->getZ());
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

%ignore f2c::types::Graph::getEdges;
%ignore f2c::types::Graph::getEdgesFrom;
%ignore f2c::types::Graph::allPathsBetween;
%ignore f2c::types::Graph::shortestPathsAndCosts;
%include "fields2cover/types/Graph.h"
%ignore f2c::types::Graph2D::allPathsBetween;
%include "fields2cover/types/Graph2D.h"

%include "fields2cover/types/Swath.h"
%ignore f2c::types::Swaths::Swaths(std::initializer_list<Swath> const &);
%ignore f2c::types::Swaths::operator[];
%include "fields2cover/types/Swaths.h"
%ignore f2c::types::SwathsByCells::operator[];
%include "fields2cover/types/SwathsByCells.h"


%include "fields2cover/types/Strip.h"
%include "fields2cover/types/Field.h"
%ignore f2c::types::Route::addSwath;
%include "fields2cover/types/Route.h"
%include "fields2cover/types/PathState.h"
%ignore f2c::types::Path::operator[];
%include "fields2cover/types/Path.h"
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
%template(VectorSwaths) std::vector<f2c::types::Swaths>;
%template(VectorLineString) std::vector<f2c::types::LineString>;
%template(VectorLinearRing) std::vector<f2c::types::LinearRing>;
%template(VectorCell) std::vector<f2c::types::Cell>;
%template(VectorCells) std::vector<f2c::types::Cells>;
%template(Strips) std::vector<f2c::types::Strip>;
%template(PathStates) std::vector<f2c::types::PathState>;
%template(Fields) std::vector<f2c::types::Field>;
%template(VectorPathDirection) std::vector<f2c::types::PathDirection>;
%template(VectorPathSectionType) std::vector<f2c::types::PathSectionType>;

%include "fields2cover/utils/random.h"
%include "fields2cover/utils/spline.h"
%include "fields2cover/utils/parser.h"
%include "fields2cover/utils/visualizer.h"


%ignore f2c::Transform::generateCoordTransf;
%ignore f2c::Transform::createSptRef;
%ignore f2c::Transform::createCoordTransf;
%include "fields2cover/utils/transformation.h"


%define DEFINE_HG_COSTS(class_name, alg)
  %extend f2c::obj::class_name {
    %template(alg) alg<F2CCell, F2CCell>;
    %template(alg) alg<F2CCells, F2CCell>;
    %template(alg) alg<F2CCell, F2CCells>;
    %template(alg) alg<F2CCells, F2CCells>;
  }
%enddef

%define DEFINE_SG_COSTS(class_name, alg)
  %extend f2c::obj::class_name {
    %template(alg) alg<F2CSwath>;
    %template(alg) alg<F2CSwaths>;
    %template(alg) alg<F2CSwathsByCells>;
    %template(alg) alg<F2CCell, F2CSwath>;
    %template(alg) alg<F2CCell, F2CSwaths>;
    %template(alg) alg<F2CCell, F2CSwathsByCells>;
    %template(alg) alg<F2CCells, F2CSwath>;
    %template(alg) alg<F2CCells, F2CSwaths>;
    %template(alg) alg<F2CCells, F2CSwathsByCells>;
  }
%enddef
%define DEFINE_RP_COSTS(class_name, alg)
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
  }
%enddef
%define DEFINE_PP_COSTS(class_name, alg)
  %extend f2c::obj::class_name {
    %template(alg) alg<F2CPath>;
  }
%enddef


%include "fields2cover/objectives/base_objective.h"

%rename(OBJ_HGObjective) f2c::obj::HGObjective;
%rename(OBJ_SGObjective) f2c::obj::SGObjective;
%template(OBJ_Base_HG_OBJ) f2c::obj::BaseObjective<f2c::obj::HGObjective>;
%template(OBJ_Base_SG_OBJ) f2c::obj::BaseObjective<f2c::obj::SGObjective>;
%template(OBJ_Base_RP_OBJ) f2c::obj::BaseObjective<f2c::obj::RPObjective>;
%template(OBJ_Base_PP_OBJ) f2c::obj::BaseObjective<f2c::obj::PPObjective>;
DEFINE_HG_COSTS(BaseObjective<f2c::obj::HGObjective>, computeCostWithMinimizingSign)
DEFINE_SG_COSTS(BaseObjective<f2c::obj::SGObjective>, computeCostWithMinimizingSign)
DEFINE_RP_COSTS(BaseObjective<f2c::obj::RPObjective>, computeCostWithMinimizingSign)
DEFINE_PP_COSTS(BaseObjective<f2c::obj::PPObjective>, computeCostWithMinimizingSign)

%include "fields2cover/objectives/hg_obj/hg_objective.h"
%rename(OBJ_RemArea) f2c::obj::RemArea;
%include "fields2cover/objectives/hg_obj/rem_area.h"

%include "fields2cover/objectives/sg_obj/sg_objective.h"
%rename(OBJ_NSwath) f2c::obj::NSwath;
%include "fields2cover/objectives/sg_obj/n_swath.h"
%rename(OBJ_NSwathModified) f2c::obj::NSwathModified;
%include "fields2cover/objectives/sg_obj/n_swath_modified.h"
%rename(OBJ_FieldCoverage) f2c::obj::FieldCoverage;
%include "fields2cover/objectives/sg_obj/field_coverage.h"
%rename(OBJ_Overlaps) f2c::obj::Overlaps;
%include "fields2cover/objectives/sg_obj/overlaps.h"
%rename(OBJ_SwathLength) f2c::obj::SwathLength;
%include "fields2cover/objectives/sg_obj/swath_length.h"


%include "fields2cover/objectives/rp_obj/rp_objective.h"
%rename(OBJ_DirectDistPathObj) f2c::obj::DirectDistPathObj;
%include "fields2cover/objectives/rp_obj/direct_dist_path_obj.h"
%include "fields2cover/objectives/rp_obj/complete_turn_path_obj.h"

%include "fields2cover/objectives/pp_obj/pp_objective.h"
%rename(OBJ_PathLength) f2c::obj::PathLength;
%include "fields2cover/objectives/pp_obj/path_length.h"

%template(OBJ_CompleteTurnPathObj_Dubins) f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves>;
%template(OBJ_CompleteTurnPathObj_DubinsCC) f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurvesCC>;
%template(OBJ_CompleteTurnPathObj_ReedsShepp) f2c::obj::CompleteTurnPathObj<f2c::pp::ReedsSheppCurves>;
%template(OBJ_CompleteTurnPathObj_ReedsSheppHC) f2c::obj::CompleteTurnPathObj<f2c::pp::ReedsSheppCurvesHC>;

%include "fields2cover/headland_generator/headland_generator_base.h"
%rename(HG_Const_gen) f2c::hg::ConstHL;
%include "fields2cover/headland_generator/constant_headland.h"


%include "fields2cover/swath_generator/swath_generator_base.h"
%rename(SG_BruteForce) f2c::sg::BruteForce;
%include "fields2cover/swath_generator/brute_force.h"


%include "fields2cover/decomposition/decomposition_base.h"
%rename(DECOMP_TrapezoidalDecomp) f2c::decomp::TrapezoidalDecomp;
%include "fields2cover/decomposition/trapezoidal_decomp.h"
%rename(DECOMP_Boustrophedon) f2c::decomp::BoustrophedonDecomp;
%include "fields2cover/decomposition/boustrophedon_decomp.h"


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

// Wrap std::vector<long long int> and let SWIG handle the destructor
%feature("destructor", "delete") std::vector<long long int>;

// Explicitly wrap the std::vector<long long int> type
%template(LongLongVector) std::vector<long long int>;


%ignore f2c::rp::RoutePlannerBase::createShortestGraph;
%ignore f2c::rp::RoutePlannerBase::createCoverageGraph;
%rename(RP_RoutePlannerBase) f2c::rp::RoutePlannerBase;
%include "fields2cover/route_planning/route_planner_base.h"


%rename(PP_Turning_base_class) f2c::pp::TurningBase;
%include "fields2cover/path_planning/turning_base.h"

%define DEFINE_PP(file_src, alg)
  %rename(PP_ ## alg) f2c::pp::alg;
  %include file_src
%enddef

DEFINE_PP("fields2cover/path_planning/dubins_curves.h", DubinsCurves)
DEFINE_PP("fields2cover/path_planning/dubins_curves_cc.h", DubinsCurvesCC)
DEFINE_PP("fields2cover/path_planning/reeds_shepp_curves.h", ReedsSheppCurves)
DEFINE_PP("fields2cover/path_planning/reeds_shepp_curves_hc.h", ReedsSheppCurvesHC)
%rename(PP_PathPlanning) f2c::pp::PathPlanning;
%include "fields2cover/path_planning/path_planning.h"

%include "fields2cover.h"

%include "python/Fields2Cover.i"
