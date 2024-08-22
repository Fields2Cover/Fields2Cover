import faulthandler
import fields2cover as f2c
from openapi_server.models.point import Point
from dataclasses import dataclass

@dataclass
class RobotSettings:
    min_turning_radius: float
    width: float
    cov_width: float
    max_curv: float
    max_diff_curv: float

@dataclass
class SorterSettings:
    algorithm: str
    variant: int
    spiralSize: int

def parse_field_json(field_json: str) -> f2c.Field:
        field_parsed = f2c.Parser().importCellJsonFromString(field_json)
        field_cells = f2c.Cells(field_parsed)
        field = f2c.Field(field_cells)
        return field

def order_swaths(swaths, sorter_settings: SorterSettings) -> f2c.Swaths:
    alg = sorter_settings.algorithm
    variant = sorter_settings.variant
    # no proper support for enums just yet in openapi generator
    if alg == "snake":
        snake_sorter = f2c.RP_Snake()
        swaths = snake_sorter.genSortedSwaths(swaths, variant)
    elif alg == "boustrophedon":
        boustrophedon_sorter = f2c.RP_Boustrophedon()
        swaths = boustrophedon_sorter.genSortedSwaths(swaths, variant)
    elif alg == "spiral":
        spiral_sorter = f2c.RP_Spiral(sorter_settings.spiralSize)
        swaths = spiral_sorter.genSortedSwaths(swaths, variant)
    else:
        raise ValueError("Invalid sorter type")

    return swaths

def determine_curves(curvesAlgorithm):
    if curvesAlgorithm == "dubins":
        return f2c.PP_DubinsCurves()
    elif curvesAlgorithm == "dubins_cc":
        return f2c.PP_DubinsCurvesCC()
    elif curvesAlgorithm == "reeds_shepp":
        return f2c.PP_ReedsSheppCurves()
    else:
        raise ValueError("Invalid curve type")


def create_robot(robotSettings: RobotSettings) -> f2c.Robot:
    robot = f2c.Robot(robotSettings.width, robotSettings.cov_width, robotSettings.max_curv, robotSettings.max_diff_curv)
    robot.setMinTurningRadius(robotSettings.min_turning_radius)  # m
    return robot

def generate_path(workingLanes: str, transport_lanes: str, start_and_end_point: Point, robotSettings: RobotSettings, sorter_settings: SorterSettings, curvesAlgorithm: str):
    faulthandler.enable()
    swaths = f2c.Parser().importSwathsJsonFromString(workingLanes)
    
    transport_lanes_import = f2c.Fields()
    f2c.Parser().importJsonFromString(transport_lanes, transport_lanes_import)
    transport_lanes_cells = f2c.Cells()

    for lane in transport_lanes_import:
        transport_lanes_cells.addGeometry(lane.getCellsAbsPosition().getGeometry(0))

    robot = create_robot(robotSettings)

    if(sorter_settings.algorithm != "optimal"):
        ordered_swaths = order_swaths(swaths, sorter_settings)
    else:
        ordered_swaths = swaths

    route_planner = f2c.RP_RoutePlannerBase()
    swathsByCells = f2c.SwathsByCells()
    swathsByCells.push_back(ordered_swaths)

    route_planner.setStartAndEndPoint(f2c.Point(start_and_end_point.x, start_and_end_point.y))
    route = route_planner.genRoute(transport_lanes_cells, swathsByCells)

    curves = determine_curves(curvesAlgorithm)
        
    path: f2c.Path = f2c.PP_PathPlanning().planPath(robot, route, curves)
    
    f2c.Visualizer.figure()
    f2c.Visualizer.plot(ordered_swaths)
    f2c.Visualizer.plot(transport_lanes_cells)
    f2c.Visualizer.plot(path)
    # f2c.Visualizer.show()
    f2c.Visualizer.save("path.png");    

    return path_to_json(path)


def path_to_json(path: f2c.Path):
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
