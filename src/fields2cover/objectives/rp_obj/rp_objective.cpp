//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/objectives/rp_obj/rp_objective.h"

namespace f2c::obj {

double RPObjective::computeCost(
    const F2CPoint& p1, const F2CPoint& p2) {
  return p1.distance(p2);
}

double RPObjective::computeCost(
    const F2CPoint& p1, double ang1, const F2CPoint& p2) {
  return computeCost(p1, p2);
}


double RPObjective::computeCost(
    const F2CPoint& p1, const F2CPoint& p2, double ang2) {
  return computeCost(p1, p2);
}

double RPObjective::computeCost(
    const F2CPoint& p1, double ang1, const F2CPoint& p2, double ang2) {
  return computeCost(p1, p2);
}

double RPObjective::computeCost(const F2CSwath& s, const F2CPoint& p) {
  return computeCost(s.endPoint(), s.getOutAngle(), p);
}

double RPObjective::computeCost(const F2CSwath& s1, const F2CSwath& s2) {
  return this->computeCost(
      s1.endPoint(), s1.getOutAngle(), s2.startPoint(), s2.getInAngle());
}

double RPObjective::computeCost(
    const F2CSwath& s, const F2CPoint& p, double ang) {
  return computeCost(s.endPoint(), s.getOutAngle(), p, ang);
}

double RPObjective::computeCost(const F2CPoint& p, const F2CSwath& s) {
  return computeCost(p, s.startPoint(), s.getInAngle());
}

double RPObjective::computeCost(
    const F2CPoint& p, double ang, const F2CSwath& s) {
  return computeCost(p, ang, s.startPoint(), s.getInAngle());
}

double RPObjective::computeCost(const std::vector<F2CPoint>& ps) {
  auto dist {0.0};
  for (size_t i = 1; i < ps.size(); ++i) {
    dist += computeCost(ps[i-1], ps[i]);
  }
  return dist;
}

double RPObjective::computeCost(const F2CMultiPoint& ps) {
  if (ps.isEmpty()) {
    return 0.0;
  }
  auto dist {0.0};
  for (size_t i = 1; i < ps.size(); ++i) {
    dist += computeCost(ps.getGeometry(i-1), ps.getGeometry(i));
  }
  return dist;
}

double RPObjective::computeCost(const F2CSwath& s, const F2CMultiPoint& ps) {
  if (ps.isEmpty()) {
    return 0.0;
  }
  return computeCost(s.endPoint(), s.getOutAngle(), ps.getGeometry(0));
}

double RPObjective::computeCost(const F2CSwaths& s, const F2CMultiPoint& ps) {
  if (ps.isEmpty()) {
    return 0.0;
  }
  return computeCost(s.back(), ps);
}

double RPObjective::computeCost(const F2CMultiPoint& ps, const F2CSwath& s) {
  if (ps->IsEmpty()) {
    return 0.0;
  }
  return computeCost(ps.getGeometry(ps.size()-1),
      s.startPoint(), s.getInAngle());
}

double RPObjective::computeCost(const F2CMultiPoint& ps, const F2CSwaths& s) {
  return computeCost(ps, s[0]);
}

double RPObjective::computeCost(const F2CSwath& s) {
  double dist {0.0};
  for (size_t i = 1; i < s.numPoints(); ++i) {
    dist += computeCost(s.getPoint(i-1), s.getPoint(i));
  }
  return dist;
}

double RPObjective::computeCost(const F2CSwaths& swaths) {
  double dist {0.0};
  for (auto&& s : swaths) {
    dist += computeCost(s);
  }
  for (size_t i = 1; i < swaths.size(); ++i) {
    dist += computeCost(swaths[i-1].endPoint(), swaths[i-1].getOutAngle(),
        swaths[i].startPoint(), swaths[i].getInAngle());
  }
  return dist;
}

double RPObjective::computeCost(const F2CRoute& r) {
  double dist {0.0};
  for (auto&& s : r.getVectorSwaths()) {
    dist += computeCost(s);
  }
  for (auto&& s : r.getConnections()) {
    dist += computeCost(s);
  }
  for (size_t i = 0; i < r.sizeVectorSwaths(); ++i) {
    dist += computeCost(r.getConnection(i), r.getSwaths(i));
    if (i + 1 < r.sizeConnections()) {
      dist += computeCost(r.getSwaths(i), r.getConnection(i + 1));
    }
  }
  return dist;
}

}  // namespace f2c::obj

