import fields2cover as f2c
from openapi_server.models.point import Point
from openapi_server.dto.robot_settings import RobotSettings
from openapi_server.dto.sorter_settings import SorterSettings
from openapi_server.services.route_service import generate_route

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
    return robot

def generate_path(workingLanes: str, transport_lanes: str, start_and_end_point: Point, robotSettings: RobotSettings, sorter_settings: SorterSettings, curvesAlgorithm: str):    

    robot = create_robot(robotSettings)
    route = generate_route(workingLanes, transport_lanes, start_and_end_point, sorter_settings)
    curves = determine_curves(curvesAlgorithm)
        
    path: f2c.Path = f2c.PP_PathPlanning().planPath(robot, route, curves, True)
    
    f2c.Visualizer.figure()
    f2c.Visualizer.plot(path)
    f2c.Visualizer.save("path.png");    

    return path


