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

  /// How far rounding a corner may leave its track [m]
  double getMaxCornerCut() const;
  void setMaxCornerCut(double);

  /// Below this deviation from the straight hop, a connection is driven direct [m]
  double getDirectHopMaxDev() const;
  void setDirectHopMaxDev(double);

  /// Below this hop, a reversal is a u-turn rather than a detour [m]
  double getUturnMaxHop() const;
  void setUturnMaxHop(double);

  /// Points this close to the chord they would round are dropped [m]
  double getTrackSimplifyTol() const;
  void setTrackSimplifyTol(double);

  /// Heading difference between legs that reads as doubling back [rad]
  double getReversalSweep() const;
  void setReversalSweep(double);

  /// Turning this much past the corner's deflection is a loop or an S [rad]
  double getTurnSlack() const;
  void setTurnSlack(double);

  /// Corners deflecting less than this are driven straight through [rad]
  double getMinSweep() const;
  void setMinSweep(double);

  /// Share of a leg a corner may use, the rest left to the corner beyond it
  double getLegShare() const;
  void setLegShare(double);

  /// Room for a continuous turn's clothoid lead-in, in tangent lengths
  double getRadiusMargin() const;
  void setRadiusMargin(double);

  /// Approach a shallow corner keeps, in turn radii
  double getMinBackoffRadii() const;
  void setMinBackoffRadii(double);

  /// How far back a u-turn may start, in turn radii
  double getUturnReachRadii() const;
  void setUturnReachRadii(double);

  /// Most corners folded into a single maneuver
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

  /// If not set, derived from the turning radius and the operation width
  std::optional<double> max_corner_cut_;
  /// If not set, a quarter of the operation width
  std::optional<double> direct_hop_max_dev_;
  /// If not set, three operation widths
  std::optional<double> uturn_max_hop_;
  /// If not set, a tenth of the operation width
  std::optional<double> track_simplify_tol_;

  double reversal_sweep_ {2.0};      // [rad]
  double turn_slack_ {1.0};          // [rad]
  double min_sweep_ {0.05};          // [rad]
  double leg_share_ {0.5};
  double radius_margin_ {1.2};
  double min_backoff_radii_ {0.5};
  double uturn_reach_radii_ {4.0};
  size_t max_corner_span_ {3};
};

}  // namespace f2c::types

#endif  //  FIELDS2COVER_TYPES_ROBOT_H_
