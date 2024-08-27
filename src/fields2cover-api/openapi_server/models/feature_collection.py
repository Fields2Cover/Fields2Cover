from datetime import date, datetime  # noqa: F401

from typing import List, Dict  # noqa: F401

from openapi_server.models.base_model import Model
from openapi_server.models.feature import Feature
from openapi_server.models.geo_json_object import GeoJsonObject
from openapi_server import util

from openapi_server.models.feature import Feature  # noqa: E501
from openapi_server.models.geo_json_object import GeoJsonObject  # noqa: E501

class FeatureCollection(Model):
    """NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).

    Do not edit the class manually.
    """

    def __init__(self, type=None, bbox=None, features=None):  # noqa: E501
        """FeatureCollection - a model defined in OpenAPI

        :param type: The type of this FeatureCollection.  # noqa: E501
        :type type: str
        :param bbox: The bbox of this FeatureCollection.  # noqa: E501
        :type bbox: List[float]
        :param features: The features of this FeatureCollection.  # noqa: E501
        :type features: List[Feature]
        """
        self.openapi_types = {
            'type': str,
            'bbox': List[float],
            'features': List[Feature]
        }

        self.attribute_map = {
            'type': 'type',
            'bbox': 'bbox',
            'features': 'features'
        }

        self._type = type
        self._bbox = bbox
        self._features = features

    @classmethod
    def from_dict(cls, dikt) -> 'FeatureCollection':
        """Returns the dict as a model

        :param dikt: A dict.
        :type: dict
        :return: The FeatureCollection of this FeatureCollection.  # noqa: E501
        :rtype: FeatureCollection
        """
        return util.deserialize_model(dikt, cls)

    @property
    def type(self) -> str:
        """Gets the type of this FeatureCollection.


        :return: The type of this FeatureCollection.
        :rtype: str
        """
        return self._type

    @type.setter
    def type(self, type: str):
        """Sets the type of this FeatureCollection.


        :param type: The type of this FeatureCollection.
        :type type: str
        """
        allowed_values = ["Feature", "FeatureCollection", "Point", "MultiPoint", "LineString", "MultiLineString", "Polygon", "MultiPolygon", "GeometryCollection"]  # noqa: E501
        if type not in allowed_values:
            raise ValueError(
                "Invalid value for `type` ({0}), must be one of {1}"
                .format(type, allowed_values)
            )

        self._type = type

    @property
    def bbox(self) -> List[float]:
        """Gets the bbox of this FeatureCollection.

        A GeoJSON object MAY have a member named \"bbox\" to include information on the coordinate range for its Geometries, Features, or FeatureCollections. The value of the bbox member MUST be an array of length 2*n where n is the number of dimensions represented in the contained geometries, with all axes of the most southwesterly point followed by all axes of the more northeasterly point. The axes order of a bbox follows the axes order of geometries.   # noqa: E501

        :return: The bbox of this FeatureCollection.
        :rtype: List[float]
        """
        return self._bbox

    @bbox.setter
    def bbox(self, bbox: List[float]):
        """Sets the bbox of this FeatureCollection.

        A GeoJSON object MAY have a member named \"bbox\" to include information on the coordinate range for its Geometries, Features, or FeatureCollections. The value of the bbox member MUST be an array of length 2*n where n is the number of dimensions represented in the contained geometries, with all axes of the most southwesterly point followed by all axes of the more northeasterly point. The axes order of a bbox follows the axes order of geometries.   # noqa: E501

        :param bbox: The bbox of this FeatureCollection.
        :type bbox: List[float]
        """

        self._bbox = bbox

    @property
    def features(self) -> List[Feature]:
        """Gets the features of this FeatureCollection.


        :return: The features of this FeatureCollection.
        :rtype: List[Feature]
        """
        return self._features

    @features.setter
    def features(self, features: List[Feature]):
        """Sets the features of this FeatureCollection.


        :param features: The features of this FeatureCollection.
        :type features: List[Feature]
        """
        if features is None:
            raise ValueError("Invalid value for `features`, must not be `None`")  # noqa: E501

        self._features = features