import connexion
from openapi_server.models.generate_route_post_request import GenerateRoutePostRequest  # noqa: E501
from openapi_server.models.generate_shortest_route_post_request import GenerateShortestRoutePostRequest  # noqa: E501
from openapi_server.models.path import Path  # noqa: E501
from openapi_server.services.route_service import generate_route, generate_shortest_route

import fields2cover as f2c
import json

def removeduplicate(it):
    prev = it[0]
    for x in it:
        if x != prev:
            yield x
        prev = x

def route_to_json(route: f2c.Route) -> str:
    jString = route.asLineString().exportToJson()
    #parse json
    parsed_json = json.loads(jString)
    
    # sometimes points are there twice, remove them
    parsed_json['coordinates'] = list(removeduplicate(parsed_json['coordinates']))
    
    # print(parsed_json)
    # continue with the parsed json data
    return parsed_json


def generate_route_post(generate_route_post_request):  # noqa: E501
    """Generate a route based on field data

     # noqa: E501

    :param generate_route_post_request: 
    :type generate_route_post_request: dict | bytes

    :rtype: Union[str, Tuple[str, int], Tuple[str, int, Dict[str, str]]
    """
    if not connexion.request.is_json:
        return
    req_json = connexion.request.get_json()
    req = GenerateRoutePostRequest.from_dict(req_json)  # noqa: E501
    
        
    route = generate_route(json.dumps(req_json["workingLanes"]), json.dumps(req_json["transportLanes"]), req.start_end_point, req.sorter_settings)    
    return route_to_json(route)

def generate_shortest_route_post(generate_shortest_route_post_request):  # noqa: E501
    """Generate a route based on field data

     # noqa: E501

    :param generate_route_post_request: 
    :type generate_route_post_request: dict | bytes

    :rtype: Union[str, Tuple[str, int], Tuple[str, int, Dict[str, str]]
    """
    if not connexion.request.is_json:
        return
    req_json = connexion.request.get_json()
    req = GenerateShortestRoutePostRequest.from_dict(req_json)  # noqa: E501
    
        
    route = generate_shortest_route(json.dumps(req_json["workingLanes"]), json.dumps(req_json["transportLanes"]), req.start_point, req.end_point)    
    return route_to_json(route)
