#=============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#=============================================================================

import math
import fields2cover as f2c
import os
DATA_PATH = os.path.dirname(os.path.realpath(__file__))+"/" + 2 * "../"+"data/"
print(DATA_PATH)
# Import field
swaths = f2c.Parser().importSwathsJson(DATA_PATH + "workinglanes.geojson")
    
transport_lanes_import = f2c.Fields()
f2c.Parser().importJson(DATA_PATH + "transportlanes.geojson",  transport_lanes_import)
transport_lanes_cells = f2c.Cells()

for lane in transport_lanes_import:
    transport_lanes_cells.addGeometry(lane.getCellsAbsPosition().getGeometry(0))

robot = f2c.Robot(2.0, 2.0, 3.14, 3.14)

snake_sorter = f2c.RP_Snake()
swaths = snake_sorter.genSortedSwaths(swaths)

boustrophedon_sorter = f2c.RP_Boustrophedon()
swaths = boustrophedon_sorter.genSortedSwaths(swaths)

route_planner = f2c.RP_RoutePlannerBase()
swathsByCells = f2c.SwathsByCells()
swathsByCells.push_back(swaths)
route_planner.setStartAndEndPoint(f2c.Point(-25.433197685573184, -36.10820096737386))
route = route_planner.genRoute(transport_lanes_cells, swathsByCells)

curves = f2c.PP_DubinsCurves()
    
path: f2c.Path = f2c.PP_PathPlanning().planPath(robot, route, curves, True)

f2c.Visualizer.figure()
f2c.Visualizer.plot(swaths)
f2c.Visualizer.plot(route)
f2c.Visualizer.plot(transport_lanes_cells)
f2c.Visualizer.plot(path)
# f2c.Visualizer.show()
f2c.Visualizer.save("path.png");    




