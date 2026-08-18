//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <optional>
#include <vector>
#include <steering_functions/utilities/utilities.hpp>
#include "fields2cover/path_planning/path_planning.h"

namespace f2c::pp {

namespace {

// Fractions of the operation width, unless noted otherwise.
constexpr double kLegShare = 0.5;         // a leg is split evenly between its two corners
constexpr double kMinSweep = 0.05;        // corners under ~3deg are driven straight through
constexpr double kRadiusMargin = 1.2;     // room for the clothoid lead-in of a CC turn
constexpr double kMinBackoffRadii = 0.5;  // shallow corners still get a workable approach
constexpr double kTurnSlack = 1.0;        // turning past this much extra is a loop or an S
constexpr double kReversalSweep = 2.0;    // legs this far apart in heading double back
constexpr double kUturnHopWidths = 3.0;   // a u-turn between neighbours spans about one
constexpr double kUturnReachRadii = 4.0;  // how far back a u-turn may start, in radii
constexpr size_t kMaxSpan = 3;            // a spur, the corner it leads to, and a spur out

// Below this deviation from the straight hop, a connection is driven direct.
constexpr double kDirectHopDev = 0.25;
// Points this close to the chord they'd round are dropped before rounding.
constexpr double kSimplifyTol = 0.1;

// cut_tol is the wider of these two -- see cutTolerance() for why.
constexpr double kCutTolWidths = 0.5;
constexpr double kCutTolRadii = 1.0;

// A leg runs along the connection's own track, driven at cruise speed.
void addStraight(F2CPath& path, const F2CRobot& robot,
    const F2CPoint& a, const F2CPoint& b) {
  const double len = a.distance(b);
  if (len < 1e-6) {
    return;
  }
  F2CPathState s;
  s.point = a;
  s.angle = (b - a).getAngleFromPoint();
  s.len = len;
  s.dir = f2c::types::PathDirection::FORWARD;
  s.type = f2c::types::PathSectionType::HL_SWATH;
  s.velocity = robot.getCruiseVel();
  path.addState(s);
}

// Radius the turn actually sweeps, which is what the approach to a corner has
// to be sized from. Two effects widen it past the minimum turning radius, and
// they dominate at opposite ends of the range:
//
//   - the clothoid ramps spend part of the deflection, so a turn that reaches
//     full curvature sweeps wider than 1/kappa (getSmoothTurningRadius);
//   - a shallow corner never reaches full curvature at all, peaking at
//     sqrt(rate * sweep), so the gentler the corner the wider it sweeps.
//
// Taking the larger keeps the approach honest at both ends. Dropping the
// deflection term lets a shallow corner be planned from a radius it never
// drives, which starts the turn too late and overlaps the leg it came in on.
double cornerRadius(const F2CRobot& robot, double sweep, bool continuous) {
  const double min_radius = robot.getMinTurningRadius();
  const double rate = robot.getMaxDiffCurv();
  if (!continuous || rate <= 0.0) {
    return min_radius;
  }
  double radius = std::max(min_radius, PathPlanning::getSmoothTurningRadius(robot));
  if (sweep > 0.0) {
    radius = std::max(radius, 1.0 / std::sqrt(rate * sweep));
  }
  return radius;
}

// How far rounding a corner may leave the track. Floored in turning radii,
// capped at the operation width: a compact implement (radius close to its own
// width) still gets room for a fillet, but a wide-turning one doesn't get a
// tolerance loose enough to cut corners it should instead leave sharp.
double cutTolerance(const F2CRobot& robot) {
  return std::max(
      std::min(kCutTolRadii * robot.getMinTurningRadius(), robot.getCovWidth()),
      kCutTolWidths * robot.getCovWidth());
}

// Point `dist` along the ray from `from` to `to`; `from` itself if they coincide.
F2CPoint pointAlong(const F2CPoint& from, const F2CPoint& to, double dist) {
  if (from.distance(to) < 1e-9) {
    return from;
  }
  return from.getPointFromAngle((to - from).getAngleFromPoint(), dist);
}

// Furthest any state of `arc` strays from the polyline it stands in for.
double deviationFromTrack(const F2CPath& arc, const std::vector<F2CPoint>& track) {
  double worst = 0.0;
  for (auto&& s : arc.getStates()) {
    double nearest = std::numeric_limits<double>::max();
    for (size_t k = 0; k + 1 < track.size(); ++k) {
      nearest = std::min(nearest, s.point.distance(F2CLineString(track[k], track[k + 1])));
    }
    worst = std::max(worst, nearest);
  }
  return worst;
}

// Follow `poly`: straight runs as they are, corners through the turn planner.
// `start_angle`/`end_angle` are the swath headings at either end, unset if
// there's none -- the track's own first/last leg is often a spur off the
// border graph the robot never drives (see PR-4, B.3). Returns the count of
// corners left sharp because no maneuver fit within `cut_tol` of the track.
size_t appendRoundedTrack(
    F2CPath& path, const std::vector<F2CPoint>& poly, const F2CRobot& robot,
    TurningBase& turn, double cut_tol, bool continuous,
    const std::optional<double>& start_angle, const std::optional<double>& end_angle) {
  const double radius = robot.getMinTurningRadius();
  const double op_width = robot.getCovWidth();
  const size_t n = poly.size();
  const auto legAngle = [&poly](size_t a, size_t b) {
      return (poly[b] - poly[a]).getAngleFromPoint();
    };

  // Deflection at each corner, which is what sizes the maneuver through it.
  std::vector<double> sweeps(n, 0.0);
  for (size_t k = 1; k + 1 < n; ++k) {
    sweeps[k] = F2CPoint::getAngleDiffAbs(legAngle(k - 1, k), legAngle(k, k + 1));
  }
  if (start_angle && n > 1) {
    sweeps.front() = F2CPoint::getAngleDiffAbs(*start_angle, legAngle(0, 1));
  }
  if (end_angle && n > 1) {
    sweeps.back() = F2CPoint::getAngleDiffAbs(legAngle(n - 2, n - 1), *end_angle);
  }

  size_t sharp = 0;
  F2CPoint cursor = poly.front();
  size_t i = 0;
  while (i < n) {
    if (sweeps[i] < kMinSweep) {
      addStraight(path, robot, cursor, poly[i]);
      cursor = poly[i];
      ++i;
      continue;
    }

    // Corners too close together to round one at a time fold into one span.
    size_t min_span = 1;
    while (i + min_span < n && min_span < kMaxSpan &&
        sweeps[i + min_span] >= kMinSweep &&
        poly[i + min_span - 1].distance(poly[i + min_span]) <
            2.0 * kRadiusMargin * radius) {
      ++min_span;
    }

    bool rounded = false;
    // Widening out from there, because a shorter span cuts less corner.
    for (size_t span = min_span; span <= kMaxSpan && i + span <= n && !rounded; ++span) {
      const size_t j = i + span - 1;  // last corner taken in one go
      // At either end the pose is pinned to the swath, not slid along a leg.
      const bool pin_in = (i == 0);
      const bool pin_out = (j + 1 == n);
      if (pin_out && !end_angle) {
        break;
      }
      const F2CPoint& first = poly[i];
      const F2CPoint& last_corner = poly[j];
      const F2CPoint& after = pin_out ? poly[j] : poly[j + 1];

      // Total turning sizes the maneuver's length; net deflection (which can
      // be smaller, e.g. on an S where corners cancel) sizes its geometry.
      double total = 0.0;
      for (size_t k = i; k <= j; ++k) {
        total += sweeps[k];
      }
      const double in_angle = pin_in ?
          *start_angle : (first - cursor).getAngleFromPoint();
      const double out_angle = pin_out ?
          *end_angle : (after - last_corner).getAngleFromPoint();
      const double net = F2CPoint::getAngleDiffAbs(in_angle, out_angle);

      const bool tail = (j + 2 == n) && sweeps[n - 1] < kMinSweep;
      const double back_room = pin_in ? 0.0 : cursor.distance(first);
      const double fwd_room = pin_out ? 0.0 :
          (tail ? 1.0 : kLegShare) * last_corner.distance(after);
      const double room = pin_in ? fwd_room : (pin_out ? back_room :
          std::min(back_room, fwd_room));

      // A short reversal is a u-turn (swings past the vertices into the
      // headland), not a corner to fillet; a long one is a detour to follow.
      const double hop = first.distance(last_corner);
      const bool uturn = net > kReversalSweep && hop < kUturnHopWidths * op_width;

      // Offset of the outgoing leg from the entry heading's line.
      const double sep = std::fabs(
          std::cos(in_angle) * (after.getY() - first.getY()) -
          std::sin(in_angle) * (after.getX() - first.getX()));

      double lo = kMinBackoffRadii * cornerRadius(robot, net, continuous);
      double hi = room;
      if (!uturn) {
        if (net > M_PI - 1e-3) {
          continue;  // reversal too wide to be anything but a detour
        }
        const double tangent_margin = continuous ? kRadiusMargin : 1.0;
        lo = std::max(
            tangent_margin * cornerRadius(robot, net, continuous) * std::tan(0.5 * net), lo);
        hi = std::min(hi, cut_tol / std::max(std::tan(0.25 * net), 1e-6));
      }
      // On a jog net cancels to ~0 and the tan(net/4) bound with it; fall
      // back to bounding the offset by the ground the maneuver makes up.
      // At a pinned end taking a single corner there is no outgoing leg to
      // measure that against -- `after` is the corner itself -- so the bound
      // would collapse to zero and refuse a corner the tangent bound allows.
      if (!pin_out || j > i) {
        hi = std::min(hi, sep + kRadiusMargin * (1.0 + total) * radius);
      }
      if (lo > hi) {
        continue;
      }

      // A fillet meets both legs the same distance out; a u-turn instead
      // takes whatever room each leg has, to turn a cusp into a drivable arc.
      double in_reach = hi;
      double out_reach = hi;
      if (uturn) {
        const double reach = kUturnReachRadii * cornerRadius(robot, net, continuous);
        in_reach = std::max(lo, std::min(back_room, reach));
        out_reach = std::max(lo, std::min(fwd_room, reach));
      }
      if (pin_in) {
        in_reach = 0.0;
      }
      if (pin_out) {
        out_reach = 0.0;
      }

      // Widen the offset until the planner accepts; a fillet is tried
      // tightest first (cuts least), a u-turn widest. Sampled finely because
      // the planner can refuse one specific offset for no geometric reason,
      // and jumping straight to a much wider one would cut more than needed.
      const std::array<double, 6> fracs =
          uturn ?
          std::array<double, 6>{1.0, 0.75, 0.5, 0.25, 0.1, 0.0} :
          std::array<double, 6>{0.0, 0.1, 0.25, 0.5, 0.75, 1.0};
      for (const double frac : fracs) {
        const double back_off = pin_in ? 0.0 : lo + frac * (in_reach - lo);
        const double fwd_off = pin_out ? 0.0 : lo + frac * (out_reach - lo);
        const F2CPoint entry = pointAlong(first, cursor, back_off);
        const F2CPoint exit = pointAlong(last_corner, after, fwd_off);
        F2CPath arc = turn.createTurn(robot, entry, in_angle, exit, out_angle);
        if (arc.size() == 0) {
          continue;
        }

        // A feasible pose pair can still come back as a loop, or as an S that
        // doubles back before settling. Both turn far more than the corner
        // asks for, which length alone measures only indirectly -- a loop's
        // extra 2*pi is cheap at a small radius and hides inside a length
        // budget. Compare the turning itself.
        double turned = 0.0;
        for (size_t k = 1; k < arc.size(); ++k) {
          turned += F2CPoint::getAngleDiffAbs(arc[k].angle, arc[k - 1].angle);
        }
        // A u-turn earns its teardrop; anything else should turn once.
        if (turned > total + (uturn ? M_PI : kTurnSlack)) {
          continue;
        }

        std::vector<F2CPoint> track{entry};
        for (size_t k = i; k <= j; ++k) {
          track.push_back(poly[k]);
        }
        track.push_back(exit);
        // A u-turn is allowed the corridor plus the diameter it swings past.
        const double dev = deviationFromTrack(arc, track);
        const double dev_max = uturn ?
            cut_tol + 2.0 * cornerRadius(robot, net, continuous) : cut_tol;
        if (dev > dev_max) {
          continue;
        }

        addStraight(path, robot, cursor, entry);
        path += arc;  // as the planner returned it, reverse legs included
        cursor = exit;
        i = j + 1;
        rounded = true;
        break;
      }
    }

    if (!rounded) {
      ++sharp;
      addStraight(path, robot, cursor, poly[i]);
      cursor = poly[i];
      ++i;
    }
  }
  addStraight(path, robot, cursor, poly.back());
  return sharp;
}

}  // namespace

F2CPath PathPlanning::planPath(const F2CRobot& robot,
    const F2CRoute& route, TurningBase& turn) {
  F2CPath path;
  for (size_t i = 0; i < route.sizeVectorSwaths(); ++i) {
    auto prev_swaths = (i >0) ? route.getSwaths(i-1) : F2CSwaths();
    path += planPathForConnection(robot,
        prev_swaths, route.getConnection(i), route.getSwaths(i), turn);
    path += planPath(robot, route.getSwaths(i), turn);
  }
  if (route.sizeConnections() > route.sizeVectorSwaths()) {
    path += planPathForConnection(robot,
      route.getLastSwaths(), route.getLastConnection(), F2CSwaths(), turn);
  }
  return path;
}

F2CPath PathPlanning::planPath(const F2CRobot& robot,
    const F2CSwaths& swaths, TurningBase& turn) {
  F2CPath path;
  if (swaths.size() > 1) {
    for (size_t i = 0; i < swaths.size()-1; ++i) {
      path.appendSwath(swaths[i], robot.getCruiseVel());
      F2CPath turn_path = turn.createTurn(robot,
          swaths[i].endPoint(), swaths[i].getOutAngle(),
          swaths[i + 1].startPoint(), swaths[i + 1].getInAngle());
      path += turn_path.discretize(0.1);
    }
  }
  if (swaths.size() > 0) {
    path.appendSwath(swaths.back(), robot.getCruiseVel());
  }
  return path;
}

F2CPath PathPlanning::planPathForConnection(const F2CRobot& robot,
    const F2CSwaths& s1,
    const F2CMultiPoint& mp,
    const F2CSwaths& s2,
    TurningBase& turn) {
  F2CPoint p1, p2;
  double ang1, ang2;

  if (s1.size() > 0) {
    p1 = s1.back().endPoint();
    ang1 = s1.back().getOutAngle();
  } else if (mp.size() > 0) {
    p1 = mp[0];
    ang1 = mp.getOutAngle(0);
  } else {
    return {};
  }
  if (s2.size() > 0) {
    p2 = s2[0].startPoint();
    ang2 = s2[0].getInAngle();
  } else if (mp.size() > 0) {
    p2 = mp.getLastPoint();
    ang2 = mp.getInAngle(mp.size()-1);
  } else {
    return {};
  }
  return planPathForConnection(robot, p1, ang1, mp, p2, ang2, turn);
}

F2CPath PathPlanning::planPathForConnection(const F2CRobot& robot,
    const F2CPoint& p1, double ang1,
    const F2CMultiPoint& mp,
    const F2CPoint& p2, double ang2,
    TurningBase& turn) {
  std::vector<F2CPoint> pts{p1};
  for (size_t i = 0; i < mp.size(); ++i) {
    pts.push_back(mp[i]);
  }
  pts.push_back(p2);

  // A track close to the straight hop isn't a detour, so drive it direct; a
  // short-hop reversal is the same call, since no fillet can round a u-turn.
  double max_dev = 0.0;
  for (size_t i = 1; i + 1 < pts.size(); ++i) {
    max_dev = std::max(max_dev, pts[i].distance(F2CLineString(p1, p2)));
  }
  const double op_width = robot.getCovWidth();
  bool direct = max_dev < kDirectHopDev * op_width;
  if (!direct) {
    const double reversal = F2CPoint::getAngleDiffAbs(ang1, ang2);
    const double hop = p1.distance(p2);
    direct = reversal > kReversalSweep && hop < kUturnHopWidths * op_width;
  }
  if (direct) {
    return turn.createTurn(robot, p1, ang1, p2, ang2);
  }

  // Otherwise follow the detour, rounding its corners, rather than cutting
  // across ground the route deliberately routed around.
  F2CLineString simplified = F2CLineString(pts).simplify(kSimplifyTol * op_width);
  std::vector<F2CPoint> poly;
  for (size_t i = 0; i < simplified.size(); ++i) {
    poly.push_back(simplified[i]);
  }
  if (poly.size() < 2) {
    return {};
  }

  F2CPath path;
  appendRoundedTrack(path, poly, robot, turn, cutTolerance(robot),
      turn.hasContinuousCurvature(), ang1, ang2);
  return path;
}

double PathPlanning::getSmoothTurningRadius(const F2CRobot& robot) {
  double x, y, ang, k;
  end_of_clothoid(0.0, 0.0, 0.0, 0.0, robot.getMaxDiffCurv(), 1.0,
      robot.getMaxCurv() / robot.getMaxDiffCurv(),
      &x, &y, &ang, &k);
  double xi = x - sin(ang) / robot.getMaxCurv();
  double yi = y + cos(ang) / robot.getMaxCurv();
  return sqrt(xi*xi + yi*yi);
}

}  // namespace f2c::pp

