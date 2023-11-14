//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/types/Route.h"

namespace f2c::types {

double Route::getLength() const {
  return getRouteAsLine().getLength();
}

LineString Route::getRouteAsLine() const {
  if (isEmpty() || !isValid()) {
    return LineString();
  }
  LineString route;
  if (connections.size() > 0 && !connections[0].isEmpty()) {
    for (const auto& point : connections[0]) {
      route.addPoint(point);
    }
  }
  for (size_t i = 0; i < v_swaths.size(); ++i) {
    for (const auto& swath : v_swaths[i]) {
      for (const auto& point : swath.getPath()) {
        route.addPoint(point);
      }
    }
    if ((connections.size() > i + 1) && !connections[i+1]->IsEmpty()) {
      for (const auto& point : connections[i+1]) {
        route.addPoint(point);
      }
    }
  }
  return route;
}

bool Route::isValid() const {
  return  (type == RouteType::R_NONE &&
            (v_swaths.size() - 1) == connections.size()) ||
        (((type == RouteType::R_START) || (type == RouteType::R_END)) &&
            v_swaths.size() == connections.size()) ||
          (type == RouteType::R_START_END &&
            v_swaths.size() == (connections.size()-1));
}

bool Route::isEmpty() const {
  return v_swaths.empty() && connections.empty();
}

Route Route::clone() const {
  Route new_r;
  for (auto&& s : this->v_swaths) {
    new_r.v_swaths.emplace_back(s.clone());
  }
  for (auto&& c : this->connections) {
    new_r.connections.emplace_back(c.clone());
  }
  new_r.type = this->type;
  return new_r;
}


}  // namespace f2c::types

