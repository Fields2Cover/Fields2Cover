import faulthandler
import fields2cover as f2c

from openapi_server.models.point import Point
import json
faulthandler.enable()
def generate_path(field_json, swaths, transport_lanes, start_and_end_point: Point, min_turning_radius, width, cov_width, max_curv, max_diff_curv):
        
    field = f2c.Parser().importCellJsonFromString(field_json)
    field_cells = f2c.Cells()
    field = f2c.Field(field_cells)

    imported_swaths = f2c.Parser().importSwathsJsonFromString(swaths)
    
    transport_lanes_import = f2c.Fields()
    
    f2c.Parser().importJsonFromString(transport_lanes, transport_lanes_import)
    
    transport_lanes_cells = f2c.Cells()
    
    for field in transport_lanes_import:
        transport_lanes_cells.addGeometry(field.getCellsAbsPosition().getGeometry(0))
    
    robot = f2c.Robot(width, cov_width, max_curv, max_diff_curv)
    
    robot.setMinTurningRadius(min_turning_radius)  # m
    
    # snake_sorter = f2c.RP_Snake()
    # swaths = snake_sorter.genSortedSwaths(imported_swaths)

    boustrophedon_sorter = f2c.RP_Boustrophedon()
    swaths = boustrophedon_sorter.genSortedSwaths(imported_swaths)
    
    route_planner = f2c.RP_RoutePlannerBase()
    swathsByCells = f2c.SwathsByCells()
    swathsByCells.push_back(swaths)
        
    route_planner.setStartAndEndPoint(f2c.Point(start_and_end_point.x, start_and_end_point.y)) 
    route = route_planner.genRoute(transport_lanes_cells, swathsByCells)
    
    path_planner = f2c.PP_PathPlanning()
    dubins = f2c.PP_DubinsCurves()
    
    path: f2c.Path = path_planner.planPath(robot, route, dubins)  
    f2c.Visualizer.figure();
    f2c.Visualizer.plot(path);
    f2c.Visualizer.show();
    
    return path_to_json(path)

def path_to_json(path: f2c.Path):    
    # convert to json
    states = path.getStates()
    result = []
    state: f2c.PathState
    for state in states:
        point: f2c.Point = {
            'x': state.point.getX(),
            'y': state.point.getY(),
        }
        angle = state.angle
        result.append({'point': point, 'angle': angle})

    
    return result
