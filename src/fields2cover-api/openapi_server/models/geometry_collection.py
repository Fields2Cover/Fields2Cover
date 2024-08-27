from datetime import date, datetime  # noqa: F401

from typing import List, Dict  # noqa: F401

from openapi_server.models.base_model import Model
from openapi_server.models.geometry import Geometry
from openapi_server.models.geometry_element import GeometryElement
from openapi_server import util

from openapi_server.models.geometry import Geometry  # noqa: E501
from openapi_server.models.geometry_element import GeometryElement  # noqa: E501

class GeometryCollection(Model):
    """NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).

    Do not edit the class manually.
    """

    def __init__(self, type=None, bbox=None, geometries=None):  # noqa: E501
        """GeometryCollection - a model defined in OpenAPI

        :param type: The type of this GeometryCollection.  # noqa: E501
        :type type: str
        :param bbox: The bbox of this GeometryCollection.  # noqa: E501
        :type bbox: List[float]
        :param geometries: The geometries of this GeometryCollection.  # noqa: E501
        :type geometries: List[GeometryElement]
        """
        self.openapi_types = {
            'type': str,
            'bbox': List[float],
            'geometries': List[GeometryElement]
        }

        self.attribute_map = {
            'type': 'type',
            'bbox': 'bbox',
            'geometries': 'geometries'
        }

        self._type = type
        self._bbox = bbox
        self._geometries = geometries

    @classmethod
    def from_dict(cls, dikt) -> 'GeometryCollection':
        """Returns the dict as a model

        :param dikt: A dict.
        :type: dict
        :return: The GeometryCollection of this GeometryCollection.  # noqa: E501
        :rtype: GeometryCollection
        """
        return util.deserialize_model(dikt, cls)

    @property
    def type(self) -> str:
        """Gets the type of this GeometryCollection.


        :return: The type of this GeometryCollection.
        :rtype: str
        """
        return self._type

    @type.setter
    def type(self, type: str):
        """Sets the type of this GeometryCollection.


        :param type: The type of this GeometryCollection.
        :type type: str
        """
        allowed_values = ["Point", "MultiPoint", "LineString", "MultiLineString", "Polygon", "MultiPolygon", "GeometryCollection"]  # noqa: E501
        if type not in allowed_values:
            raise ValueError(
                "Invalid value for `type` ({0}), must be one of {1}"
                .format(type, allowed_values)
            )

        self._type = type

    @property
    def bbox(self) -> List[float]:
        """Gets the bbox of this GeometryCollection.

        A GeoJSON object MAY have a member named \"bbox\" to include information on the coordinate range for its Geometries, Features, or FeatureCollections. The value of the bbox member MUST be an array of length 2*n where n is the number of dimensions represented in the contained geometries, with all axes of the most southwesterly point followed by all axes of the more northeasterly point. The axes order of a bbox follows the axes order of geometries.   # noqa: E501

        :return: The bbox of this GeometryCollection.
        :rtype: List[float]
        """
        return self._bbox

    @bbox.setter
    def bbox(self, bbox: List[float]):
        """Sets the bbox of this GeometryCollection.

        A GeoJSON object MAY have a member named \"bbox\" to include information on the coordinate range for its Geometries, Features, or FeatureCollections. The value of the bbox member MUST be an array of length 2*n where n is the number of dimensions represented in the contained geometries, with all axes of the most southwesterly point followed by all axes of the more northeasterly point. The axes order of a bbox follows the axes order of geometries.   # noqa: E501

        :param bbox: The bbox of this GeometryCollection.
        :type bbox: List[float]
        """

        self._bbox = bbox

    @property
    def geometries(self) -> List[GeometryElement]:
        """Gets the geometries of this GeometryCollection.


        :return: The geometries of this GeometryCollection.
        :rtype: List[GeometryElement]
        """
        return self._geometries

    @geometries.setter
    def geometries(self, geometries: List[GeometryElement]):
        """Sets the geometries of this GeometryCollection.


        :param geometries: The geometries of this GeometryCollection.
        :type geometries: List[GeometryElement]
        """
        if geometries is None:
            raise ValueError("Invalid value for `geometries`, must not be `None`")  # noqa: E501
        if geometries is not None and len(geometries) < 0:
            raise ValueError("Invalid value for `geometries`, number of items must be greater than or equal to `0`")  # noqa: E501

        self._geometries = geometries
