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
#include "fields2cover/path_planning/path_planning.h"

namespace f2c::pp {

namespace {

// A leg along the connection's own track is driven as a headland pass.
void addStraight(F2CPath& path, const F2CRobot& robot,
    const F2CPoint& a, const F2CPoint& b) {
  path.appendStraight(a, b, robot.getCruiseVel(),
      f2c::types::PathSectionType::HL_SWATH);
}

// Deflection at each corner of `poly`. The ends take the swath heading where
// there is one: the track's own first/last leg is often a spur off the border
// graph the robot never drives (see PR-4, B.3).
std::vector<double> cornerSweeps(const std::vector<F2CPoint>& poly,
    const std::optional<double>& start_angle,
    const std::optional<double>& end_angle) {
  const size_t n = poly.size();
  const auto legAngle = [&poly](size_t a, size_t b) {
      return (poly[b] - poly[a]).getAngleFromPoint();
    };
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
  return sweeps;
}

// Corners from `i` on that sit too close together to round one at a time:
// rounding the first would leave the next no approach.
size_t foldedSpan(const std::vector<F2CPoint>& poly,
    const std::vector<double>& sweeps, size_t i, const F2CRobot& robot) {
  const double gap = 2.0 * robot.getRadiusMargin() * robot.getMinTurningRadius();
  size_t span = 1;
  while (i + span < poly.size() && span < robot.getMaxCornerSpan() &&
      sweeps[i + span] >= robot.getMinSweep() &&
      poly[i + span - 1].distance(poly[i + span]) < gap) {
    ++span;
  }
  return span;
}

// Where a maneuver through poly[i..j] may start and end, and what it must turn.
struct Span {
  F2CPoint first, last_corner, after;
  double in_angle, out_angle, net, total;
  double lo, in_reach, out_reach;
  bool pin_in, pin_out, uturn;
};

// Size the maneuver through poly[i..j]. Unset if no turn fits between the
// room the legs leave and the ground `cut_tol` lets it cut.
std::optional<Span> spanGeometry(const std::vector<F2CPoint>& poly,
    const std::vector<double>& sweeps, size_t i, size_t j,
    const F2CPoint& cursor, const F2CRobot& robot, double cut_tol,
    bool continuous, const std::optional<double>& start_angle,
    const std::optional<double>& end_angle) {
  const size_t n = poly.size();
  const double radius = robot.getMinTurningRadius();
  Span s;
  // At either end the pose is pinned to the swath, not slid along a leg.
  s.pin_in = (i == 0);
  s.pin_out = (j + 1 == n);
  s.first = poly[i];
  s.last_corner = poly[j];
  s.after = s.pin_out ? poly[j] : poly[j + 1];

  // Total turning sizes the maneuver's length; net deflection (which can be
  // smaller, e.g. on an S where corners cancel) sizes its geometry.
  s.total = 0.0;
  for (size_t k = i; k <= j; ++k) {
    s.total += sweeps[k];
  }
  s.in_angle = s.pin_in ?
      *start_angle : (s.first - cursor).getAngleFromPoint();
  s.out_angle = s.pin_out ?
      *end_angle : (s.after - s.last_corner).getAngleFromPoint();
  s.net = F2CPoint::getAngleDiffAbs(s.in_angle, s.out_angle);

  const bool tail = (j + 2 == n) && sweeps[n - 1] < robot.getMinSweep();
  const double back_room = s.pin_in ? 0.0 : cursor.distance(s.first);
  const double fwd_room = s.pin_out ? 0.0 :
      (tail ? 1.0 : robot.getLegShare()) * s.last_corner.distance(s.after);
  const double room = s.pin_in ? fwd_room : (s.pin_out ? back_room :
      std::min(back_room, fwd_room));

  // A short reversal is a u-turn (swings past the vertices into the headland),
  // not a corner to fillet; a long one is a detour to follow.
  const double hop = s.first.distance(s.last_corner);
  s.uturn = s.net > robot.getReversalSweep() && hop < robot.getUturnMaxHop();

  // Offset of the outgoing leg from the entry heading's line.
  const double sep = std::fabs(
      std::cos(s.in_angle) * (s.after.getY() - s.first.getY()) -
      std::sin(s.in_angle) * (s.after.getX() - s.first.getX()));

  const double turn_radius = robot.getTurnRadius(s.net, continuous);
  s.lo = robot.getMinBackoffRadii() * turn_radius;
  double hi = room;
  if (!s.uturn) {
    if (s.net > M_PI - 1e-3) {
      return {};  // reversal too wide to be anything but a detour
    }
    const double tangent_margin = continuous ? robot.getRadiusMargin() : 1.0;
    s.lo = std::max(tangent_margin * turn_radius * std::tan(0.5 * s.net), s.lo);
    hi = std::min(hi, cut_tol / std::max(std::tan(0.25 * s.net), 1e-6));
  }
  // On a jog net cancels to ~0 and the tan(net/4) bound with it; fall back to
  // bounding the offset by the ground the maneuver makes up. At a pinned end
  // taking a single corner there is no outgoing leg to measure that against --
  // `after` is the corner itself -- so the bound would collapse to zero.
  if (!s.pin_out || j > i) {
    hi = std::min(hi, sep + robot.getRadiusMargin() * (1.0 + s.total) * radius);
  }
  if (s.lo > hi) {
    return {};
  }

  // A fillet meets both legs the same distance out; a u-turn instead takes
  // whatever room each leg has, to turn a cusp into a drivable arc.
  s.in_reach = hi;
  s.out_reach = hi;
  if (s.uturn) {
    const double reach = robot.getUturnReachRadii() * turn_radius;
    s.in_reach = std::max(s.lo, std::min(back_room, reach));
    s.out_reach = std::max(s.lo, std::min(fwd_room, reach));
  }
  if (s.pin_in) {
    s.in_reach = 0.0;
  }
  if (s.pin_out) {
    s.out_reach = 0.0;
  }
  return s;
}

// A turn accepted for a span, and the points it runs between.
struct Turn {
  F2CPath arc;
  F2CPoint entry, exit;
};

// Widen the offset until the planner returns a turn that rounds the span
// rather than looping around it. Unset if none of them does.
std::optional<Turn> planSpanTurn(const Span& s,
    const std::vector<F2CPoint>& poly, size_t i, size_t j,
    const F2CPoint& cursor, const F2CRobot& robot, TurningBase& turn,
    double cut_tol, bool continuous) {
  // A fillet is tried tightest first (it cuts least), a u-turn widest. Sampled
  // finely because the planner can refuse one specific offset for no geometric
  // reason, and jumping to a much wider one would cut more than needed.
  const std::array<double, 6> fracs = s.uturn ?
      std::array<double, 6>{1.0, 0.75, 0.5, 0.25, 0.1, 0.0} :
      std::array<double, 6>{0.0, 0.1, 0.25, 0.5, 0.75, 1.0};

  for (const double frac : fracs) {
    Turn t;
    const double back_off = s.pin_in ? 0.0 : s.lo + frac * (s.in_reach - s.lo);
    const double fwd_off = s.pin_out ? 0.0 : s.lo + frac * (s.out_reach - s.lo);
    t.entry = s.first.getPointAlong(cursor, back_off);
    t.exit = s.last_corner.getPointAlong(s.after, fwd_off);
    t.arc = turn.createTurn(robot, t.entry, s.in_angle, t.exit, s.out_angle);
    if (t.arc.size() == 0) {
      continue;
    }

    // A feasible pose pair can still come back as a loop, or as an S that
    // doubles back before settling. Both turn far more than the span asks for,
    // which length measures only indirectly -- a loop's extra 2*pi is cheap at
    // a small radius. Compare the turning itself.
    double turned = 0.0;
    for (size_t k = 1; k < t.arc.size(); ++k) {
      turned += F2CPoint::getAngleDiffAbs(t.arc[k].angle, t.arc[k - 1].angle);
    }
    // A u-turn earns its teardrop; anything else should turn once.
    if (turned > s.total + (s.uturn ? M_PI : robot.getTurnSlack())) {
      continue;
    }

    std::vector<F2CPoint> track{t.entry};
    for (size_t k = i; k <= j; ++k) {
      track.push_back(poly[k]);
    }
    track.push_back(t.exit);
    // A u-turn is allowed the corridor plus the diameter it swings past.
    const double dev_max = s.uturn ?
        cut_tol + 2.0 * robot.getTurnRadius(s.net, continuous) : cut_tol;
    if (t.arc.maxDistanceTo(F2CLineString(track)) > dev_max) {
      continue;
    }
    return t;
  }
  return {};
}

// Follow `poly`: straight runs as they are, corners through the turn planner.
// A corner only a turn cutting past `cut_tol` could round is left square.
void appendRoundedTrack(
    F2CPath& path, const std::vector<F2CPoint>& poly, const F2CRobot& robot,
    TurningBase& turn, double cut_tol, bool continuous,
    const std::optional<double>& start_angle, const std::optional<double>& end_angle) {
  const size_t n = poly.size();
  const std::vector<double> sweeps = cornerSweeps(poly, start_angle, end_angle);

  F2CPoint cursor = poly.front();
  size_t i = 0;
  while (i < n) {
    if (sweeps[i] < robot.getMinSweep()) {
      addStraight(path, robot, cursor, poly[i]);
      cursor = poly[i];
      ++i;
      continue;
    }

    std::optional<Turn> rounded;
    size_t last = i;
    // Widening out from the fold, because a shorter span cuts less corner.
    for (size_t span = foldedSpan(poly, sweeps, i, robot);
        span <= robot.getMaxCornerSpan() && i + span <= n && !rounded; ++span) {
      const size_t j = i + span - 1;
      if (j + 1 == n && !end_angle) {
        break;  // an open route end holds no heading, so it is no corner
      }
      const std::optional<Span> s = spanGeometry(poly, sweeps, i, j, cursor,
          robot, cut_tol, continuous, start_angle, end_angle);
      if (!s) {
        continue;
      }
      rounded = planSpanTurn(*s, poly, i, j, cursor, robot, turn,
          cut_tol, continuous);
      last = j;
    }

    if (rounded) {
      addStraight(path, robot, cursor, rounded->entry);
      path += rounded->arc;  // as returned, reverse legs included
      cursor = rounded->exit;
      i = last + 1;
    } else {
      addStraight(path, robot, cursor, poly[i]);
      cursor = poly[i];
      ++i;
    }
  }
  addStraight(path, robot, cursor, poly.back());
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
  const std::vector<F2CPoint> mid = mp.toVectorPoint();
  pts.insert(pts.end(), mid.begin(), mid.end());
  pts.push_back(p2);

  // A track close to the straight hop isn't a detour, so drive it direct; a
  // short-hop reversal is the same call, since no fillet can round a u-turn.
  double max_dev = 0.0;
  for (size_t i = 1; i + 1 < pts.size(); ++i) {
    max_dev = std::max(max_dev, pts[i].distance(F2CLineString(p1, p2)));
  }
  bool direct = max_dev < robot.getDirectHopMaxDev();
  if (!direct) {
    const double reversal = F2CPoint::getAngleDiffAbs(ang1, ang2);
    const double hop = p1.distance(p2);
    direct = reversal > robot.getReversalSweep() && hop < robot.getUturnMaxHop();
  }
  if (direct) {
    return turn.createTurn(robot, p1, ang1, p2, ang2);
  }

  // Otherwise follow the detour, rounding its corners, rather than cutting
  // across ground the route deliberately routed around.
  const std::vector<F2CPoint> poly =
      F2CLineString(pts).simplify(robot.getTrackSimplifyTol()).toVectorPoint();
  if (poly.size() < 2) {
    return {};
  }

  F2CPath path;
  appendRoundedTrack(path, poly, robot, turn, robot.getMaxCornerCut(),
      turn.hasContinuousCurvature(), ang1, ang2);
  return path;
}

double PathPlanning::getSmoothTurningRadius(const F2CRobot& robot) {
  return robot.getSmoothTurningRadius();
}

}  // namespace f2c::pp

