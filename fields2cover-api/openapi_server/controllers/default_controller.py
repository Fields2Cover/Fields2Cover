import connexion
from typing import Dict
from typing import Tuple
from typing import Union

from openapi_server.models.generate_path_post_request import GeneratePathPostRequest  # noqa: E501
from openapi_server.models.generate_route_post200_response import GenerateRoutePost200Response  # noqa: E501
from openapi_server.models.generate_route_post_request import GenerateRoutePostRequest  # noqa: E501
from openapi_server.models.generate_shortest_route_post_request import GenerateShortestRoutePostRequest  # noqa: E501
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

    :rtype: Union[GenerateRoutePost200Response, Tuple[GenerateRoutePost200Response, int], Tuple[GenerateRoutePost200Response, int, Dict[str, str]]
    """
    if connexion.request.is_json:
        generate_route_post_request = GenerateRoutePostRequest.from_dict(connexion.request.get_json())  # noqa: E501
    return 'do some magic!'


def generate_shortest_route_post(generate_shortest_route_post_request):  # noqa: E501
    """Generate the shortest route based on field data

     # noqa: E501

    :param generate_shortest_route_post_request: 
    :type generate_shortest_route_post_request: dict | bytes

    :rtype: Union[GenerateRoutePost200Response, Tuple[GenerateRoutePost200Response, int], Tuple[GenerateRoutePost200Response, int, Dict[str, str]]
    """
    if connexion.request.is_json:
        generate_shortest_route_post_request = GenerateShortestRoutePostRequest.from_dict(connexion.request.get_json())  # noqa: E501
    return 'do some magic!'
