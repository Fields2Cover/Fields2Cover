//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_ROUTE_PLANNING_FREE_SPACE_ROUTE_PLANNER_H_
#define FIELDS2COVER_ROUTE_PLANNING_FREE_SPACE_ROUTE_PLANNER_H_

#include "fields2cover/types.h"
#include "fields2cover/route_planning/route_planner_base.h"

namespace f2c::rp {

/// Route planner that travels the drivable ground itself, not its outline.
///
/// RoutePlannerBase joins each swath end to the nearest point on the border of
/// the cells it is given and travels along those borders. That is an outline,
/// not a free space, and three things follow from it: the route can never cut
/// across the ground, only walk around it; two pieces of ground are joined
/// only where their borders happen to meet within \a d_tol, so the graph falls
/// apart without saying so; and the route stays off the crop only where those
/// borders happen to lie off it.
///
/// This planner reads the same cells as the ground the robot may drive on --
/// the headland and any corridors, that is the field minus the mainland -- and
/// builds a visibility graph over it: a node for every swath end and every
/// corner of that ground, an edge wherever the straight segment between two
/// nodes stays on it. Shortest paths are then the real geodesics of the
/// drivable area, every edge is on drivable ground by construction, and
/// getComponentCount() says when the ground is not all one piece.
///
/// The graph is quadratic in its nodes and Graph2D solves all pairs, so keep
/// the ground's outline simple: setCornerTolerance() drops corners that carry
/// no detour.
class FreeSpaceRoutePlanner : public RoutePlannerBase {
 public:
  /// Create the visibility graph of the drivable ground.
  ///
  /// @param cells Ground the robot may drive on, not the outline to follow.
  /// @param swaths_by_cells Swaths to be covered. Their ends become nodes, so
  ///        an end on the border of \a cells is still reachable.
  /// @param d_tol Tolerance distance to consider if two points are the same.
  F2CGraph2D createShortestGraph(
      const F2CCells& cells, const F2CSwathsByCells& swaths_by_cells,
      double d_tol) const override;

  /// Keep the route this far inside the drivable ground where it can.
  ///
  /// A geodesic hugs whatever it goes around, so without a clearance the
  /// shortest path grazes the crop and the turn that follows it has to cut in.
  /// A clearance of about the turning radius leaves that turn its room.
  /// It is priced, not forbidden: ground the robot has no other way to reach,
  /// a swath end above all, stays reachable.
  /// @param clearance Width of the band to avoid. Zero turns this off.
  void setClearance(double clearance);

  /// Width of the band along the edge of the drivable ground the route avoids.
  double getClearance() const;

  /// What a metre driven inside the clearance band costs, as a multiple of it.
  /// @param cost Zero prices the band like any other ground.
  void setClearanceCost(double cost);

  /// What a metre driven inside the clearance band costs.
  double getClearanceCost() const;

  /// Drop corners of the drivable ground that sit within this of the line
  /// they cut, before they become graph nodes.
  /// @param tol Zero (the default) keeps every corner.
  void setCornerTolerance(double tol);

  /// Tolerance corners of the drivable ground are dropped within.
  double getCornerTolerance() const;

  /// Step the segments are sampled at when asked whether they stay on the
  /// drivable ground. Smaller sees narrower slivers and costs more.
  /// @param step Length in the units of the field. Defaults to 0.5.
  void setSampleStep(double step);

  /// Step the segments are sampled at.
  double getSampleStep() const;

  /// Pieces the last graph built fell into.
  ///
  /// One means every swath can be reached from every other. More than one
  /// means it cannot, and the route will jump straight between the pieces --
  /// over the crop, and possibly outside the field. The ground itself is what
  /// is disconnected, so the answer is to hand this planner ground that joins
  /// up, not to route around it.
  size_t getComponentCount() const;

  virtual ~FreeSpaceRoutePlanner() = default;

 private:
  double clearance_ {0.0};
  double clearance_cost_ {10.0};
  double corner_tol_ {0.0};
  double sample_step_ {0.5};
  mutable size_t n_components_ {1};
};

}  // namespace f2c::rp

#endif  // FIELDS2COVER_ROUTE_PLANNING_FREE_SPACE_ROUTE_PLANNER_H_
