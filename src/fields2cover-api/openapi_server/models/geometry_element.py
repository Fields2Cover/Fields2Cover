from datetime import date, datetime  # noqa: F401

from typing import List, Dict  # noqa: F401

from openapi_server.models.base_model import Model
from openapi_server.models.geometry import Geometry
from openapi_server import util

from openapi_server.models.geometry import Geometry  # noqa: E501

class GeometryElement(Model):
    """NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).

    Do not edit the class manually.
    """

    def __init__(self, type=None, bbox=None):  # noqa: E501
        """GeometryElement - a model defined in OpenAPI

        :param type: The type of this GeometryElement.  # noqa: E501
        :type type: str
        :param bbox: The bbox of this GeometryElement.  # noqa: E501
        :type bbox: List[float]
        """
        self.openapi_types = {
            'type': str,
            'bbox': List[float]
        }

        self.attribute_map = {
            'type': 'type',
            'bbox': 'bbox'
        }

        self._type = type
        self._bbox = bbox

    @classmethod
    def from_dict(cls, dikt) -> 'GeometryElement':
        """Returns the dict as a model

        :param dikt: A dict.
        :type: dict
        :return: The GeometryElement of this GeometryElement.  # noqa: E501
        :rtype: GeometryElement
        """
        return util.deserialize_model(dikt, cls)

    @property
    def type(self) -> str:
        """Gets the type of this GeometryElement.


        :return: The type of this GeometryElement.
        :rtype: str
        """
        return self._type

    @type.setter
    def type(self, type: str):
        """Sets the type of this GeometryElement.


        :param type: The type of this GeometryElement.
        :type type: str
        """
        allowed_values = ["Point", "MultiPoint", "LineString", "MultiLineString", "Polygon", "MultiPolygon"]  # noqa: E501
        if type not in allowed_values:
            raise ValueError(
                "Invalid value for `type` ({0}), must be one of {1}"
                .format(type, allowed_values)
            )

        self._type = type

    @property
    def bbox(self) -> List[float]:
        """Gets the bbox of this GeometryElement.

        A GeoJSON object MAY have a member named \"bbox\" to include information on the coordinate range for its Geometries, Features, or FeatureCollections. The value of the bbox member MUST be an array of length 2*n where n is the number of dimensions represented in the contained geometries, with all axes of the most southwesterly point followed by all axes of the more northeasterly point. The axes order of a bbox follows the axes order of geometries.   # noqa: E501

        :return: The bbox of this GeometryElement.
        :rtype: List[float]
        """
        return self._bbox

    @bbox.setter
    def bbox(self, bbox: List[float]):
        """Sets the bbox of this GeometryElement.

        A GeoJSON object MAY have a member named \"bbox\" to include information on the coordinate range for its Geometries, Features, or FeatureCollections. The value of the bbox member MUST be an array of length 2*n where n is the number of dimensions represented in the contained geometries, with all axes of the most southwesterly point followed by all axes of the more northeasterly point. The axes order of a bbox follows the axes order of geometries.   # noqa: E501

        :param bbox: The bbox of this GeometryElement.
        :type bbox: List[float]
        """

        self._bbox = bbox
