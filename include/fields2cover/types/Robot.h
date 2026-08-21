//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_ROBOT_H_
#define FIELDS2COVER_TYPES_ROBOT_H_

#include <ogr_geometry.h>
#include <utility>
#include <string>
#include <vector>
#include <optional>
#include "fields2cover/types/Point.h"
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/MultiLineString.h"
#include "fields2cover/types/Cell.h"
#include "fields2cover/types/Cells.h"
#include "fields2cover/types/Path.h"

namespace f2c::types {

struct Robot {
 public:
  explicit Robot(double width, double cov_width = 0.0,
      double max_curv = 1.0, double max_diff_curv = 0.3);
  Robot();
  ~Robot();
  Robot(const Robot&);
  Robot(Robot &&);
  Robot &operator=(const Robot&);
  Robot &operator=(Robot&&);

  std::string getName() const;
  void setName(const std::string& str);

  double getWidth() const;
  void setWidth(double);

  double getCovWidth() const;
  void setCovWidth(double);

  double getCruiseVel() const;
  void setCruiseVel(double);

  double getTurnVel() const;
  void setTurnVel(double);

  double getMinTurningRadius() const;
  void setMinTurningRadius(double rad);

  double getMaxCurv() const;
  void setMaxCurv(double);

  double getMaxDiffCurv() const;
  void setMaxDiffCurv(double);

  /// Radius swept by a turn with continuous curvature, wider than the minimum
  double getSmoothTurningRadius() const;
  /// Radius a turn deflecting `sweep` radians is driven at
  double getTurnRadius(double sweep, bool continuous) const;

  double getMaxCornerCut() const;
  void setMaxCornerCut(double);

  double getDirectHopMaxDev() const;
  void setDirectHopMaxDev(double);

  double getUturnMaxHop() const;
  void setUturnMaxHop(double);

  double getTrackSimplifyTol() const;
  void setTrackSimplifyTol(double);

  double getReversalSweep() const;
  void setReversalSweep(double);

  double getTurnSlack() const;
  void setTurnSlack(double);

  double getMinSweep() const;
  void setMinSweep(double);

  double getLegShare() const;
  void setLegShare(double);

  double getRadiusMargin() const;
  void setRadiusMargin(double);

  double getMinBackoffRadii() const;
  void setMinBackoffRadii(double);

  double getUturnReachRadii() const;
  void setUturnReachRadii(double);

  size_t getMaxCornerSpan() const;
  void setMaxCornerSpan(size_t);

 private:
  std::string name_;

  /// Width of the robot
  double width_ {0.0};
  /// Width of the coverage area of the robot
  double cov_width_ {0.0};

  /// Velocity of the robot when not doing turns.
  double cruise_speed_ {1.0};

  /// Maximum instantaneous curvature change
  double max_icc_ {1.0};  // [1/m]

  /// Maximum linear curvature change
  double linear_curv_change_ {0.3};  // [1/m^2]

  /// Velocity of the robot when doing turns. If not set, cruise_speed_ is used
  std::optional<double> turn_vel_;

  /// How far rounding a corner may leave its track. If not set, the wider of
  /// the turning radius and half the operation width, capped at the width.
  std::optional<double> max_corner_cut_;
  /// Below this deviation from the straight hop, a connection is driven
  /// direct. If not set, a quarter of the operation width.
  std::optional<double> direct_hop_max_dev_;
  /// Below this hop, a reversal is a u-turn rather than a detour to follow.
  /// If not set, three operation widths.
  std::optional<double> uturn_max_hop_;
  /// Points this close to the chord they would round are dropped before a
  /// connection's corners are planned. If not set, a tenth of the width.
  std::optional<double> track_simplify_tol_;

  /// Heading difference between two legs that reads as doubling back
  double reversal_sweep_ {2.0};  // [rad]
  /// Turning this much past a corner's deflection is a loop or an S, refused
  double turn_slack_ {1.0};  // [rad]
  /// Corners deflecting less than this are driven straight through
  double min_sweep_ {0.05};  // [rad]
  /// Share of a leg a corner may use, the rest left to the corner beyond it
  double leg_share_ {0.5};
  /// Room for a continuous turn's clothoid lead-in, in tangent lengths
  double radius_margin_ {1.2};
  /// Approach a shallow corner keeps even where its tangent collapses, in turn radii
  double min_backoff_radii_ {0.5};
  /// How far back a u-turn may start, in turn radii
  double uturn_reach_radii_ {4.0};
  /// Most corners folded into a single maneuver
  size_t max_corner_span_ {3};
};

}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_ROBOT_H_
