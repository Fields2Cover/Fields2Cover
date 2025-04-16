//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <utility>
#include <algorithm>
#include "fields2cover/headland_generator/headland_generator_base.h"
#include "fields2cover/utils/solver.h"
#include "steering_functions/utilities/utilities.hpp"
#include "fields2cover/utils/visualizer.h"

namespace f2c::hg {

F2CCells HeadlandGeneratorBase::generateHeadlands(
    const F2CCell& field, const F2CRobot& robot,
    double track_ang) const {
  return this->generateHeadlands(F2CCells(field), maxHLWidthRequired(robot));
}

F2CCells HeadlandGeneratorBase::generateHeadlands(
    const F2CCells& field, const F2CRobot& robot,
    const std::vector<double>& track_angs) const {
  return this->generateHeadlands(field, maxHLWidthRequired(robot));
}

double HeadlandGeneratorBase::maxHLWidthRequired(const F2CRobot& robot) const {
  return 2.0 * robot.getMinTurningRadius() + 0.5 * robot.getWidth();
}

F2CPoint HeadlandGeneratorBase::findCenterCornerTurn(
    const F2CPathSimp& path, double ang) {
  auto end_p = path.atEnd().p;
  return F2CPoint(end_p.getX() - end_p.getY()/tan(ang), 0.0);
}

F2CPathSimp HeadlandGeneratorBase::continuousFrontTurn(double ang_turn,
    const F2CRobot& robot, bool is_impl_on,
    double dist_wall_start, double dist_wall_end,
    double buff_cloth_pos, double buff_cloth_neg) {
  F2CPathSimp turn;
  if (ang_turn < boost::math::constants::pi<double>()) {
    turn = contConvexTurn(ang_turn,
        robot.getMaxCurv(is_impl_on), robot.getMaxDiffCurv(is_impl_on),
        buff_cloth_pos);
  } else {
    turn = contConcaveTurn(ang_turn, robot,
        robot.getMaxCurv(is_impl_on), robot.getMaxDiffCurv(is_impl_on),
        robot.getRobotWidth(), dist_wall_start, dist_wall_end,
        buff_cloth_pos, buff_cloth_neg);
  }
  if (!is_impl_on) {
    turn.setImplOff();
  }
  return turn;
}


F2CPathSimp HeadlandGeneratorBase::contFishTailTurn(
    double ang_turn, const F2CRobot& robot,
    double dist_wall_start, double dist_wall_end) {
  F2CPathSimp turn;
  F2CPathSimp path;
  if (ang_turn < boost::math::constants::pi<double>()) {
    turn = contConvexTurn(ang_turn,
        robot.getMaxCurv(false), robot.getMaxDiffCurv(false));
  } else {
    turn = contConvexTurn(2.0 * M_PI - ang_turn,
        robot.getMaxCurv(false), robot.getMaxDiffCurv(false));
    turn.mirrorX();
    turn.reverseVehicleDir();
  }
  turn.setImplOff();
  F2CPoint c_turn = findCenterCornerTurn(turn, ang_turn);
  double dist_end_c = turn.atEnd().p.getY()/(sin(ang_turn));
  double R_w = robot.getRobotWidth();
  double R_l = robot.getRobotLength();
  double I_bb_x = robot.getBackImpl().getX();
  double Ic_bb_x = robot.getBackImplCov().getX();
  double Ic_fb_x = robot.getFrontImplCov().getX();
  double Ic_w_2 = 0.5 * robot.getImplCovWidth();
  double I_w_2 = 0.5 * robot.getImplWidth();
  double inv_sa = 1. / sin(ang_turn);
  double inv_ta = 1. / tan(ang_turn);
  double abs_inv_sa = fabs(inv_sa);
  double abs_inv_ta = fabs(inv_ta);
  constexpr double pi = boost::math::constants::pi<double>();

  double x_a = c_turn.getX() - Ic_bb_x - (abs_inv_sa - abs_inv_ta) * Ic_w_2;
  if (ang_turn < pi) {
    double max_x_a = c_turn.getX() - 0.5 * R_l - 0.5 * R_w * abs_inv_ta
        + dist_wall_end * inv_sa;
    x_a = std::min(x_a, max_x_a);
  }

  double x_b = -dist_end_c - Ic_fb_x - (abs_inv_sa + abs_inv_ta) * Ic_w_2;
  double P =  x_a - (c_turn.getX() - Ic_bb_x) + Ic_w_2 * abs_inv_sa;
  x_b += std::max(0., 2.0 * P * cos(ang_turn));
  if (ang_turn < pi) {
    double min_x_b = -dist_end_c - I_bb_x + I_w_2 * abs_inv_ta
      - dist_wall_start * inv_sa;
    x_b = std::max(x_b, min_x_b);
  }

  if (x_a >= 0) {
    path.appendStraightLine(x_a, true);
    path.appendStraightLine(-x_a, false);
  } else {
    path.prependStraightLine(-x_a, false);
    path.states.back().start.p.setX(x_a);
  }
  path += turn;

  path.appendStraightLine(x_b, false);
  if (x_b < 0) {
    path.appendStraightLine(-x_b, true);
  }
  return path;
}


F2CPathSimp HeadlandGeneratorBase::outerCornerPlanning(const F2CRobot& robot,
    double ang_turn, CornerPlanners planner, size_t i_hl, size_t max_hl,
    double dist_border_start, double dist_border_end) {
  F2CPathSimp turn;
  switch (planner) {
    case (CornerPlanners::FORWARD_CONT_IMPL_ON) :
      turn = continuousFrontTurn(
          ang_turn, robot, true, dist_border_start, dist_border_end);
      break;
    case (CornerPlanners::FORWARD_CONT_IMPL_OFF) :
      turn = continuousFrontTurn(
          ang_turn, robot, false, dist_border_start, dist_border_end);
      break;
    case (CornerPlanners::FISHTAIL) :
      turn = contFishTailTurn(
          ang_turn, robot, dist_border_start, dist_border_end);
      break;
    case (CornerPlanners::FORWARD_CONT_BUFF_IMPL_ON) : {
      double w = robot.getImplCovWidth();
      turn = continuousFrontTurn(
          ang_turn, robot, true, dist_border_start, dist_border_end,
          i_hl * w, (max_hl - i_hl) * w);
      break;
    }
  }
  return turn;
}

F2CPathSimp HeadlandGeneratorBase::contConvexTurn(
    double ang, double max_kurv, double max_sigma, double buff_cloth) {
  f2c::types::PathSimp path;
  path.appendSmoothCircularSegment(ang, max_kurv, max_sigma, buff_cloth);
  return path;
}

F2CPathSimp HeadlandGeneratorBase::getConcaveHalfTurn(
    double ang_trans, double ang_end,
    double kurv, double sigma, double buff_cloth_pos, double buff_cloth_neg) {
  f2c::types::PathSimp path;
  double circ_len = F2CPoint::mod_2pi(-2*ang_end + 2. * ang_trans);
  path
    .appendSmoothCircularSegment(ang_trans, kurv, sigma, buff_cloth_pos)
    .appendStartHalfSmoothCircularSegment(
        circ_len, kurv, -sigma, buff_cloth_neg);
  return path;
}

F2CPoint HeadlandGeneratorBase::findOffsetCornerOnConcaveTurn(
    double ang, double offset1, double offset2) {
  return {-offset1/tan(ang) + offset2/sin(ang), -offset1};
}

std::pair<double, double> HeadlandGeneratorBase::computeConcaveOffset(
    double ang_t1, double ang_t2, const F2CRobot& robot,
    double kurv, double sigma, double buff_cloth_pos) {
  F2CPathSimp path1;
  path1.appendSmoothCircularSegment(ang_t1, kurv, sigma, buff_cloth_pos);
  F2CPathSimp path2;
  path2.appendSmoothCircularSegment(ang_t2, kurv, sigma, buff_cloth_pos);
  return {-robot.getMinY(path1), -robot.getMinYWithRobotBackwards(path2)};
}

bool HeadlandGeneratorBase::checkConcaveTurnPathCrossBorder(
    double ang, const F2CRobot& robot, const F2CPathSimp& path,
    const std::pair<double, double>& offsets) {
  F2CPoint c = findOffsetCornerOnConcaveTurn(
      ang, offsets.first, offsets.second);
  F2CPoint c_path = findCenterCornerTurn(path, ang) + c;
  auto start = path.atStart().p + c;
  auto end = path.atEnd().p + c;

  // Find point that crosses x axis.
  // The path is moved up to avoid finding the start point as solution.
  auto fy = [&path] (double t) {
    return path.at(t).p.getY() + 1e-3;
  };
  double L = path.length();
  double w2 = 0.5 * robot.getWidth();
  double t_y0 = f2c::Solver::findZeroBisectionMethod(fy, 0, L, 1e-3);
  auto p_y0 = path.at(t_y0).p;
  if (p_y0.getX() < c_path.getX() || p_y0.distance(c_path) <= w2) {
    return true;
  }
  auto f = [&path, &c_path] (double t) {
    return path.at(t).p.distance(c_path);
  };

  double t = f2c::Solver::findMinGoldenSection(f, t_y0 - 0.25 * L, t_y0 + 0.25 * L, 1e-3);
  return c_path.distance(path.at(t).p) <= w2;
}

std::vector<double> HeadlandGeneratorBase::getConcaveTurnParams(
    double ang, const F2CRobot& robot, double kurv, double sigma,
    double buff_cloth_pos, double buff_cloth_neg) {
  return f2c::Solver::exhaustive_solve(
      {0, 0},
      {0.5*M_PI, 0.5*M_PI},
      [&] (const std::vector<double>& x) {
        auto offsets = computeConcaveOffset(x[0], x[1],
            robot, kurv, sigma, buff_cloth_pos);
        F2CPathSimp path = concaveTurn(ang, x[0], x[1],
            kurv, sigma, buff_cloth_pos, buff_cloth_neg);
        if (checkConcaveTurnPathCrossBorder(ang, robot, path, offsets)) {
          return 1e10;
        }
        return path.length();
      }, {0.0125*M_PI, 0.0125*M_PI});
}


std::vector<double> HeadlandGeneratorBase::getTransAngleOnConcaveTurn(
    double ang, const F2CRobot& robot, double kurv, double sigma,
    double offset_start, double offset_end,
    double buff_cloth_pos, double buff_cloth_neg) {
  return f2c::Solver::exhaustive_solve(
      {0, 0}, {0.5*M_PI, 0.5*M_PI},
      [&] (const std::vector<double>& ang_t) {
        auto offsets = computeConcaveOffset(ang_t[0], ang_t[1], robot,
            kurv, sigma, buff_cloth_pos);
        if (offsets.first > offset_start || offsets.second > offset_end) {
          return 1e10;
        }
        F2CPathSimp path = concaveTurn(ang, ang_t[0], ang_t[1],
            kurv, sigma, buff_cloth_pos, buff_cloth_neg);
        if (checkConcaveTurnPathCrossBorder(
              ang, robot, path, {offset_start, offset_end})) {
          return 1e10;
        }
        return path.length();
      }, {0.0125*M_PI, 0.0125*M_PI});
}

F2CPathSimp HeadlandGeneratorBase::concaveTurn(
    double ang_end, double ang_t1, double ang_t2,
    double max_kurv, double max_sigma,
    double buff_cloth_pos, double buff_cloth_neg) {
  double circ_len = F2CPoint::mod_2pi(-ang_end + ang_t1 + ang_t2);
  f2c::types::PathSimp path;
  path.appendSmoothCircularSegment(
          ang_t1,   max_kurv,  max_sigma, buff_cloth_pos);
  path.appendSmoothCircularSegment(
          circ_len, max_kurv, -max_sigma, buff_cloth_neg);
  path.appendSmoothCircularSegment(
          ang_t2,   max_kurv,  max_sigma, buff_cloth_pos);
  return path;
}


F2CPathSimp HeadlandGeneratorBase::contConcaveTurn(
    double ang, const F2CRobot& robot, double max_kurv, double max_sigma,
    double rob_width, double offset_start, double offset_end,
    double buff_cloth_pos, double buff_cloth_neg) {
  auto ang_trans = getTransAngleOnConcaveTurn(
      ang, robot, max_kurv, max_sigma,
      offset_start, offset_end, buff_cloth_pos, buff_cloth_neg);
  return concaveTurn(ang, ang_trans[0], ang_trans[1],
      max_kurv, max_sigma, buff_cloth_pos, buff_cloth_neg);
}


std::pair<double, double> HeadlandGeneratorBase::computeOffsetTurn(
    const F2CRobot& robot, double ang_turn,
    CornerPlanners planner, size_t n_hl_swaths) {

  bool is_impl_on = planner == CornerPlanners::FORWARD_CONT_IMPL_ON || \
                    planner == CornerPlanners::FORWARD_CONT_BUFF_IMPL_ON;
  double kurv = robot.getMaxCurv(is_impl_on);
  double sigma = robot.getMaxDiffCurv(is_impl_on);
  double buff_cloth_pos =
                    (planner == CornerPlanners::FORWARD_CONT_BUFF_IMPL_ON ?
                     n_hl_swaths * robot.getImplCovWidth() : 0.0);

  if (ang_turn < boost::math::constants::pi<double>()) {
    auto path = contConvexTurn(ang_turn, kurv, sigma, buff_cloth_pos);
    return {-robot.getMinY(path), -robot.getMinYWithRobotBackwards(path)};
  } else if (planner == CornerPlanners::FISHTAIL) {
    return {robot.getWidth() * 0.5, robot.getWidth() * 0.5};
  }
  auto ang_t = getConcaveTurnParams(
        ang_turn, robot, kurv, sigma, buff_cloth_pos, 0.0);
  if (ang_t.size() < 1) {
    return {robot.getWidth(), robot.getWidth()};
  }
  auto path1 = contConvexTurn(ang_t[0], kurv, sigma, buff_cloth_pos);
  auto path2 = contConvexTurn(ang_t[1], kurv, sigma, buff_cloth_pos);
  return {-robot.getMinY(path1), -robot.getMinYWithRobotBackwards(path2)};
}




std::vector<std::pair<double, double>> HeadlandGeneratorBase::computeOffsetTurn(
    const F2CRobot& robot, const F2CLinearRing& border, bool is_border_right,
    CornerPlanners planner, size_t n_hl_swaths) {
  std::vector<CornerPlanners> planners(border.size() - 1, planner);
  return computeOffsetTurn(robot, border, is_border_right, planners, n_hl_swaths);
}

std::vector<std::pair<double, double>> HeadlandGeneratorBase::computeOffsetTurn(
    const F2CRobot& robot, const F2CLinearRing& border, bool is_border_right,
    std::vector<CornerPlanners> planners, size_t n_hl_swaths) {
  auto border_c = border.clone();
  size_t n_b = border.size() - 1;
  std::vector<std::pair<double, double>> offsets(n_b);
  bool inverted_border = !border_c.isClockwise() ^ is_border_right;

  if (inverted_border) {
    border_c.reversePoints();
  }
  for (size_t i = 0; i < n_b; ++i) {
    double ang_turn = F2CPoint::mod_2pi((is_border_right ? 1.0 : -1.0) *
        F2CPoint::getAngleFromPoints(border_c.getGeometry(i),
                                     border_c.getGeometry((i+1)%n_b),
                                     border_c.getGeometry((i+2)%n_b)));
    offsets[i] = computeOffsetTurn(robot, ang_turn, planners[(i+1)%n_b], n_hl_swaths);
  }
  return offsets;
}

std::vector<double> HeadlandGeneratorBase::pickOffsets(
    const std::vector<std::pair<double, double>>& pair_offsets,
    double w_robot, bool inside_out) {
  size_t n = pair_offsets.size();
  std::vector<double> offsets(n);

  for (size_t i = 0; i < n; ++i) {
    double o1 = pair_offsets[i].second;
    double o2 = pair_offsets[(i+1)%n].first;
    //if (inside_out && i == 0) {o2 -= w_robot;}
    if (inside_out && i == n-2) {o2 -= w_robot*0.5;}
    offsets[(i+1)%n] = std::max(o1, o2);
  }
  return offsets;
}

F2CLinearRing HeadlandGeneratorBase::generateHeadlandExtBorder(
    const F2CLinearRing& border, std::vector<double> offsets) const {
  F2CLinearRing border_c = border;
  if (border_c.size() != offsets.size() + 1) {
    throw std::invalid_argument(
        "Error HeadlandGeneratorBase::generateHeadlandExtBorder. "  \
        "border.size() and offsets.size()+1 are not equal");
  }

  if (border_c.isClockwise()) {
    border_c.reversePoints();
    std::reverse(offsets.begin(), offsets.end());
  }

  std::vector<F2CLineString> inter_borders;
  for (size_t i = 0; i < border_c.size()-1; ++i) {
    F2CPoint p1 = border_c.getGeometry(i);
    F2CPoint p2 = border_c.getGeometry(i + 1);
    double d_border = p1.distance(p2);
    F2CPoint ort_p1p2 {
      -(p2.getY() - p1.getY()) / d_border,
       (p2.getX() - p1.getX()) / d_border};
    inter_borders.emplace_back(
        p1 + ort_p1p2 * offsets[i], p2 + ort_p1p2 * offsets[i]);
  }

  std::vector<F2CPoint> new_border;
  for (size_t i = 0; i < inter_borders.size(); ++i) {
    size_t j = (i + 1) % inter_borders.size();
    new_border.emplace_back(F2CPoint::intersectionOfLines(
          inter_borders[i].startPoint(), inter_borders[i].endPoint(),
          inter_borders[j].startPoint(), inter_borders[j].endPoint()));
  }
  std::vector<F2CPoint> rot_border(new_border.size());
  std::rotate_copy(new_border.begin(), new_border.end()-1, new_border.end(), rot_border.begin());
  rot_border.emplace_back(rot_border[0]);
  return static_cast<F2CLinearRing>(rot_border);
}

std::vector<F2CLinearRing> HeadlandGeneratorBase::generateHeadlandBorders(
    const F2CLinearRing& border, std::vector<double> offsets,
    size_t n_headlands, double width_headland) const {
  std::vector<F2CLinearRing> rings {generateHeadlandExtBorder(border, offsets)};

  for (size_t i = 1; i < n_headlands; ++i) {
    for (auto&& d : offsets) {
      d += width_headland;
    }
    rings.emplace_back(generateHeadlandExtBorder(border, offsets));
  }
  return rings;
}

F2CLineString HeadlandGeneratorBase::generateHeadlandBorderRoute(
    const std::vector<F2CLinearRing>& borders, bool inside_out) const {
  if (borders.size() == 0) {
    return {};
  }
  std::vector<F2CLinearRing> borders_c;
  for (auto&& b : borders) {
    borders_c.emplace_back(b.clone());
    if (!inside_out) {
      borders_c.back().reversePoints();
    }
  }


  F2CLineString line {borders_c[0]};
  for (size_t i = 1; i < borders_c.size(); ++i) {
    size_t n = line.size() - 1;
    line.setGeometry(n, F2CPoint::intersectionOfLines(
          line.getGeometry(n-1), line.getGeometry(n),
          borders_c[i].getGeometry(0), borders_c[i].getGeometry(1)));
    for (size_t j = 1; j < borders_c[i].size(); ++j) {
      line.addPoint(borders_c[i].getGeometry(j));
    }
  }
  if (inside_out) {
    line.reversePoints();
  }
  return line;
}

F2CPathSimp HeadlandGeneratorBase::connectHeadlandBorderRoute(
    const F2CRobot& robot, CornerPlanners planner, const F2CLineString& route,
    const std::vector<double>& offsets,
    bool is_border_right, bool inside_out) const {
  std::vector<CornerPlanners> planners =
    std::vector<CornerPlanners>(offsets.size(), planner);
  return connectHeadlandBorderRoute(robot, planners, route, offsets,
      is_border_right, inside_out);
}


F2CPathSimp HeadlandGeneratorBase::connectHeadlandBorderRoute(
    const F2CRobot& robot, const std::vector<CornerPlanners>& planners,
    const F2CLineString& route, const std::vector<double>& offsets,
    bool is_border_right, bool inside_out) const {
  if (planners.size() != offsets.size()) {
    throw std::invalid_argument("Error in "
        "HeadlandGeneratorBase::connectHeadlandBorderRoute: "
        "planners and offsets should have the same size().");
  }


  F2CPathSimp path;
  double w = robot.getImplCovWidth();
  size_t n_o = offsets.size();
  size_t max_hl = (route.size()) / n_o - 1;
  for (size_t i = 1; i < route.size() - 1; ++i) {
    size_t i_hl = (i - 1) / n_o;
    size_t j_hl = i / n_o;
    i_hl = inside_out ? i_hl : (max_hl - i_hl);
    j_hl = inside_out ? j_hl : (max_hl - j_hl);
    path += genCornerTurn(robot,
        route.getGeometry(i-1), route.getGeometry(i), route.getGeometry(i+1),
        is_border_right, planners[(i) % n_o], i_hl, max_hl,
        (max_hl - i_hl) * w + offsets[(i-1) % n_o],
        (max_hl - j_hl) * w + offsets[i % n_o]);
  }
  path.prependStraightLine(route.startPoint().distance(path.atStart().p));
  path.appendStraightLine(route.endPoint().distance(path.atEnd().p));

  return path;
}

F2CPathSimp HeadlandGeneratorBase::genCornerTurn(const F2CRobot& robot,
    const F2CPoint& p1, const F2CPoint& p2, const F2CPoint& p3,
    bool is_border_right, CornerPlanners planner,
    size_t i_hl, size_t max_hl,
    double offset_start, double offset_end) const {
  double ang_start = (p2 - p1).getAngleFromPoint();
  double ang_end   = (p3 - p2).getAngleFromPoint();
  double ang_turn = F2CPoint::mod_2pi(
      (is_border_right ? 1.0 : -1.0) * (ang_end - ang_start));

  F2CPathSimp path = outerCornerPlanning(
      robot, ang_turn, planner, i_hl, max_hl, offset_start, offset_end);
  F2CPoint c_path = findCenterCornerTurn(path, ang_turn);
  if (!is_border_right) {
    path.mirrorY();
  }
  path.moveTo(p2 - c_path);
  path.rotate(p2, ang_start);
  return path;
}



}  // namespace f2c::hg

