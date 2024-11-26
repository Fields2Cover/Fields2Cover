import datetime
import fields2cover as f2c
from openapi_server.models.point import Point
from openapi_server.dto.sorter_settings import SorterSettings
from openapi_server.services.f2c_import import import_transport_lanes, to_point

rp = f2c.RP_RoutePlannerBase()

def sort_swaths(swaths, sorter_settings: SorterSettings) -> f2c.Swaths:
    alg = sorter_settings.algorithm
    variant = sorter_settings.variant
    # no proper support for enums just yet in openapi generator
    if alg == "snake":
        snake_sorter = f2c.RP_Snake()
        swaths = snake_sorter.genSortedSwaths(swaths, variant)
    elif alg == "boustrophedon":#
        boustrophedon_sorter = f2c.RP_Boustrophedon()
        swaths = boustrophedon_sorter.genSortedSwaths(swaths, variant)
    elif alg == "spiral":
        spiral_sorter = f2c.RP_Spiral(sorter_settings.spiral_size)
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

def generate_route(workingLanes: str, transport_lanes: str, start_point: Point, end_point: Point, sorter_settings: SorterSettings):    
    swaths = f2c.Parser().importSwathsJsonFromString(workingLanes)
    transport_lanes_cells = import_transport_lanes(transport_lanes)
    ordered_swaths = order_swaths(swaths, sorter_settings)

    rp.setStartAndEndPoint(to_point(start_point), to_point(end_point))
    route = rp.genRoute(transport_lanes_cells, ordered_swaths, True, 0)

    f2c.Visualizer.figure()
    f2c.Visualizer.plot(route)
    # f2c.Visualizer.show()
    timestamp = datetime.datetime.now().strftime("%Y%m%d%H%M%S%f")
    f2c.Visualizer.save(f"output/route_r_{sorter_settings.algorithm}_{timestamp}.png");   

    return route


def generate_shortest_route(workingLanes: str, transport_lanes: str, start_point: Point, end_point: Point):
    swaths = f2c.Parser().importSwathsJsonFromString(workingLanes)
    transport_lanes_cells = import_transport_lanes(transport_lanes)
    swathsByCells = f2c.SwathsByCells()
    swathsByCells.push_back(swaths)
    
    route = rp.genShortestRoute(transport_lanes_cells, swathsByCells, to_point(start_point), to_point(end_point), 0, True)

    f2c.Visualizer.figure()
    f2c.Visualizer.plot(route)
    timestamp = datetime.datetime.now().strftime("%Y%m%d%H%M%S%f")
    f2c.Visualizer.save(f"output/shortestRoute_r_{timestamp}.png")
    
    f2c.Visualizer.figure()
    f2c.Visualizer.plot(swathsByCells)
    f2c.Visualizer.plot(transport_lanes_cells)
    timestamp = datetime.datetime.now().strftime("%Y%m%d%H%M%S%f")
    f2c.Visualizer.save(f"output/shortestRoute_lanes_{timestamp}.png")

    return route

