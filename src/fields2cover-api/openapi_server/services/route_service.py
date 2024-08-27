import faulthandler
import fields2cover as f2c
from openapi_server.models.point import Point
from openapi_server.dto.robot_settings import RobotSettings
from openapi_server.dto.sorter_settings import SorterSettings
from openapi_server.services.f2c_import import import_transport_lanes

def sort_swaths(swaths, sorter_settings: SorterSettings) -> f2c.Swaths:
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


def order_swaths(swaths: f2c.Swaths, sorter_settings: SorterSettings):
    if(sorter_settings.algorithm != "optimal"):
        ordered_swaths = sort_swaths(swaths, sorter_settings)
    else:
        ordered_swaths = swaths

    swathsByCells = f2c.SwathsByCells()
    swathsByCells.push_back(ordered_swaths)
    return swathsByCells

def generate_route(workingLanes: str, transport_lanes: str, start_and_end_point: Point, sorter_settings: SorterSettings):    

    swaths = f2c.Parser().importSwathsJsonFromString(workingLanes)
    transport_lanes_cells = import_transport_lanes(transport_lanes)
    ordered_swaths = order_swaths(swaths, sorter_settings)

    route_planner = f2c.RP_RoutePlannerBase()

    route_planner.setStartAndEndPoint(f2c.Point(start_and_end_point.coordinates[0], start_and_end_point.coordinates[1]))
    route = route_planner.genRoute(transport_lanes_cells, ordered_swaths)

    return route


