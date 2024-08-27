import connexion
from typing import Dict
from typing import Tuple
from typing import Union

from openapi_server.models.generate_path_post_request import GeneratePathPostRequest  # noqa: E501
from openapi_server.models.generate_route_post_request import GenerateRoutePostRequest  # noqa: E501
from openapi_server.models.line_string import LineString  # noqa: E501
from openapi_server.models.path import Path  # noqa: E501
from openapi_server import util


def generate_path_post(generate_path_post_request):  # noqa: E501
    """Generate a path based on field data

     # noqa: E501

    :param generate_path_post_request: 
    :type generate_path_post_request: dict | bytes

    :rtype: Union[Path, Tuple[Path, int], Tuple[Path, int, Dict[str, str]]
    """
    if connexion.request.is_json:
        generate_path_post_request = GeneratePathPostRequest.from_dict(connexion.request.get_json())  # noqa: E501
    return 'do some magic!'


def generate_route_post(generate_route_post_request):  # noqa: E501
    """Generate a route based on field data

     # noqa: E501

    :param generate_route_post_request: 
    :type generate_route_post_request: dict | bytes

    :rtype: Union[LineString, Tuple[LineString, int], Tuple[LineString, int, Dict[str, str]]
    """
    if connexion.request.is_json:
        generate_route_post_request = GenerateRoutePostRequest.from_dict(connexion.request.get_json())  # noqa: E501
    return 'do some magic!'
