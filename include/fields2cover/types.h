//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_H_
#define FIELDS2COVER_TYPES_H_

#include <gdal/ogr_geometry.h>
#include <vector>


#include "fields2cover/types/Point.h"
#include "fields2cover/types/MultiPoint.h"
#include "fields2cover/types/LinearRing.h"
#include "fields2cover/types/LineString.h"
#include "fields2cover/types/MultiLineString.h"
#include "fields2cover/types/Cell.h"
#include "fields2cover/types/Cells.h"

#include "fields2cover/types/Strip.h"
#include "fields2cover/types/Field.h"

#include "fields2cover/types/Swath.h"
#include "fields2cover/types/Swaths.h"
#include "fields2cover/types/SwathsByCells.h"
#include "fields2cover/types/Route.h"
#include "fields2cover/types/Path.h"
#include "fields2cover/types/Robot.h"

#include "fields2cover/types/Graph.h"
#include "fields2cover/types/Graph2D.h"

using F2CRobot = f2c::types::Robot;

using F2CPoint = f2c::types::Point;
using F2CMultiPoint = f2c::types::MultiPoint;
using F2CLineString = f2c::types::LineString;
using F2CLinearRing = f2c::types::LinearRing;
using F2CMultiLineString = f2c::types::MultiLineString;
using F2CCell = f2c::types::Cell;
using F2CCells = f2c::types::Cells;
using F2CStrip = f2c::types::Strip;
using F2CField = f2c::types::Field;
using F2CPath = f2c::types::Path;
using F2CPathState = f2c::types::PathState;
using F2CRoute = f2c::types::Route;
using F2CSwath = f2c::types::Swath;
using F2CSwaths = f2c::types::Swaths;
using F2CSwathsByCells = f2c::types::SwathsByCells;
using F2CGraph = f2c::types::Graph;
using F2CGraph2D = f2c::types::Graph2D;

using F2CRobots = std::vector<F2CRobot>;
using F2CStrips = std::vector<F2CStrip>;
using F2CFields = std::vector<F2CField>;

#endif  //  FIELDS2COVER_TYPES_H_
