import faulthandler
import fields2cover as f2c
from openapi_server.models.point import Point

def parse_field_json(field_json: str) -> f2c.Field:
        field_parsed = f2c.Parser().importCellJsonFromString(field_json)
        field_cells = f2c.Cells(field_parsed)
        field = f2c.Field(field_cells)
        return field

def order_swaths()
    
    snake_sorter = f2c.RP_Snake()
    swaths = snake_sorter.genSortedSwaths(swaths)

    boustrophedon_sorter = f2c.RP_Boustrophedon()
    swaths = boustrophedon_sorter.genSortedSwaths(swaths)

def generate_path(field_json: str, workingLanes: str, transport_lanes: str, start_and_end_point: Point, min_turning_radius: float, width: float, cov_width: float, max_curv: float, max_diff_curv: float):
    faulthandler.enable()
    
    field = parse_field_json(field_json)

    swaths = f2c.Parser().importSwathsJsonFromString(workingLanes)

    transport_lanes_import = f2c.Fields()

    f2c.Parser().importJsonFromString(transport_lanes, transport_lanes_import)

    transport_lanes_cells = f2c.Cells()

    for field in transport_lanes_import:
        transport_lanes_cells.addGeometry(field.getCellsAbsPosition().getGeometry(0))

    robot = f2c.Robot(width, cov_width, max_curv, max_diff_curv)

    robot.setMinTurningRadius(min_turning_radius)  # m

    

    route_planner = f2c.RP_RoutePlannerBase()
    swathsByCells = f2c.SwathsByCells()
    swathsByCells.push_back(swaths)

    route_planner.setStartAndEndPoint(f2c.Point(start_and_end_point.x, start_and_end_point.y))
    route = route_planner.genRoute(transport_lanes_cells, swathsByCells)

    path_planner = f2c.PP_PathPlanning()
    dubins = f2c.PP_DubinsCurves()

    path: f2c.Path = path_planner.planPath(robot, route, dubins)
    f2c.Visualizer.figure()
    f2c.Visualizer.plot(path)
    f2c.Visualizer.show()

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
