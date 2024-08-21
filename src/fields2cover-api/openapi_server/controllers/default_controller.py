import connexion
from typing import Dict
from typing import Tuple
from typing import Union

from openapi_server.models.generate_path_post_request import GeneratePathPostRequest  # noqa: E501
from openapi_server.models.path import Path  # noqa: E501
from openapi_server import util
from openapi_server.controllers.api import generate_path

def generate_path_post(generate_path_post_request):  # noqa: E501
    """Generate a path based on field data

     # noqa: E501

    :param generate_path_post_request: 
    :type generate_path_post_request: dict | bytes

    :rtype: Union[Path, Tuple[Path, int], Tuple[Path, int, Dict[str, str]]
    """
    
    if not connexion.request.is_json:
        return
    
    req: GeneratePathPostRequest = GeneratePathPostRequest.from_dict(connexion.request.get_json())  # noqa: E501
    
    path = generate_path(req._field, req.working_lanes, req.transport_lanes, req.start_end_point, 2.0, 2.0, 3.14, 3.14, 2.0)
    
    return path


