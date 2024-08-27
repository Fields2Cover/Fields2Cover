import connexion

from openapi_server.models.generate_path_post_request import GeneratePathPostRequest  # noqa: E501
from openapi_server.models.path import Path  # noqa: E501
from openapi_server.services.path_service import generate_path

import json

import fields2cover as f2c

def path_to_json(path: f2c.Path) -> Path:
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


def generate_path_post(generate_path_post_request):  # noqa: E501
    """Generate a path based on field data

     # noqa: E501

    :param generate_path_post_request: 
    :type generate_path_post_request: dict | bytes

    :rtype: Union[Path, Tuple[Path, int], Tuple[Path, int, Dict[str, str]]
    """
    if not connexion.request.is_json:        
        return
    req_json = connexion.request.get_json()
    req = GeneratePathPostRequest.from_dict(req_json)  # noqa: E501
    path = generate_path(json.dumps(req_json["workingLanes"]), json.dumps(req_json["transportLanes"]), req.start_end_point, req.robot_settings , req.sorter_settings, req.curves_algorithm)    
    return path_to_json(path)
